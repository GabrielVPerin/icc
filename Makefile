CC = gcc

CFLAGS = -Wall -Wextra -O3 -march=native -mavx -fopt-info-vec

# Mantemos o diretório de include globalmente para o compilador não reclamar do 
# #include <likwid.h> nos arquivos fonte, mesmo quando não for calcular a performance.
CPPFLAGS = -I/home/soft/likwid/include
# Por padrão, linkamos apenas a biblioteca matemática.
LDLIBS = -lm

TARGET = broyden

SRCS = main.c broyden.c gauss.c utils.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean likwid

# "make" padrão compila com otimizações (-O3, -mavx, etc), mas SEM O LIKWID.
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# "make likwid" injeta as macros e caminhos de biblioteca do likwid 
# e força a recompilação limpa.
likwid: CPPFLAGS += -DLIKWID_PERFMON
likwid: LDFLAGS += -L/home/soft/likwid/lib
likwid: LDLIBS += -llikwid
likwid: clean $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.csv *.log *.txt
