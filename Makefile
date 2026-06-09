CC = gcc
CFLAGS = -Wall -Wextra -O3 -march=native -mavx -fopt-info-vec -DLIKWID_PERFMON
LIBS = -lm -llikwid
TARGET = broyden
SRCS = main.c broyden.c gauss.c utils.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
