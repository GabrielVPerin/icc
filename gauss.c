#include <stddef.h>

#include "gauss.h"

// POSSIVEIS OTIMIZAÇÕES
// 1 - É possivel utilizar uma eliminacao de gauss simplificada para matrizes 3-diagonais (tem uma aula sobre isso)
// 2 - Loop unrolling

// Implementa eliminacao de gauss
void eliminacao_gauss(double * restrict d, double * restrict a, double * restrict c, double * restrict b, size_t n)
{
    double m;

    for(size_t i = 0; i < n-1; i++) {
        m = a[i] / d[i];
        a[i] = 0.0;
        d[i+1] -= c[i] * m;
        b[i+1] -= b[i] * m;
    }
}

// Resolve um sistema linear triangular
void sl_triangular(const double * restrict d, const double * restrict c, double * restrict b, double * restrict x, size_t n)
{
    x[n-1] = b[n-1] / d[n-1];
    for(long long i = n-2; i >= 0; i--) {
        x[i] = (b[i] - c[i] * x[i+1]) / d[i];
    }
}
