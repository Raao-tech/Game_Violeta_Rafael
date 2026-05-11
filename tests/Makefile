# COMPILER AND FLAGS
CC = gcc
FLAGS = -g -Wall -O0 -Wextra -I../headers -Wpedantic -DDEBUG
PREFLAG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s

# TARGETS
SET_BIN = set_test
ENT_BIN = entity_test
SPC_BIN = space_test
PLY_BIN = player_test
CHR_BIN = character_test
GAM_BIN = game_test
CMD_BIN = command_test
LNK_BIN = links_test
OBJ_BIN = object_test
ROJO=\033[1;35m
RESET=\033[0m

all: $(SET_BIN) $(ENT_BIN) $(SPC_BIN) $(PLY_BIN) $(CHR_BIN) $(GAM_BIN) $(CMD_BIN) $(LNK_BIN) $(OBJ_BIN)

# COMPILATION RULES

$(SET_BIN): set_test.o ../set.o
	$(CC) $(FLAGS) $^ -o $@

$(ENT_BIN): entity_test.o ../entity.o ../set.o
	$(CC) $(FLAGS) $^ -o $@

$(SPC_BIN): space_test.o ../space.o ../set.o
	$(CC) $(FLAGS) $^ -o $@

$(PLY_BIN): player_test.o ../player.o ../entity.o ../inventory.o ../set.o
	$(CC) $(FLAGS) $^ -o $@

$(CHR_BIN): character_test.o ../character.o ../entity.o ../set.o
	$(CC) $(FLAGS) $^ -o $@

$(GAM_BIN): game_test.o ../game.o ../player.o ../character.o ../object.o ../space.o ../entity.o ../inventory.o ../set.o ../command.o ../links.o ../numen.o ../types.o ../skills.o
	$(CC) $(FLAGS) $^ -o $@

$(CMD_BIN): command_test.o ../command.o
	$(CC) $(FLAGS) $^ -o $@

$(LNK_BIN): links_test.o ../links.o
	$(CC) $(FLAGS) $^ -o $@

$(OBJ_BIN): object_test.o ../object.o ../entity.o ../set.o
	$(CC) $(FLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@

../%.o: ../src/%.c
	$(CC) -c $(FLAGS) $< -o $@

# RUN WITH VALGRIND
vset: $(SET_BIN)
	$(PREFLAG) ./$(SET_BIN)
ventity: $(ENT_BIN)
	$(PREFLAG) ./$(ENT_BIN)
vspace: $(SPC_BIN)
	$(PREFLAG) ./$(SPC_BIN)
vplayer: $(PLY_BIN)
	$(PREFLAG) ./$(PLY_BIN)
vcharacter: $(CHR_BIN)
	$(PREFLAG) ./$(CHR_BIN)
vgame: $(GAM_BIN)
	$(PREFLAG) ./$(GAM_BIN)
vcommand: $(CMD_BIN)
	$(PREFLAG) ./$(CMD_BIN)
vlinks: $(LNK_BIN)
	$(PREFLAG) ./$(LNK_BIN)
vobject: $(OBJ_BIN)
	$(PREFLAG) ./$(OBJ_BIN)



# RUN SINGLE TEST
run_set: $(SET_BIN)
	./$(SET_BIN) $(T)
run_entity: $(ENT_BIN)
	./$(ENT_BIN) $(T)
run_space: $(SPC_BIN)
	./$(SPC_BIN) $(T)
run_player: $(PLY_BIN)
	./$(PLY_BIN) $(T)
run_character: $(CHR_BIN)
	./$(CHR_BIN) $(T)
run_game: $(GAM_BIN)
	./$(GAM_BIN) $(T)
run_command: $(CMD_BIN)
	./$(CMD_BIN) $(T)
run_links: $(LNK_BIN)
	./$(LNK_BIN) $(T)
run_object: $(OBJ_BIN)
	./$(OBJ_BIN) $(T)


run_all: $(SET_BIN) $(ENT_BIN) $(SPC_BIN) $(PLY_BIN) $(CHR_BIN) $(GAM_BIN) $(CMD_BIN) $(LNK_BIN) $(OBJ_BIN)
	@echo -e "$(ROJO)$(SET_BIN)$(RESET)\n" && ./$(SET_BIN) $(T)
	@echo -e "$(ROJO)$(ENT_BIN)$(RESET)\n" && ./$(ENT_BIN) $(T)
	@echo -e "$(ROJO)$(SPC_BIN)$(RESET)\n" && ./$(SPC_BIN) $(T)
	@echo -e "$(ROJO)$(PLY_BIN)$(RESET)\n" && ./$(PLY_BIN) $(T)
	@echo -e "$(ROJO)$(CHR_BIN)$(RESET)\n" && ./$(CHR_BIN) $(T)
	@echo -e "$(ROJO)$(GAM_BIN)$(RESET)\n" && ./$(GAM_BIN) $(T)
	@echo -e "$(ROJO)$(LNK_BIN)$(RESET)\n" && ./$(LNK_BIN) $(T)
	@echo -e "$(ROJO)$(OBJ_BIN)$(RESET)\n" && ./$(OBJ_BIN) $(T)

runv_all: $(SET_BIN) $(ENT_BIN) $(SPC_BIN) $(PLY_BIN) $(CHR_BIN) $(GAM_BIN) $(CMD_BIN) $(LNK_BIN) $(OBJ_BIN)
	@echo -e "$(ROJO)$(SET_BIN)$(RESET)\n" && $(PREFLAG) ./$(SET_BIN)
	@echo -e "$(ROJO)$(ENT_BIN)$(RESET)\n" && $(PREFLAG) ./$(ENT_BIN)
	@echo -e "$(ROJO)$(SPC_BIN)$(RESET)\n" && $(PREFLAG) ./$(SPC_BIN)
	@echo -e "$(ROJO)$(PLY_BIN)$(RESET)\n" && $(PREFLAG) ./$(PLY_BIN)
	@echo -e "$(ROJO)$(CHR_BIN)$(RESET)\n" && $(PREFLAG) ./$(CHR_BIN)
	@echo -e "$(ROJO)$(GAM_BIN)$(RESET)\n" && $(PREFLAG) ./$(GAM_BIN)
	@echo -e "$(ROJO)$(LNK_BIN)$(RESET)\n" && $(PREFLAG) ./$(LNK_BIN)
	@echo -e "$(ROJO)$(OBJ_BIN)$(RESET)\n" && $(PREFLAG) ./$(OBJ_BIN)

	
# CLEANUP
clean:
	@rm -f *.o ../*.o $(SET_BIN) $(ENT_BIN) $(SPC_BIN) $(PLY_BIN) $(CHR_BIN) $(GAM_BIN) $(CMD_BIN) $(LNK_BIN) $(OBJ_BIN)
	cd ..
	rm *.o
.PHONY: clean all vset vent vspc vply vchr vgam vcmd vlnk vobj
