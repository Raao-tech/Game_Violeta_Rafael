#include <stdlib.h>
#include "graphic_engine.h"
#include "raylib.h"

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


char* graphic_engine_menu_init(char* name_game_default)
{
  Bool  exit_on   = FALSE;
  char* name_game = NULL;
  int   w_boton     = 250;
  int   h_boton     = 50;
  int   intra_space = 10;

  if(!name_game_default ) return NULL;
  name_game = (char*) calloc(WORD_SIZE+1,sizeof(char));
  if(!name_game) return NULL;


  /*Muestro el menú con todas las opciones posibles {New Game, Load Game, Exit}*/
  InitWindow(WIDHT_SCREEN, HIGHT_SCREEN, TITLE);
  SetTargetFPS(FPS);
  while (!WindowShouldClose && exit_on == FALSE)
  {
    z
  }
  
  CloseWindow()

  /*Escucho que opcion ha escogido {New_game, Load_game_1, (Load_game_2) esta segunda opcion es contignente, empezaremos con una sola opcion de guardado}*/

  /*si es New  Game ---> devuelvo el valor de F_DATA_N*/
  /*si es Load Game ---> devuelvo el valor de F_DATA_S*/
  /*si es Exit      ---> devuelvo el valor NULL       */


  return name_game;
}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game) {
  if (!ge || !game) return;

  BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Atlantic Quest", 20, 20, 30, GOLD);
    /* Aquí irá el render completo del juego en siguientes iteraciones */
  EndDrawing();
}