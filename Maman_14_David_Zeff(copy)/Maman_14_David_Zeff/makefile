CC = gcc
CFLAGS = -ansi -Wall -pedantic
TARGET = run
SRCS = main.c macroParser.c firstRun.c
OBJS = $(SRCS:.c=.o)
DEPS = main.h macroParser.h firstRun.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

