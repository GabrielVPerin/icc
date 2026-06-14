#ifndef GAUSS_H
#define GAUSS_H

#include <stddef.h>

#include "utils.h"

void resolve_sl(double *d, double *a, double *c, double *b, double *X, size_t n);

extern rtime_t tempoSL;

#endif
