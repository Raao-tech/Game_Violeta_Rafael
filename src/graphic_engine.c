#include <stdlib.h>
#include "graphic_engine.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h" /*Esto es una extension de raylib para hacer la interfaz grafica*/

/* ------------------------------------------------------------------ */
/*  Tipos privados                                                      */
/* ------------------------------------------------------------------ */

typedef struct {
  int width;
  int height;
} Collider;            

typedef struct _Entity_graphic {
  Vector2  position;
  Collider collider;              
  char    *img;
} Entity_ge;

struct _Graphic_engine {
  Entity_ge  player;
  Entity_ge *obj;
  Entity_ge *spc;
  Entity_ge *numens;
};

/* ------------------------------------------------------------------ */
/*  Funciones privadas                                                  */
/* ------------------------------------------------------------------ */

static Entity_ge *entity_ge_create(void) {
  Entity_ge *e = (Entity_ge *) malloc(sizeof(Entity_ge)); 
  if (!e) return NULL;

  e->img          = NULL;
  e->position.x   = -1.0f;        /* posición inválida inicial */
  e->position.y   = -1.0f;
  e->collider.width  = 0;
  e->collider.height = 0;
  return e;
}

static void entity_ge_destroy(Entity_ge *entity) {
  if (!entity) return;
  if (entity->img) free(entity->img);
  free(entity);
}

/* ------------------------------------------------------------------ */
/*  API pública                                                         */
/* ------------------------------------------------------------------ */

Graphic_engine *graphic_engine_create(void) {
  Graphic_engine *gp = (Graphic_engine *) malloc(sizeof(Graphic_engine)); /* ← sizeof(struct), NO sizeof(puntero) */
  if (!gp) return NULL;


  gp->player.img          = NULL;
  gp->player.position.x   = -1.0f;
  gp->player.position.y   = -1.0f;
  gp->player.collider.width  = 0;
  gp->player.collider.height = 0;


  gp->spc = entity_ge_create();
  if (!gp->spc) {
    free(gp);
    return NULL;
  }

  gp->obj = entity_ge_create();
  if (!gp->obj) {
    entity_ge_destroy(gp->spc); 
    free(gp);
    return NULL;
  }

  gp->numens = entity_ge_create();
  if (!gp->numens) {
    entity_ge_destroy(gp->obj);
    entity_ge_destroy(gp->spc);
    free(gp);
    return NULL;
  }

  return gp; 
}

void graphic_engine_destroy(Graphic_engine *ge) {
  if (!ge) return;
  entity_ge_destroy(ge->numens);
  entity_ge_destroy(ge->obj);
  entity_ge_destroy(ge->spc);
  if (ge->player.img) free(ge->player.img);
  free(ge);
}


MenuResult graphic_engine_init(Graphic_engine *ge)
{
  MenuResult result;
  Bool       exit_on   = FALSE;
  char*      name_game = NULL;

  /*==== Parte de pruebas ====*/
  int   currentMenu = 0;
  int   w_boton     = 250;
  int   h_boton     = 50;
  int   intra_space = 10;
  /*==== ================ ====*/

  /*===== Inicializamos el result ====*/
  result.data_name[0] = '\0';
  result.init_numen = NO_ID;
  result.menu_out =  OUT_ERR;
  /*===== ======================= ====*/

  if(!ge) return result;

  /*Muestro el menú con todas las opciones posibles {New Game, Load Game, Exit}*/
  InitWindow(WIDHT_SCREEN, HIGHT_SCREEN, TITLE);
  SetTargetFPS(FPS);

  while (!WindowShouldClose() && exit_on == FALSE)
  {
    BeginDrawing();
		ClearBackground(RAYWHITE);

		if (result.menu_out == OUT_ERR)							/* Menú Principal (OUT_ERR) */
		{
			DrawText("ATLANTIC QUEST", (int)(WIDHT_SCREEN*(3.0/8.0)), 100, 20, DARKGRAY);
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(2.0/8.0)), 150, 200, 50 }, "NEW GAME"))  result.menu_out = NEW_GAME;
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(2.0/8.0)), 220, 200, 50 }, "LOAD GAME")) result.menu_out = LOAD_GAME;
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(2.0/8.0)), 290, 200, 50 }, "Exit"))      exit_on = TRUE;

		}
		else if(result.menu_out == NEW_GAME)					/* Menú selección de Pokemons (NEW_GAME) */
		{
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(6.0/8.0)), 10, 100, 30 }, "BACK"))		result.menu_out = NEW_GAME;
			DrawText("Choose your Initial Numen", (int)(WIDHT_SCREEN*(3.0/8.0)), 100, 20, DARKGRAY);
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(2.0/8.0)), 150, 200, 50 }, "BULBASAUR"))
			{
				strncpy(result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen = First_numen;
				exit_on = TRUE;
			}
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(2.0/8.0)), 220, 200, 50 }, "SQUIRTLE"))
			{
				strncpy(result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen = Second_numen;
				exit_on = TRUE;
			}
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(2.0/8.0)), 220, 200, 50 }, "CHARMANDER"))
			{
				strncpy(result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen = third_numen;
				exit_on = TRUE;
			}
		}
		else if (result.menu_out == LOAD_GAME)					/* Menú selección de Partidas (LOAD_GAME) */
		{
			DrawText("WHITCH YOUR GAME?", 300, 100, 20, DARKGRAY);
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(6.0/8.0)), 10, 100, 30 }, "BACK"))       result.menu_out = OUT_ERR;
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(2.0/8.0)), 150, 200, 50 }, "LOAD GAME #1"))  {
				strncpy(result.data_name, F_DATA_S_1, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen = NO_ID;
				exit_on = TRUE;
			}
			if (GuiButton((Rectangle){ (int)(WIDHT_SCREEN*(2.0/8.0)), 220, 200, 50 }, "LOAD GAME #2"))  {
				strncpy(name_game, F_DATA_S_2, WORD_SIZE);
				name_game[WORD_SIZE] ='\0';
				result.init_numen = NO_ID;
				exit_on = TRUE;
			}
		}
    EndDrawing();
  }
  
  CloseWindow();

  return result;  /*Si ha salido por medio el New_game reuslt contendrá en su out  NEW_GAME,   si ha salido mediante EXIT  result-out_menu = ERR_OUT*/
}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game) {
  if (!ge || !game) return;

  BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Atlantic Quest", 20, 20, 30, GOLD);
    /* Aquí irá el render completo del juego en siguientes iteraciones */
  EndDrawing();
}