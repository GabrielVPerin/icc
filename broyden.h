#ifndef BROYDEN_H
#define BROYDEN_H

#include <stdio.h>

#include "utils.h"

// Implementa o método de newton a partir das funções acima
// Recebe respectivamente
// - Um vetor com os valores iniciais
// - Um epsilon de erro
// - Tamanho do sistema
// - Quantidade maxima de iterações
void newton(double *X, double epsilon, size_t n, long long max);

extern rtime_t tempoJacobiana;

extern rtime_t tempoSL;

extern FILE *saidaArq;

#endif
