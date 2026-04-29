# Nom de l'exécutable
TARGET = memorpg

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -Wall -g

# Fichiers sources
SRCS = board.c display.c player.c main.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(TARGET)

# Génération de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)
