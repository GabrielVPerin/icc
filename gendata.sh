#!/bin/bash

PROG="broyden"
CPU=3

OUTPUT_DIR="resultados"
mkdir -p "${OUTPUT_DIR}"

METRICAS="L2CACHE L3 FLOPS_DP FLOPS_AVX"

TAMANHOS="32 64 128 256 512 1000 2000 4000 8000" # 9000 10000 20000"

X0="0"
EPSILON="0.0"
MAX_ITER="25"

echo "=== Compilando ==="

make clean
make all

if [ ! -f "./${PROG}" ]; then
    echo "Erro: executável não encontrado."
    exit 1
fi

for m in ${METRICAS}
do
    echo "=== ${m} ==="

    CSV="${OUTPUT_DIR}/${m}.csv"

    rm -f "${CSV}"

    echo "N,Newton,Jacobiana,Gauss" > "${CSV}"

    for n in ${TAMANHOS}
    do
        echo "   N=${n}"

        TMP=$(mktemp)

        likwid-perfctr \
            -O \
            -C ${CPU} \
            -g ${m} \
            -m \
            ./${PROG} \
            ${n} ${X0} ${EPSILON} ${MAX_ITER} \
            > "${TMP}" 2>/dev/null

        python3 genplot.py ${n} < "${TMP}" >> "${CSV}"

        rm -f "${TMP}"
    done
done

echo
echo "Arquivos gerados:"
ls -lh ${OUTPUT_DIR}