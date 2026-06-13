#ifndef GAUSS_H
#define GAUSS_H

#include <stddef.h>

// Implementa eliminacao de gauss
void eliminacao_gauss(double **A, double *b, size_t n);

// Resolve um sistema linear triangular
void sl_triangular(double **A, double *b, double *x, size_t n);

#endif