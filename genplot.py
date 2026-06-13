#!/usr/bin/env python3

from math import *
import re
import sys

campos = {
    "L2CACHE": r"L2 miss ratio",
    "L3": r"L3 bandwidth",
    "FLOPS_DP": r"DP MFLOP/s",
    "FLOPS_AVX": r"Packed DP MFLOP/s"
}

def lerDados():
    N_atual = int(sys.argv[1])

    linha = sys.stdin.readline()
    while linha and re.match("STRUCT,Info", linha) is None:
        linha = sys.stdin.readline()

    for linha in sys.stdin:
        if re.match("TABLE,Region.*Metric,", linha):
            break

    if linha:
        linha = linha.split(',')

        metrica = linha[3].strip()

        regiao = linha[1].split(' ')
        marker = regiao[1].strip()

        ordem = N_atual

        for linha in sys.stdin:
            if re.match(campos[metrica], linha):
                break

        if linha:
            linha = linha.split(',')
            valor = float(linha[1])

            return [metrica, ordem, marker, valor]

    return None


saida = {}

item = lerDados()

while item:
    metrica, n, marker, valor = item

    if metrica not in saida:
        saida[metrica] = {}

    if n not in saida[metrica]:
        saida[metrica][n] = []

    saida[metrica][n].append([marker, valor])

    item = lerDados()

for metrica in saida:
    for n in sorted(saida[metrica]):
        linha = [str(n)]

        for marker, valor in saida[metrica][n]:
            linha.append(str(valor))

        print(",".join(linha))
