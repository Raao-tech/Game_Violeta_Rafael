#######################################
# COMPILER AND FLAGS
#######################################
CC = gcc
# -I../headers tells the compiler where to find .h files
FLAGS = -g -Wall -O0 -Wextra -I../headers -Wpedantic -DDEBUG
# Valgrind configuration
PREFLAG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s

#######################################
# TARGETS (Executables)
#######################################
SET_BIN = test_set
ENT_BIN = test_entity

# Default rule
all: $(SET_BIN) $(ENT_BIN)

#######################################
# COMPILATION RULES
#######################################

# Rule for Entity Test: Needs test_entity.o + entity.o + set.o
# (Using character.c/entity.c from parent directory)
$(ENT_BIN): $(ENT_BIN).o ../entity.o ../set.o
	$(CC) $(FLAGS) $^ -o $@

# Rule for Set Test: Needs test_set.o + set.o
$(SET_BIN): $(SET_BIN).o ../set.o
	$(CC) $(FLAGS) $^ -o $@

# Pattern rule for local test objects
%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@

# Pattern rule for parent directory objects
../%.o: ../%.c
	$(CC) -c $(FLAGS) $< -o $@

#######################################
# EXECUTION SHORTCUTS (using $< for the first dependency)
#######################################

# Run Entity Test with Valgrind
vent: $(ENT_BIN)
	@read -p "Show ENTITY data on terminal? (s/n): " answ; \
	if [ "$$answ" = "n" ]; then \
		mkdir -p Outputs; \
		$(PREFLAG) ./$< > Outputs/out_entity.txt 2>&1; \
		echo "Results saved in tests/Outputs/out_entity.txt"; \
	else \
		$(PREFLAG) ./$<; \
	fi

# Run Set Test with Valgrind
vset: $(SET_BIN)
	@read -p "Show SET data on terminal? (s/n): " answ; \
	if [ "$$answ" = "n" ]; then \
		mkdir -p Outputs; \
		$(PREFLAG) ./$< > Outputs/out_set.txt 2>&1; \
		echo "Results saved in tests/Outputs/out_set.txt"; \
	else \
		$(PREFLAG) ./$<; \
	fi

#######################################
# CLEANUP
#######################################
clean:
	@rm -f *.o ../*.o $(SET_BIN) $(ENT_BIN)
	@rm -rf Outputs

.PHONY: clean all vent vset
