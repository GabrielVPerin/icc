CC = gcc

CFLAGS = -Wall -Wextra -O3 -march=native -mavx -fopt-info-vec
CPPFLAGS = -DLIKWID_PERFMON -I/home/soft/likwid/include
LDFLAGS = -L/home/soft/likwid/lib
LDLIBS = -llikwid -lm

TARGET = broyden

SRCS = main.c broyden.c gauss.c utils.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

debug: clean
	@echo "=== Compilando versao de teste isolado (Sem LIKWID) ==="
	$(CC) -Wall -Wextra -O2 $(SRCS) -lm -o broyden_debug
	@echo "Pronto! Use: ./broyden_debug [N] [x0] [epsilon] [max]"

clean:
	rm -f $(OBJS) $(TARGET) broyden_debug *.csv *.log *.txt
