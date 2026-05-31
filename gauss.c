#include <stddef.h>

#include "gauss.h"

// POSSIVEIS OTIMIZAÇÕES
// 1 - É possivel utilizar uma eliminacao de gauss simplificada para matrizes 3-diagonais (tem uma aula sobre isso)
// 2 - Loop unrolling

// Implementa eliminacao de gauss
void eliminacao_gauss(double **A, double *b, size_t n)
{
    double m;

    for(size_t i = 0; i < n; i++) {
        for(size_t k = i+1; k < n; k++) {
            m = A[k][i] / A[i][i];
            A[k][i] = 0.0;
            for(size_t j = i+1; j < n; j++)
                A[k][j] -= A[i][j] * m;
            b[k] -= b[i] * m;
        }
    }
}

// Resolve um sistema linear triangular
void sl_triangular(double **A, double *b, double *x, size_t n)
{
    for(long i = n-1; i >= 0; i--) {
        x[i] = b[i];
        for(size_t j = i+1; j < n; j++)
            x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }
}
