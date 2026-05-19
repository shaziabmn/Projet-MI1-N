TARGET = memorpg
CC     = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c plateau.c tour.c statistiques.c affichage.c joueur.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.dat
