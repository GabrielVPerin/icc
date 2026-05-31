#ifndef BROYDEN_H
#define BROYDEN_H

#include <stdio.h>

#include "utils.h"

void newton(double *X, double epsilon, size_t n, long long max);

extern rtime_t tempoJacobiana;

extern rtime_t tempoSL;

extern FILE *saidaArq;

#endif
