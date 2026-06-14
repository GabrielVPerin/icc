#include <stddef.h>
#include <likwid.h>

#include "gauss.h"
#include "utils.h"


// POSSIVEIS OTIMIZAÇÕES
// 1 - É possivel utilizar uma eliminacao de gauss simplificada para matrizes 3-diagonais (tem uma aula sobre isso)
// 2 - Loop unrolling

rtime_t tempoSL = 0;

// Implementa eliminacao de gauss

static inline void eliminacao_gauss(double *d, double *a, double *c, double *b, size_t n)
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
static inline void retrosubstituicao(double *d, double *c, double *b, double *x, size_t n)
{
    x[n-1] = b[n-1] / d[n-1];
    for(long long i = n-2; i >= 0; i--) {
        x[i] = (b[i] - c[i] * x[i+1]) / d[i];
    }
}

// Resolve um SL
void resolve_sl(double *d, double *a, double *c, double *b, double *X, size_t n)
{
    rtime_t tempoAntes = timestamp();
    LIKWID_MARKER_START("Gauss");

    eliminacao_gauss(d, a, c, b, n);
    retrosubstituicao(d, c, b, X, n);

    LIKWID_MARKER_STOP("Gauss");
    tempoSL += timestamp() - tempoAntes;
}
