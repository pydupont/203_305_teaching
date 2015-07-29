import sys

# sys.argv is a list containing the command line parameters
# sys.argv[0] always contains the name of the python script
if len(sys.argv) != 2:
   print 'Error: give a file name on the command line'
   exit(1) # Exit and return the error code 1

csv_file_name = sys.argv[1]
csv = open(csv_file_name, 'r') #open the csv file
data = [[],[]] #initiation of the data list
for line in csv: #read the file
   line = line.rstrip() # remove the 'newline' character at the end of the line
   line = line.split(",")
   for i in range(len(line)): # read the data on the line
      data[i].append(float(line[i]))#append the current value in the data list
print data
csv.close() #close the csv file
