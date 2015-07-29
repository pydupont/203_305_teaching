#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define WORKTAG 1
#define DIETAG 2

#define MAXLINE 4096

/*
 Imbrace your inner lazy!
 This is program uses the message passing interface (MPI) to distribute jobs in what we call a 'command file'
 onto multiple processors on a PBS batch system (It may be useful on other systems too...).
 
 A command file is a file that has, you guessed it, unix commands in it.
 So, if the command
 blah -h foo -t bar > output.txt is a command you'd like to run, you can put this in the command file
 (along with 5000 other commands you want to be run)
 and this program will distribute those commands across the N processors requested
 
 to compile this, type:
 
 mpicc -Wall -o clazyMPI clazyMPI.c
 
 
 Typical usage is:
 
 mpirun -np N clazyMPI commandFile.txt
 
 where N is the number of processes you wish to distribute the work upon
 
 The default use of this program allocates one 'coordinator' process, and N-1 'worker' processes
 where the coordinator gives the N-1 workers work from the command file.
 
 NOTE: the command above should be in the submit script you give to the super computer (eg, ice, marin)!
 
 Alternate usage is:
 
 mpirun -np N clazyMPI -u commandFile.txt
 
 This is the 'uncoordinated' way of running this code. This method does NOT use a coordinator process
 to allocate the work; rather the work is evenly divided up by the workers (very egaletarian)
 This will work well if:
 	The workload is homogenous
	the amount of work is very large (law of large numbers)
	the amount of work exceeds the amount of time allotable on PBS (as long as no 1 process runs out of work)
	if number of commands in the command file is equal to the number of CPUs available (no workload to balance)


  Troubleshooting:
  
  If you have problems, first try:
  chmod +x commandfile
  (this makes your command file into a script.)
  Now type:
  ./commandfile
  This will execute the first command in the command file (followed by the second, third...)
  If you see error messages, your command file is malformed
  (remember ctrl-c will cancel the run).
  
  If the above works fine, try:
  chmod +x ./submitScript.pbs
  ./submitScript.pbs
  
  If this fails, then there's a problem with the mpirun command (or another command in your submit script).
  
  (if you don't know what a submit script is, go to:
  http://anise.u.arizona.edu/HPC2/02_hpc/02_hpc_starting/03_05_hpc_user_guide_for_SGI_ICE_cluster.shtml
  )
  
  If the above still works, then you have a bug in the submit script itself...

  Also note that this program will report that a command 'failed' if it exits unnaturally from main.
  Some programs do not exit naturally when they should, so if you see lots of errors of this type
  this may be what's occurring.
  
  If you want these warning messages to go away, you can use the -q flag (quiet). eg:
  
  mpirun -np N clazyMPI -q commandFile.txt
  or
  mpirun -np N clazyMPI -q -u commandFile.txt


Suggestions? Comments? contact me!

Author:
August Woerner
augustw AT email DOT arizona DOT edu


*/


void worker(int id, int quiet);
void coordinator(char *argv, int ncpus);
void uncoordinated(char *argv, int id, int cpus, int quiet);

int
main(int argc, char** argv) {

  int id, cpus, help = 0, coordinated = 1, i, fileIndex = 0, quiet = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &cpus);

// parse the argv
  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'h')
        help = 1;
      else if (argv[i][1] == 'q')
        quiet = 1;
      else if (argv[i][1] == 'c')
        coordinated = 1;
      else if (argv[i][1] == 'u')
        coordinated = 0;
      else if (id == 0){
        fprintf(stderr, "Failed to parse flag: %s\n\tExpected -h -u -q or -c\n", argv[i]);
	help = 1;
      }
      argv[i] = NULL;   
    } else if (fileIndex == 0)
      fileIndex = i;
  }

  if (! fileIndex) {
    fprintf(stderr, 
    	"Failed to find the command file!\n");
    help = 1;
  }
  
  if (coordinated && cpus < 2) {
    fprintf(stderr, "Doh! You cannot have a coordinated run AND request less than two processors!\n");
    help = 1;
  }


  if (help) {
    if (id == 0) {
      fprintf(stderr, 
      	"Correct usage: %s (Flags) commandFile\nFlags:\n\t-h (help)\n\t-c (This is a default setting. This requests a coordinated run; IE load balancing. It's recommended if the runtime of the commands is heterogenous ", argv[0]);
      fprintf(stderr,
      	"and if you expect the amount of work in the command file is less than the amount of time allotted. Note that you will have 1 less processor performing work with this approach.)\n");
      fprintf(stderr,"\n\t-u (uncoordinated run. This is recommended if the runtimes are homogenous, or if you do not expect to finish to task in the allotted time)\n");
      fprintf(stderr, "\n\t-q (this suppresses warning messages\n");
    }
  } else if (coordinated) {

    if (id == 0) {
      coordinator(argv[fileIndex], cpus);
    } else {
      worker(id, quiet);
    }
    
  } else {
    uncoordinated(argv[fileIndex], id, cpus, quiet);
  }

  MPI_Finalize();
  return 0;
}


void
uncoordinated(char *argv, int id, int cpus, int quiet) {

  int i;
  FILE *file = NULL;


  file = fopen(argv, "r");
  if (file == NULL) {
    if (id == 0) 
      fprintf(stderr, "Failed to open %s for reading\n", argv);

    return;
  }

  char line[MAXLINE];
  i = 0;

// Distribute the work accross the N processors
  while ( fgets(line, MAXLINE, file) != NULL) {
    if (i % cpus == id) {
      if (system(line) && !quiet) {
        fprintf(stderr, "ID: %d Failed to execute command:\n%s\n",
       			id, line);
      }
    }
    ++i;
  }

  fclose(file);

}

void
worker(int id, int quiet) {

  MPI_Status status;
  char line[MAXLINE];
  
  while (1) {
  // tell the 'master' that this process needs more work to do
    MPI_Send(&id,
    		1,
		MPI_INT,
		0,
		WORKTAG,
		MPI_COMM_WORLD);
  
  // receive the message, and do the work

    MPI_Recv((void*) line, 
    		MAXLINE,
		MPI_CHAR,
		0,
		MPI_ANY_TAG,
		MPI_COMM_WORLD,
		&status);
     
     if (status.MPI_TAG == DIETAG)
       return;
  
   // printf("%d\n%s\n", id, line);
  
     if (system(line) && !quiet) {
       fprintf(stderr, "ID: %d Failed to execute command:\n%s\n",
       			id, line);
     }
  }


}



void
coordinator(char *argv, int ncpus) {
  char line[MAXLINE];
  int i, workerId;
  MPI_Status status;
  
  FILE *file = NULL;
  file = fopen(argv, "r");
  if (file == NULL) {
    fprintf(stderr, "Failed to open %s for reading\n", argv);
    goto DONE;
  }

  i = 0;

// Distribute the work accross the N processors
  while ( fgets(line, MAXLINE, file) != NULL) {
    // receive a request for more work

    MPI_Recv(&workerId,
    		1,
		MPI_INT,
		MPI_ANY_SOURCE,
		WORKTAG,
		MPI_COMM_WORLD,
		&status);	

    // and give it to the requesting process
    MPI_Send(line,
    		MAXLINE,
		MPI_CHAR,
		workerId,
		WORKTAG,
		MPI_COMM_WORLD);
    
    ++i;
  }

  fclose(file);


DONE:
/*
After all of the work has been doled out, tell the workers that they're done
*/
  for (i = 1; i < ncpus; ++i) {
    MPI_Recv(&workerId,
    		1,
		MPI_INT,
		MPI_ANY_SOURCE,
		WORKTAG,
		MPI_COMM_WORLD,
		&status);	
		
    // and give it to the calling process
    MPI_Send(line,
    		MAXLINE,
		MPI_CHAR,
		workerId,
		DIETAG,
		MPI_COMM_WORLD);
  
  
  }

}
