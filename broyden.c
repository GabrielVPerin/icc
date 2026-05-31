#include <endian.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "gauss.h"
#include "utils.h"
#include "broyden.h"

// POSSIVEIS OTIMIZAÇÕES
// 1 - Não utilizar a função pow
// 2 - Melhorar alocação de matriz
// 3 - Jacobiana é formada por uma matriz 3-diagonal, talvez seja possivel representá-la por 3 vetores
// 4 - Usar funções inline (principalmente nas funções auxiliares)

rtime_t tempoJacobiana = 0;
rtime_t tempoSL = 0;
FILE *saidaArq;

// Calcula o resultado de uma linha do sistema de broyden
double equacoes_broyden(size_t linha, size_t n, double *X)
{
    if(linha == 0)
        return (-2.0 * pow(X[0], 2) + 3.0 * X[0] - 2.0 * X[1] + 1.0);
    else if(linha == n-1)
        return(-2.0 * pow(X[linha], 2) + 3.0 * X[linha] - X[linha-1]);

    return (-2.0 * pow(X[linha], 2) + 3.0 * X[linha] - X[linha-1] - 2.0 * X[linha+1] + 1.0);
}

// Calcula o resultado da derivada de uma linha do sistema de broyden
double derivadas_broyden(size_t linha, size_t variavelDerivada, double *X)
{
    if(linha == variavelDerivada)
        return (-4.0 * X[linha] + 3.0);
    else if(linha == (variavelDerivada - 1))
        return -2.0;
    else if(linha == (variavelDerivada + 1))
        return -1.0;

    return 0.0;
}

// Calcula todas as linhas do sistema de broyden
void calcula_broyden(double *Fx, double *X, size_t n)
{
    for(size_t i = 0; i < n; i++)
        Fx[i] = equacoes_broyden(i, n, X);
}

// Cria a matriz jacobiana a partir das derivadas
void calcula_jacobiana(double **jacobiana, double *X, size_t n)
{
    rtime_t tempoAntes = timestamp();

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++)
            jacobiana[i][j] = derivadas_broyden(i, j, X);
    }

    tempoJacobiana += timestamp() - tempoAntes;
}

// Encontra o valor em modulo maximo de um vetor
double max_vetor(double *X, size_t n)
{
    double melhor = fabs(X[0]);

    for(size_t i = 1; i < n; i++) {
        if(fabs(X[i]) > melhor)
            melhor = fabs(X[i]);
    }

    return melhor;
}

// Inverte o sinal de todos os valores do vetor
void inverte_vetor(double *X, size_t n)
{
    for(size_t i = 0; i < n; i++)
        X[i] = -X[i];
}

// Resolve um SL
void resolve_sl(double **A, double *b, double *X, size_t n)
{
    rtime_t tempoAntes = timestamp();

    eliminacao_gauss(A, b, n);
    sl_triangular(A, b, X, n);

    tempoSL += timestamp() - tempoAntes;
}

// Soma dois vetores e guarda o resultado no primeiro vetor
void soma_vetores(double *a, double *b, size_t n)
{
    for(size_t i = 0; i < n; i++)
        a[i] += b[i];
}

// Aloca uma matriz de tamanho n
double **aloca_matriz(size_t n)
{
    double **matriz = malloc(n * sizeof(double *));
    for(size_t i = 0; i < n; i++) {
        matriz[i] = malloc(n * sizeof(double));
    }

    return matriz;
}

// Destroi uma matriz de tamanho n
void destroi_matriz(double **matriz, size_t n)
{
    for(size_t i = 0; i < n; i++) {
        free(matriz[i]);
    }

    free(matriz);
}

// Printa um vetor (usado para debug)
void print_X(double *X, size_t n)
{
    for(size_t i = 0; i < n; i++)
        fprintf(saidaArq, "x%zu = %f\n", i+1, X[i]);
}

// Implementa o método de newton a partir das funções acima
void newton(double *X, double epsilon, size_t n, long long max)
{
    double *delta = malloc(n * sizeof(double));
    double *Fx = malloc(n * sizeof(double));
    double **jacobiana = aloca_matriz(n);

    for(long long i = 0; i < max-1; i++) {
        fprintf(saidaArq, "#\n");
        calcula_broyden(Fx, X, n);
        if(max_vetor(Fx, n) < epsilon)
            break;

        calcula_jacobiana(jacobiana, X, n);
        inverte_vetor(Fx, n);
        resolve_sl(jacobiana, Fx, delta, n);
        soma_vetores(X, delta, n);

        print_X(X, n);

        if(max_vetor(delta, n) < epsilon)
            break;
    }

    free(delta);
    free(Fx);
    destroi_matriz(jacobiana, n);
}
