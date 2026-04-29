#######################################
#  Atlantic Quest — Makefile I4
#  Violeta, Rafael, Salvador & Javier
#######################################

#Hola!, Esto es una pequeña explicación para que todos los colaboradores puedan entender la estructura del makefile
# Todo lo que está presente en el makefile, cumple con un estandar dado por GNU Make / GCC 
# Pueden echarle un vistazo al manual en https://www.gnu.org/software/make/manual/make.html
#
#
#	Antes de empezar, tengan presente el siguiente esquema durante toda la lectura:
#	Proceso de compilación (¿Qué ocurre en el momento en que gcc se activa?):
#
#			(PREPROCESADO)			 (PROCESP/COMPILACIÓN  Y ENSAMBLADO (*.o))							(LINKEADO)		
#		headers (*.h) + files (*.c) --->  Todo .c es compilado y ensamblado en su respectivo .o -->  *.o se fusiona en un Ejecutable == ./atlantic_quest
#
# Estructura:
#	TOOLCHAIN: Todas las flags que usamos en nuestra compilación
#		CC			--> Compilador 
#			Acá definimos si usamos gcc / cc o cualquier otro compilador, como estamos en linux y Windows es un virus (Se respetan todas las erradas opiniones posibles)
#			no hablamos de MinGW o MSYS2, pero se podría crear una versión de este Makefile con el fin de integrar más OS.
#		CPPFLAGS	--> Preprocesado (flags)
#			Según el manual de GNU Make/gcc, esta constante es el hogar de toda flag relacioanda con el preprocesado del compilador C,
#			para asegurarme de estar hablando el mismo idioma, les recuerdo que el preprocesado es esa parte de la compilación, en el que 
#			los #includes "..." o #include <...> son fusionados en un sólo archivo, en otras palabras, todas las cabeceras (*.h) 
#			se van "INCLUYENDO o definiendo" en su respectivo .c, luego los compila y crea un .o por cada .c compilado.
#		CFLAGS		--> Procesado (flags)
#			Ya con nuestros .h incluídos en sus .c, el compilador gcc/cc comenzará a leer, el objetico es ir crear los Objects (.o) de cada .c,
#			esto implica una traducción del código en C pasado a otro lenguaje llamado Assembler, y éste a su vez pasa directo al binario.
#			Dado el trabajo que tiene, es más razonable buscar errores de código antes de hacer la traducción, y ahí es donden entran nuestras flags,
#			permitiendo  especificar que tan quisquilloso debe de ser, las flags deben de estar orientadas a la depuración, optimizacion, Alertas de 
#			de la sintaxis y posibles errores, más no la inclusion de archivos, pues eso ya está en la anterior constante :
#			|			-Wall (Warn All) : nos chiva Errores y odiosos Warnings, sin embargo no los activa todos (no confién tanto)
#			|			-Wextra (Warn Extras) : Es una ayuda para Wall, se encarga de advertir cosas como comparaciones entre datos 
#			|				con y sin signo o parámetros en funciones que no están siendo usadas
#			|			-Wpedantic : Fuerza al compilador a seguir el estándar de C o C++ de la ISO, evita que se use "extensiones" 
#			|				o artilugios que normalmente aceptaria GCC pero tal vez otro compilador no, lo que nos obliga a escribir un código portátil
#			|			-O0 (Optimización 0) : Esta flag asegura que el compilador no modificará tu código para optimizarlo, sino que permitirá  
#			|			apreciar vuestras "metidas de pata" en tiempo real mientras que se ejecuta cualquier programa de debugueado.  
#			|			-g (Debug) : Es la flag que se benefica de la nula optimización, está le dice al compilador gcc que deje "comentarios" en el archivo .o para 
#			|			que herramientas como GDB o Valgrind puedan identificar la línea exacta en donde esta la razón de vuestras pesadillas.
#			|____________________________________________________________________________________________________________________________________________________________>>>>
#		LDFLAGS		--> Opciones del Linkeado  (flags)
#			Se supone, según el manual, que es en ésta constante de makefile que le decimos al compilador cómo debería "linkear" todos los .o del proyecto,
#			en un contexto normal, acá le diríamos algo del estilo: "Vete a tal directorio y espera a que te diga que lirebrias linkearas con estos .o",
#			para ello escribiríamos algo como -L./directorio. Bien, la razón por la cual esta vacío, es porque no quería confiar en que cada sistema buscaría
#			la librería estática correcta, por lo que decidí que LDLIBS tenga cada una de las rutas de nustras librerías (libscreen.a, libraylib.a, etc)	
#			por lo que LDFLAGS queda vacío, y esto es así sólo por respetar el estándar, pero podríamos quitarlo y todo compilaría tan fácil como lo hace el agua 
#			agua por un río.  	
#		PREFLAGS		--> Opciones debugueado (flags)
#			En esta constante se llama a la herramienta que nos permitirá debuguear todo nuestro programa, en este caso es el fmaoso Valgrind, dejo un pequeño
#			resumen de que hace cada flag de Valgrind:
#			|		--leak-check=full: Le pide a Valgrind que te dé el detalle completo de cada bloque de memoria que no liberaste (usando free), diciéndote dónde se reservó originalmente.
#			|		--show-leak-kinds=all: Muestra todos los tipos de fugas: las que "definitivamente se perdieron", las que son "indirectamente inalcanzables" y las que "posiblemente se perdieron". No deja nada oculto.
#			|		--track-origins=yes: Esta es vital. Si usas una variable sin haberle dado un valor inicial (memoria no inicializada), Valgrind te dirá exactamente dónde se creó esa variable. Es perfecta para rastrear errores de tipo "Conditional jump depends on uninitialised value".
#			|		-s (Summary): Al final de la ejecución, muestra un resumen detallado de todos los errores y listas de fugas encontrados.
#			|_______________________________________________________________________________________________________________________________________________________________>>>>>>>>
#			Esta constante es usada para debugueado, así que, si la queires usar, activa make runv, playv o runv_log.
#
#	DIRECTORIES: Acá tenemos definido el nombre de todos los directorios que usamos en este proyecto.
#		SRC_DIR  = src  		--> ubicación de todos nuestros .*c
#		HDR_DIR  = headers  	--> ubicación de todos nuestros .*h
#		OBJ_DIR  = obj  		--> ubicación de todos nuestros .*o
#		LIB_DIR  = lib			--> ubicación de nuestras librerías estáticas *.a
#		DOC_DIR  = doc			--> ubicación de lo que sea que haga Doxygien (cosas raras)
#		LOG_DIR  = otros		--> ubicación de documentos no relacionados con  el funcionamiento del juego
#
#	FILES: Acá tenemos el nombre de los archivos con ciertas reglas que ya explicaré: 
#
#		SRCS     = $(wildcard $(SRC_DIR)/*.c)
#					Esta constante lo que tendrá será todos los nombres de los .c en el directorio src/
#
#		OBJS     = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS)) 
#					Esta constante lo que tiene es que relaciona el nombre  de todos los .c (%.c) que haya en el directorio src/
#					luego, los guardan sustituyendo el ".c" por un ".o". La estructura de la función usada es:
#					patsubst ( patrón, Nuevo patrón, los archvios donde buscara el patron)
#
#		TARGET   = atlantic_quest
#					Nombre del ejecutable
#
#		LOG_FILE ?= $(LOG_DIR)/output.log
#					Dirección del archivo de logs por defecto. Lo puedes probar ejecutando make run_log runv_log playv play test_cmd etc 
#					
#	DATA FILES: Acá tenemos los archivos donde se guardará y/o leerá información necesaria para el jeugo (GameLoad, *.dat, .cmd. etc)
#		BD       --> Es el archivo que leerá game_reader.c, aquí estará definido todo lo relacioando con los espacios, players, objects, numens, etc
#		CMD		 --> Acá estarán los comandos que se usarán en las pruebas automatizadas de integración.
#
#
#==============================================| RayLib es una librería usada en la creación  de videojuegos, usa otras librerías que le permiten gestionar
#											   | gráficos (OpenGL), inputs (X11), texturas, texto, sombras, audio, etc.  Dada su versatilidad, se requeire tener 
#											   | ciertas dependencias para que raylib nos sirva. Les invito a chequear su arquitectura, la convención que usa y en general, 
#	RECURSOS PARA EL CORRECTO USO DE RAYLIB	   | trastear antes de codear:  
#											   | Arquitectura: https://github.com/raysan5/raylib/wiki/raylib-architecture
#											   | Web Page: https://github.com/raysan5
#==============================================|
# LOCAL STATIC LIBRARIES
#	LIBSCREEN_A  >>>> Guarda la libería que hemos usado durante las anteriores iteraciones
#	RAYLIB_A	 >>>> Guardar la ubicación de la librería estatica (.a) que usaremos para la definición de raylib
#
# POSIBLES DEPENDECIAS PARA LINUX
#	No siempre las dependencias que usa raylib están en nuestros computadores, lo cual es raro pues son dependencias
#	muy usadas en la industria (OpenGL, X11, etc), pero puede ocurrir que aunque estén instaldas, se tenga discrepancia en el 
#	nombre, este es el problema con el que me tope en los ordenadores de la uni;  al parecer el nombre de ciertas librerías dinámicas,
#	(identificadas por su temrinación ".so") tienen incluído por alguna razón que desconozco un "*.so.1" o .2 o algo que no coincide con
#	el patrón exacto que busca el compialdor cuando se le manda a linkear con -name_of_genericLib, devolviendo error.  Para evitar esos problemas
#	he definido la ruta exacta en la que se encuentra cada librería, veamos cuales son para que son usadas por raylib: 
#
#		GL_LIB     ̣---> librería de OpenGL: Es una librería o API de renderizado gráfico 2D y 3D, muchos juegos como Minecraft (versión java) u Angry Birds (para Pc s) lo usarón
#		X11_LIB    ---> librería X11: Es la librería que usa Ubuntu para gestionar la creación de su interfaz gráfica.  (abrir ventanas, detectar clicks del ratón o teclado, lo irémos viendo en raylib)
#		DL_LIB     ---> librería Dynamic Linker Library: es un a lirebria que permite cargar librerias dinamicas (.so) en tiempo de ejecución, se suelen usar en proyectos grandes como este para reducir el tiempo de compilación
#		RT_LIB     ---> librería Real-time Extensions: basicamente nos trae una precisiónm absurda del tiempo, lo que nos permite hacer animaciones fluidas a 60 fps
#		M_LIB      ---> librería Math Library: Esta librería la hemos usado con anterioridad, trae funciones matemáticas para poder calcular potencias, raíces cuadradas, y otros artilugios
#						matemáticos que podremos usar para calcular el moódulo del vector "distancia enemigo", área de efecto de las habildiades, etc. 
#
#
#		                    --------------------------------------------------------------
#		
#		LDLIBS ------> ÉSTE SERÁ LA CONSTANTE USADA EN ORDENADORES DE LA UNI PARA COMPILAR NUESTRO CÓDIGO
#		
#		RAYLIB_SYS_LIBS  Esta constante guarda todas las librerías, pero hay una flag especial que usa, a la cual me gustaría dedicarle un pequeño inciso:
#				-pthread   ---> Esta flag le dice al compilador que pueda ejecutar múltiples cosas en paralelo.  El audio, debe sonar mientras que la imágen se mueve.
#				
#		                    --------------------------------------------------------------
#
#		
## =============== TOOLCHAIN ===============
# Compilador
CC = gcc
# Preprocesado: include path, define DEBUG, dependencias automaticas
CPPFLAGS = -I ./headers -DDEBUG -MMD -MP
# Procesado: warnings, debug, sin optimizacion, C89 estricto (exigido por I4)
CFLAGS = -g -Wall -Wextra -Wpedantic -ansi -O0
# Linkeado: vacio (ver LDLIBS)
LDFLAGS =
# Valgrind: para targets *_v y debug de fugas de memoria
PREFLAG = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s




# =============== DIRECTORIES ===============
SRC_DIR = src
HDR_DIR = headers
OBJ_DIR = obj
LIB_DIR = lib
DOC_DIR = doc
LOG_DIR = otros


# =============== FILES ===============
# Compilamos todo lo que tenga .c en SRC_DIR
SRCS = $(wildcard $(SRC_DIR)/*.c)
# patsubst (patron, nuevo patron, archivos): src/x.c -> obj/x.o
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
# Nombre del ejecutable principal
TARGET = atlantic_quest
# Log por defecto. Sobreescribible: make run_log LOG_FILE=otra.log
LOG_FILE ?= $(LOG_DIR)/output.log

# =============== DATA FILES ===============
BD  ?= atlantic_quest.dat
CMD ?= game1.cmd


# =============== LOCAL STATIC LIBRARIES ===============
LIBSCREEN_A = $(LIB_DIR)/libscreen.a
RAYLIB_A    = $(LIB_DIR)/libraylib.a


# =============== POSIBLES DEPENDENCIAS PARA LINUX ===============
GL_LIB  = /usr/lib64/libGL.so.1
X11_LIB = /usr/lib64/libX11.so
DL_LIB  = /usr/lib64/libdl.so.2
RT_LIB  = /usr/lib64/librt.so.1
M_LIB   = /usr/lib64/libm.so

RAYLIB_SYS_LIBS = $(GL_LIB) $(X11_LIB) $(DL_LIB) $(RT_LIB) $(M_LIB) -pthread

# Linkeado: librerias estaticas locales + dependencias del sistema
LDLIBS = $(LIBSCREEN_A) $(RAYLIB_A) $(RAYLIB_SYS_LIBS)


# =============== TARGET POR DEFECTO ===============
# Si haces 'make' a secas, ejecuta 'all' (no las pruebas aisladas).
.DEFAULT_GOAL := all


#######################################
#            BUILD TARGETS
#######################################

all: $(TARGET)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LOG_DIR):
	@mkdir -p $(LOG_DIR)

$(TARGET): $(OBJ_DIR) $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


#######################################
#       PRUEBAS AISLADAS DE RAYLIB
#######################################
# Estos targets compilan window_prueba.c por separado para verificar
# que la cadena de raylib funciona en tu maquina. NO forman parte
# del proyecto principal y se invocan explicitamente.

prueba_local: window_prueba.c
	$(CC) window_prueba.c $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o prueba

prueba_sys: window_prueba.c
	$(CC) window_prueba.c -o prueba_sys -lraylib -lGL -lm -lpthread -ldl -lrt -lX11


#######################################
#           RUN TARGETS
#######################################

run: $(TARGET)
	./$(TARGET) $(BD)

runv: $(TARGET)
	$(PREFLAG) ./$(TARGET) $(BD)

run_log: $(TARGET) $(LOG_DIR)
	./$(TARGET) $(BD) -l $(LOG_FILE)

runv_log: $(TARGET) $(LOG_DIR)
	$(PREFLAG) ./$(TARGET) $(BD) -l $(LOG_FILE)

run_custom: $(TARGET)
	./$(TARGET) $(BD)

run_custom_log: $(TARGET) $(LOG_DIR)
	./$(TARGET) $(BD) -l $(LOG_FILE)

play: $(TARGET) $(LOG_DIR)
	@echo ""
	@echo "======================================"
	@echo "   Atlantic Quest — Select Map"
	@echo "======================================"
	@echo ""
	@echo "Available data files:"
	@echo ""

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
		echo "Running: ./$(TARGET) $$file -l $(LOG_DIR)/$$logname"; \
		./$(TARGET) "$$file" -l "$(LOG_DIR)/$$logname"; \
	else \
		echo "Running: ./$(TARGET) $$file"; \
		./$(TARGET) "$$file"; \
	fi

playv: $(TARGET) $(LOG_DIR)
	@echo ""
	@echo "======================================"
	@echo "   Atlantic Quest — Select Map (Valgrind)"
	@echo "======================================"
	@echo ""
	@echo "Available data files:"
	@echo ""
	
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
		echo "Running with Valgrind: ./$(TARGET) $$file -l $(LOG_DIR)/$$logname"; \
		$(PREFLAG) ./$(TARGET) "$$file" -l "$(LOG_DIR)/$$logname"; \
	else \
		echo "Running with Valgrind: ./$(TARGET) $$file"; \
		$(PREFLAG) ./$(TARGET) "$$file"; \
	fi


#######################################
#         INTEGRATION TESTS (P3)
#######################################

test_cmd: $(TARGET) $(LOG_DIR)
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

Tests: automatic_test.sh
	./automatic_test.sh

mandar: clean
	zip -r Game_mandar_RaVi.zip ./
	@echo "Packaged into Game_mandar_RaVi.zip"

mandar_rm:
	rm -f Game_mandar_RaVi.zip

rm_log:
	rm -f $(LOG_DIR)/*.log *.log

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(TARGET) *.gch *.log
	rm -f $(SRC_DIR)/*.o
	rm -f Game_mandar_RaVi.zip
	rm -f prueba prueba_sys

clean_all: clean
	rm -rf $(OBJ_DIR) $(DOC_DIR) tests/*.o tests/*_test


.PHONY: all run runv run_log runv_log run_custom run_custom_log \
        play playv test_cmd doc Ingit Tests mandar mandar_rm rm_log \
        clean clean_all prueba_local prueba_sys

-include $(OBJS:.o=.d)