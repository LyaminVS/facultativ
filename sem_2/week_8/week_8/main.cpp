#include <stdio.h>
#include <lapacke.h>
#include <iostream>
#include <array>

int main (int argc, const char * argv[])
{
//    double a[2 * 3] = {1, 2, 3, 4, 5, 6};
    
    
    const int N = 2;
    const int NRHS = 1;
    const int LDA = N;
    const int LDB = NRHS;
    const int n = N;
    const int nrhs = NRHS;
    const int lda = LDA;
    const int ldb = LDB;
    
    double M[LDA * N] = {1, 2, 3, 4};
    double V[LDB * N] = {5, 11};
    
    int ipiv[0];
    
    auto info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, n, nrhs, M, lda, ipiv, V, ldb);
    
    std::cout << info;
    
}
