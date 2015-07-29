namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;
using namespace std;
typedef ublas::bounded_matrix<double,3,3,ublas::column_major> mat;
ublas::bounded_vector<double,3> lambda;
ublas::bounded_vector<double,34*3> work; // the 34*width is from syev.hpp

mat A;
int i,j,k,idx;
#pragma omp parallel for private(i,j,k,A,idx,lambda,work)
for(i=0;i<nx;i++){
  for(j=0;j<ny;j++){
    for(k=0;k<nz;k++){
      A(0,0) = Dxx(i,j,k);  // fill upper triangle of A
      A(1,1) = Dyy(i,j,k);
      A(2,2) = Dzz(i,j,k);
      A(0,1) = Dxy(i,j,k);
      A(0,2) = Dxz(i,j,k);
      A(1,2) = Dyz(i,j,k);

      // determine eigenvalues (N) of upper (U) triangle of A in lambda
      lapack::syev('N','U',A,lambda,lapack::workspace(work));
      lambda1(i,j,k) = lambda(0);  // eigenvalues in ascending order
      lambda2(i,j,k) = lambda(1);
      lambda3(i,j,k) = lambda(2);
    }
  }
 }  
