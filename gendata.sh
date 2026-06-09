#!/bin/bash

PROG="broyden"
CPU=3 # Core que o professor colocou no exercicio

# Pasta onde todos os arquivos gerados serão salvos
OUTPUT_DIR="resultados"
mkdir -p ${OUTPUT_DIR}

METRICAS="L2CACHE L3 FLOPS_DP FLOPS_AVX"

TAMANHOS="32 64 128 256 512 1000 2000 4000 8000 9000 10000 20000"

# Parâmetros fixos exigidos para os testes de desempenho
X0="1.5"
EPSILON="0.0"
MAX_ITER="25"

echo "=== Compilando o projeto ==="
make clean
make all

# Verifica se a compilação teve sucesso
if [ ! -f "./${PROG}" ]; then
    echo "Erro: O executável ./${PROG} não foi gerado. Abortando."
    exit 1
fi

echo "=== Iniciando os testes de desempenho com LIKWID ==="

for m in ${METRICAS}
do
    LIKWID_LOG="${OUTPUT_DIR}/${m}.log"
    
    # Remove logs antigos da métrica atual para não acumular lixo
    rm -f ${LIKWID_LOG}
    
    for n in $TAMANHOS
    do
        LIKWID_OUT="${OUTPUT_DIR}/${m}_${n}.txt"
        
        echo "--->> Medindo ${m} para N = ${n}"
        
        # O "echo" envia os 4 parâmetros via pipe (|) para a entrada padrão (stdin) do C.
        # O "likwid-perfctr" joga a saída formatada do LIKWID no arquivo .txt temporário.
        # A saída padrão do seu programa (os x1...xn e os tempos) é descartada (> /dev/null) 
        # para não sujar o terminal durante os testes estruturais.
	#        echo "$n $X0 $EPSILON $MAX_ITER" | likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} > /dev/null
        likwid-perfctr -O -C ${CPU} -g ${m} -o ${LIKWID_OUT} -m ./${PROG} $n $X0 $EPSILON $MAX_ITER > /dev/null
        # Junta o resultado deste N no arquivo de log geral desta métrica
        cat ${LIKWID_OUT} >> ${LIKWID_LOG}
        
        # Apaga o arquivo temporário individual
        rm -f ${LIKWID_OUT}
    done

    echo "=== Processando dados de ${m} com genplot.py ==="
    # O script Python lê o arquivo .log unificado e gera a tabela .csv limpa dentro da pasta
    python3 genplot.py $n < ${LIKWID_LOG} > "${OUTPUT_DIR}/${m}.csv"
done

echo "=== Todos os testes foram concluídos! ==="
echo "Verifique as tabelas geradas na pasta: ./${OUTPUT_DIR}/"
