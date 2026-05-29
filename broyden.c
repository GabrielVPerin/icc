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

double derivadas_broyden(size_t linha, size_t variavel, size_t n, double *parametros_atuais)
{
    if(linha == variavel)
        return (-4 * parametros_atuais[linha] + 3);
    else if(linha == variavel - 1)
        return -2;
    else if(linha == variavel +1)
        return -1;
    else
        return 0;

    
}
