TARGET = memorpg
CC     = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c plateau.c tour.c stats.c display.c player.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.dat
