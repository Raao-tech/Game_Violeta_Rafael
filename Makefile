#######################################
FLAGS = -g -Wall -Wextra -I ./headers -pedantic 
PREFLAG = valgrind
BD = castle.dat
DEPENDS = command.o game_actions.o game.o game_loop.o game_reader.o graphic_engine.o object.o player.o space.o
CC = gcc
TARGET = castle
#######################################

run: play

play: $(TARGET)
	./$(TARGET) $(BD)

debug: $(TARGET)
	$(PREFLAG) ./$(TARGET)	$(BD)

castle: $(DEPENDS)
	$(CC) $(FLAGS) $^ -L. -lscreen -o $(TARGET)

command.o: command.c
	$(CC) -c  $(FLAGS) $< -o $@

game_actions.o: game_actions.c
	$(CC) -c  $(FLAGS) $< -o $@

game_loop.o: game_loop.c
	$(CC) -c $(FLAGS) $< -o $@

game_reader.o: game_reader.c
	$(CC) -c $(FLAGS) $< -o $@


game.o: game.c
	$(CC) -c $(FLAGS) $< -o $@

graphic_engine.o: graphic_engine.c headers/libscreen.h
	$(CC) -c $(FLAGS) $< -o $@


object.o: object.c
	$(CC) -c $(FLAGS) $< -o $@

player.o: player.c
	$(CC) -c $(FLAGS) $< -o $@

space.o: space.c
	$(CC) -c $(FLAGS) $< -o $@


.PHONY: clean play debug

clean:
	rm -f *.o $(TARGET)
