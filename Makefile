#######################################
CC = gcc
# Busca todos los .c en el directorio actual
SRCS = $(wildcard *.c)
# Convierte la lista de .c en .o
OBJS = $(SRCS:.c=.o)

FLAGS = -g -Wall -O0 -Wextra -I ./headers -Wpedantic -DDEBUG 
PREFLAG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s
BD = castle.dat
TARGET = castle
#######################################

all: $(TARGET)

run: $(TARGET)
	./$(TARGET) $(BD)

# Ejecución con Valgrind
runv: $(TARGET)
	$(PREFLAG) ./$(TARGET) $(BD)

# Linkado final (usando $^ que son todas las dependencias)
$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $^ -L. -lscreen -o $@


# REGLA DE PATRÓN: Esto sustituye a todas tus reglas individuales (.o: .c)
# $< es el archivo .c y $@ es el archivo .o
%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@


	

# Empaquetado
mandar: clean
	zip -r Game_mandar_RaVi.zip ./
	clear

mandar_rm:
	rm -f Game_mandar_RaVi.zip

.PHONY: clean run runv mandar mandar_rm

clean:
	rm -f *.o *.gch $(TARGET) play Game_mandar_RaVi.zip
