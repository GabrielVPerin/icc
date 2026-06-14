#ifndef GAUSS_H
#define GAUSS_H

#include <stddef.h>

// Implementa eliminacao de gauss
void eliminacao_gauss(double * restrict d, double * restrict a, double * restrict c, double * restrict b, size_t n);

// Resolve um sistema linear triangular
void sl_triangular(const double * restrict d, const double * restrict c, double * restrict b, double * restrict x, size_t n);

#endif
