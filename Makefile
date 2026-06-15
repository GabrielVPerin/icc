CC = gcc

CFLAGS = -Wall -Wextra -O3 -march=native -mavx -fopt-info-vec

CPPFLAGS = -I/home/soft/likwid/include
LDLIBS = -lm

TARGET = broyden

SRCS = main.c broyden.c gauss.c utils.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean likwid

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

likwid: CPPFLAGS += -DLIKWID_PERFMON
likwid: LDFLAGS += -L/home/soft/likwid/lib
likwid: LDLIBS += -llikwid
likwid: clean $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.csv *.log *.txt
