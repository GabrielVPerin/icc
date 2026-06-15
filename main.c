#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <likwid.h>

#include "broyden.h"
#include "utils.h"
#include "gauss.h"

int main(int argc, char **argv)
{
    if (argc < 5 || argc > 7)
    {
        fprintf(stderr, "Uso do programa: ./broyden [N] [x0] [epsilon] [max] (opcional)[-o <arquivo_saida>]\n");
        return EXIT_FAILURE;
    }

    saidaArq = stdout;
    FILE *arquivoSaida = NULL;

    if (argc == 7 && !strcmp(argv[5], "-o"))
    {
        arquivoSaida = fopen(argv[6], "w");
        if (arquivoSaida == NULL)
        {
            fprintf(stderr, "Erro ao abrir arquivo\n");
        }
        saidaArq = arquivoSaida;
    }
    else if (argc > 5 && !strcmp(argv[5], "-o"))
    {
        fprintf(stderr, "É necessário informar nome de arquivo: -o <arquivo_saida>\n");
        return EXIT_FAILURE;
    }

    LIKWID_MARKER_INIT;

    size_t n = atoll(argv[1]);
    double x0 = atof(argv[2]);
    double epsilon = atof(argv[3]);
    long long max = atoll(argv[4]);

    double *X = malloc(n * sizeof(double));
    for (size_t i = 0; i < n; i++)
        X[i] = x0;

    rtime_t tempoAntes = timestamp();
    LIKWID_MARKER_START("Newton");
    newton(X, epsilon, n, max);
    LIKWID_MARKER_STOP("Newton");
    rtime_t tempoDepois = timestamp();

    fprintf(saidaArq, "\n#################\n");
    fprintf(saidaArq, "Tempo Total: %f\n", tempoDepois - tempoAntes);
    fprintf(saidaArq, "Tempo Jacobiana: %f\n", tempoJacobiana);
    fprintf(saidaArq, "Tempo SL: %f\n", tempoSL);
    fprintf(saidaArq, "#################\n");

    if (arquivoSaida != NULL)
        fclose(arquivoSaida);
    free(X);

    LIKWID_MARKER_CLOSE;

    return EXIT_SUCCESS;
}
