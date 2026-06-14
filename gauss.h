#ifndef GAUSS_H
#define GAUSS_H

#include <stddef.h>

#include "utils.h"

void resolve_sl(double * restrict d, double * restrict a, double * restrict c, double * restrict b, double * restrict X, size_t n);

extern rtime_t tempoSL;

#endif