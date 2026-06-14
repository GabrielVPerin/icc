#include <endian.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "gauss.h"
#include "utils.h"
#include "broyden.h"

#include <likwid.h>

// POSSIVEIS OTIMIZAÇÕES
// 1 - Não utilizar a função pow
// 2 - Melhorar alocação de matriz
// 3 - Jacobiana é formada por uma matriz 3-diagonal, talvez seja possivel representá-la por 3 vetores
// 4 - Usar funções inline (principalmente nas funções auxiliares)
// 5 - Loop unrolling

rtime_t tempoJacobiana = 0;
rtime_t tempoSL = 0;
FILE *saidaArq;

// Calcula o resultado de uma linha do sistema de broyden
static inline double equacoes_broyden(size_t linha, size_t n, double *X)
{
    if (linha == 0)
        return (-2.0 * (X[0] * X[0]) + 3.0 * X[0] - 2.0 * X[1] + 1.0);
    else if (linha == n - 1)
        return (-2.0 * (X[linha] * X[linha]) + 3.0 * X[linha] - X[linha - 1]);

    return (-2.0 * (X[linha] * X[linha])+ 3.0 * X[linha] - X[linha - 1] - 2.0 * X[linha + 1] + 1.0);
}

// Calcula o resultado da derivada de uma linha do sistema de broyden
static inline double derivadas_broyden(size_t linha, size_t variavelDerivada, double *X)
{
    if (linha == variavelDerivada)
        return (-4.0 * X[linha] + 3.0);
    else if (linha == (variavelDerivada - 1))
        return -2.0;
    else if (linha == (variavelDerivada + 1))
        return -1.0;

    return 0.0;
}

// Calcula todas as linhas do sistema de broyden
static void calcula_broyden(double * restrict Fx, double * restrict X, size_t n)
{
    for (size_t i = 0; i < n; i++)
        Fx[i] = equacoes_broyden(i, n, X);
}

// Cria a matriz jacobiana a partir das derivadas
static void calcula_jacobiana(double * restrict a, double * restrict c, double * restrict d, double * restrict X, size_t n)
{
    rtime_t tempoAntes = timestamp();
    LIKWID_MARKER_START("Jacobiana");
    for (size_t i = 0; i < n; i++)
    {
        d[i] = derivadas_broyden(i, i, X);
        a[i] = derivadas_broyden(i + 1, i, X);
        c[i] = derivadas_broyden(i, i + 1, X);
    }

    LIKWID_MARKER_STOP("Jacobiana");
    tempoJacobiana += timestamp() - tempoAntes;
}

// Encontra o valor em modulo maximo de um vetor
static inline double max_vetor(double *X, size_t n)
{
    double melhor = fabs(X[0]);

    for (size_t i = 1; i < n; i++)
    {
        if (fabs(X[i]) > melhor)
            melhor = fabs(X[i]);
    }

    return melhor;
}

// Inverte o sinal de todos os valores do vetor
static inline void inverte_vetor(double *X, size_t n)
{
    for (size_t i = 0; i < n; i++)
        X[i] = -X[i];
}

// Resolve um SL
static void resolve_sl(double * restrict d, double * restrict a, double * restrict c, double * restrict b, double * restrict X, size_t n)
{
    rtime_t tempoAntes = timestamp();
    LIKWID_MARKER_START("Gauss");

    eliminacao_gauss(d, a, c, b, n);
    sl_triangular(d, c, b, X, n);

    LIKWID_MARKER_STOP("Gauss");
    tempoSL += timestamp() - tempoAntes;
}

// Soma dois vetores e guarda o resultado no primeiro vetor
static inline void soma_vetores(double * restrict a, double * restrict b, size_t n)
{
    for (size_t i = 0; i < n; i++)
        a[i] += b[i];
}

// Printa um vetor (usado para debug)
static void print_X(double *X, size_t n)
{
    for (size_t i = 0; i < n; i++)
        fprintf(saidaArq, "x%zu = %f\n", i + 1, X[i]);
}

// Implementa o método de newton a partir das funções acima
// Recebe respectivamente
// - Um vetor com os valores iniciais
// - Um epsilon de erro
// - Tamanho do sistema
// - Quantidade maxima de iterações
void newton(double *X, double epsilon, size_t n, long long max)
{
    double *delta = malloc(n * sizeof(double));
    double *Fx = malloc(n * sizeof(double));
    double *d = malloc(n * sizeof(double));
    double *a = malloc(n * sizeof(double));
    double *c = malloc(n * sizeof(double));

    for (long long i = 0; i < max - 1; i++)
    {
        fprintf(saidaArq, "#\n");
        calcula_broyden(Fx, X, n);
        if (max_vetor(Fx, n) < epsilon)
            break;

        calcula_jacobiana(a, c, d, X, n);
        inverte_vetor(Fx, n);
        resolve_sl(d, a, c, Fx, delta, n);
        soma_vetores(X, delta, n);

        print_X(X, n);

        if (max_vetor(delta, n) < epsilon)
            break;
    }

    free(d);
    free(a);
    free(c);
    free(delta);
    free(Fx);
}
