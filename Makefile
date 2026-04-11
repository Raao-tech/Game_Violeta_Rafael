#######################################
#  The Haunted Castle — Makefile I3
#  Violeta, Rafael, Salvador & Javier
#######################################

# =============== TOOLCHAIN ===============
CC       = gcc
FLAGS    = -g -Wall -O0 -Wextra -I ./headers -Wpedantic
DFLAGS   = $(FLAGS) -DDEBUG
PREFLAG  = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s

# =============== DIRECTORIES ===============
SRC_DIR  = src
HDR_DIR  = headers
OBJ_DIR  = obj
DOC_DIR  = doc

# =============== FILES ===============
SRCS     = $(wildcard $(SRC_DIR)/*.c)
OBJS     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET   = castle

# =============== DATA FILES ===============
BD       = castle.dat
LOG_FILE = output.log

# =============== LIBRARIES ===============
LIBS     = -L. -lscreen

#######################################
#            BUILD TARGETS
#######################################

all: $(OBJ_DIR) $(TARGET)

# Create obj/ directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Link all objects into the executable
$(TARGET): $(OBJS)
	$(CC) $(DFLAGS) $^ $(LIBS) -o $@

# Pattern rule: compile each .c from src/ into obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(DFLAGS) $< -o $@

#######################################
#           RUN TARGETS
#######################################

# --- Simple run (default .dat) ---
run: $(TARGET)
	./$(TARGET) $(BD)

# --- Run with Valgrind ---
runv: $(TARGET)
	$(PREFLAG) ./$(TARGET) $(BD)

# --- Run with LOG (F14) ---
#   Usage:  make run_log
#   Produces output.log with all commands and results
run_log: $(TARGET)
	./$(TARGET) $(BD) -l $(LOG_FILE)

# --- Run with LOG + Valgrind ---
runv_log: $(TARGET)
	$(PREFLAG) ./$(TARGET) $(BD) -l $(LOG_FILE)

# --- Run with custom .dat via argument ---
#   Usage:  make run_custom BD=anthill.dat
run_custom: $(TARGET)
	./$(TARGET) $(BD)

# --- Run with custom .dat + LOG ---
#   Usage:  make run_custom_log BD=anthill.dat LOG_FILE=anthill.log
run_custom_log: $(TARGET)
	./$(TARGET) $(BD) -l $(LOG_FILE)

# --- Interactive: ask the user which .dat to use ---
#   Usage:  make play
#   Shows available .dat files and lets the user pick one
play: $(TARGET)
	@echo ""
	@echo "======================================"
	@echo "   The Haunted Castle — Select Map"
	@echo "======================================"
	@echo ""
	@echo "Available data files:"
	@echo ""
	@i=1; for f in *.dat; do \
		echo "  $$i) $$f"; \
		i=$$((i + 1)); \
	done
	@echo ""
	@read -p "Enter the .dat filename (or number): " choice; \
	if echo "$$choice" | grep -qE '^[0-9]+$$'; then \
		file=$$(ls *.dat 2>/dev/null | sed -n "$${choice}p"); \
		if [ -z "$$file" ]; then \
			echo "Invalid number."; exit 1; \
		fi; \
	else \
		file="$$choice"; \
	fi; \
	if [ ! -f "$$file" ]; then \
		echo "File '$$file' not found."; exit 1; \
	fi; \
	echo ""; \
	read -p "Enable LOG? (y/n): " logyn; \
	if [ "$$logyn" = "y" ] || [ "$$logyn" = "Y" ]; then \
		read -p "Log filename [output.log]: " logname; \
		logname=$${logname:-output.log}; \
		echo "Running: ./$(TARGET) $$file -l $$logname"; \
		./$(TARGET) "$$file" -l "$$logname"; \
	else \
		echo "Running: ./$(TARGET) $$file"; \
		./$(TARGET) "$$file"; \
	fi

# --- Interactive with Valgrind ---
playv: $(TARGET)
	@echo ""
	@echo "======================================"
	@echo "   The Haunted Castle — Select Map"
	@echo "======================================"
	@echo ""
	@echo "Available data files:"
	@echo ""
	@i=1; for f in *.dat; do \
		echo "  $$i) $$f"; \
		i=$$((i + 1)); \
	done
	@echo ""
	@read -p "Enter the .dat filename (or number): " choice; \
	if echo "$$choice" | grep -qE '^[0-9]+$$'; then \
		file=$$(ls *.dat 2>/dev/null | sed -n "$${choice}p"); \
		if [ -z "$$file" ]; then \
			echo "Invalid number."; exit 1; \
		fi; \
	else \
		file="$$choice"; \
	fi; \
	if [ ! -f "$$file" ]; then \
		echo "File '$$file' not found."; exit 1; \
	fi; \
	echo ""; \
	read -p "Enable LOG? (y/n): " logyn; \
	if [ "$$logyn" = "y" ] || [ "$$logyn" = "Y" ]; then \
		read -p "Log filename [output.log]: " logname; \
		logname=$${logname:-output.log}; \
		echo "Running with Valgrind: ./$(TARGET) $$file -l $$logname"; \
		$(PREFLAG) ./$(TARGET) "$$file" -l "$$logname"; \
	else \
		echo "Running with Valgrind: ./$(TARGET) $$file"; \
		$(PREFLAG) ./$(TARGET) "$$file"; \
	fi

#######################################
#         INTEGRATION TESTS (P2)
#######################################

# Run a .cmd file as input and produce a LOG
#   Usage:  make test_cmd CMD=game1.cmd
CMD      = game1.cmd
test_cmd: $(TARGET)
	@if [ ! -f "$(CMD)" ]; then \
		echo "Error: file '$(CMD)' not found."; exit 1; \
	fi
	./$(TARGET) $(BD) -l $(LOG_FILE) < $(CMD)
	@echo ""
	@echo "LOG written to $(LOG_FILE):"
	@cat $(LOG_FILE)

#######################################
#          DOCUMENTATION (C3)
#######################################

# Generate Doxygen documentation
#   Requires: doxygen installed, Doxyfile in project root
doc:
	@mkdir -p $(DOC_DIR)
	@if [ -f Doxyfile ]; then \
		doxygen Doxyfile; \
		echo "Documentation generated in $(DOC_DIR)/"; \
	else \
		echo "No Doxyfile found. Create one with: doxygen -g"; \
	fi

#######################################
#             UTILITIES
#######################################

# Ingit assistant 
#She is your greatest friend (enemy if you provoke her),she is supposed  
#to be our ally, but if misused she will be our worst nightmare
Ingit: Ingit.sh
	./Ingit.sh

# Package for submission
mandar: clean
	zip -r Game_mandar_RaVi.zip ./
	@echo "Packaged into Game_mandar_RaVi.zip"

mandar_rm:
	rm -f Game_mandar_RaVi.zip

# Remove log files
rm_log:
	rm -f *.log

#######################################
#             CLEANUP
#######################################

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET) *.gch *.log
	rm -f $(SRC_DIR)/*.o
	rm -f Game_mandar_RaVi.zip

clean_all: clean
	rm -rf $(OBJ_DIR) $(DOC_DIR)

.PHONY: all run runv run_log runv_log run_custom run_custom_log \
        play playv test_cmd doc Ingit mandar mandar_rm rm_log \
        clean clean_all