CC = gcc

CFLAGS = -Wall -Wextra -O3 -march=native -mavx -fopt-info-vec
CFLAGS2 = -Wall -Wextra

CPPFLAGS = -DLIKWID_PERFMON -I/home/soft/likwid/include
LDFLAGS = -L/home/soft/likwid/lib
LDLIBS = -llikwid -lm

TARGET = broyden

SRCS = main.c broyden.c gauss.c utils.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean debug noflags

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

noflags: CFLAGS=$(CFLAGS2)
noflags: clean $(TARGET)

debug: clean
	$(CC) -Wall -Wextra -O2 $(SRCS) -lm -o broyden

clean:
	rm -f $(OBJS) $(TARGET) *.csv *.log *.txt