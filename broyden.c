#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double equacoes_broyden(size_t linha, size_t n, double *parametros_atuais)
{
    if(linha == 0)
        return (-2 * pow(parametros_atuais[0], 2) + 3 * parametros_atuais[0] - 2 * parametros_atuais[1] + 1);
    else if(linha == n-1)
        return(-2 * pow(parametros_atuais[linha], 2) + 3 * parametros_atuais[linha] - parametros_atuais[linha-1]);
    else
        return (-2 * pow(parametros_atuais[linha], 2) + 3 * parametros_atuais[linha] - parametros_atuais[linha-1] - 2 * parametros_atuais[linha+1] + 1);
}

double derivadas_broyden(size_t linha, size_t variavelDerivada, size_t n, double *parametros_atuais)
{
    if(linha == variavelDerivada)
        return (-4 * parametros_atuais[linha] + 3);
    else if(linha != (n - 1) && linha == (variavelDerivada - 1))
        return -2;
    else if(linha != 0 && linha == (variavelDerivada + 1))
        return -1;

    return 0;
}

int main()
{
    size_t n = 10;
    // double x0 = 2;

    double *fx = malloc(n * sizeof(double));
    double *x = malloc(n * sizeof(double));
    for(size_t i = 0; i < n; i++) {
        x[i] = i;
    }

    double **jacobiana = malloc(n * sizeof(double *));
    for(size_t i = 0; i < n; i++) {
        jacobiana[i] = malloc(n * sizeof(double));
    }

    for(size_t i = 0; i < n; i++) {
        for(size_t j = 0; j < n; j++) {
            jacobiana[i][j] = derivadas_broyden(i, j, n, x);
            printf("%.2f ", jacobiana[i][j]);
        }
        puts("");
    }

    puts("");

    for(size_t i = 0; i < n; i++) {
        fx[i] = equacoes_broyden(i, n, x);
        printf("%.2f ", fx[i]);
    }

    puts("");

    return EXIT_SUCCESS;
}
