/**
 * @brief It defines the textual graphic engine interface
 *
 * @file graphic_engine_raylib.h
 * @author Rafael
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "graphic_engine.h"
#include "raylib.h"


/*<! Definimos La estrcutra de las colisiones de una entidad. A este se le suam a la psotion*/
struct Collider
{
  int     widht;
  int     height;
};
/*<! Definimos el tipo de dato Entity_grafico que contine toda la info relacionada con los graficos*/
typedef struct _Entity_graphic
{
  Vector2 position;         /*<! Determina la posicion inical del persoanje*/
  struct  Collider;
  char*   img;              /**/

}Entity_ge;

struct _Graphic_engine
{
    Entity_ge  player;
    Entity_ge  *obj;
    Entity_ge  *spc;
    Entity_ge  *numens;
};


/*===================== Private Funtion ==============================*/
Entity_ge* entity_ge_create();
void entity_ge_destroy(Entity_ge* entity);






/*==================================================================== */
Graphic_engine *graphic_engine_create(){
    Graphic_engine* gp =(Graphic_engine*)malloc(sizeof(gp));
    if(!gp) return NULL;

	SetWindowTitle("Atlantic Quest --- Game ");

    gp->player.position.x = ERROR_POSITION;
    gp->player.position.y = ERROR_POSITION;
    gp->spc = entity_ge_create();
    if(!gp->spc) return NULL;
    gp->obj = entity_ge_create();
    if(!gp->obj){
        entity_ge_destroy(gp->obj);
        return NULL;
    }
    gp->numens = entity_ge_create();
    if(!gp->numens){
        entity_ge_destroy(gp->obj);
        entity_ge_destroy(gp->spc);
        return NULL;
    }
}

void graphic_engine_destroy(Graphic_engine *ge){
    if(ge){
        if(ge->numens)  entity_ge_destroy(ge->numens);
        if(ge->obj)     entity_ge_destroy(ge->obj);
        if(ge->spc)     entity_ge_destroy(ge->spc);
        free(ge);
    }
}

void graphic_engine_menu_init(Graphic_engine *ge, Game *game)
{
	int	w_boton = 250;
	int	h_boton = 50;
	int intra_space = 10;
	if(!ge || !game) return;
	InitWindow(WIDHT_SCREEN, HIGHT_SCREEN, "Atlantic Quest ---- Menu Init");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		DrawRectangle(POS_X_INIT, POS_Y_INIT, WIDHT_SCREEN, HIGHT_SCREEN, RAYWHITE); //Background 
		DrawRectangle(MARGIN_WIDHT, MARGIN_HIGHT, WIDHT_SCREEN-MARGIN_WIDHT, HIGHT_SCREEN-MARGIN_HIGHT, SKYBLUE); //Workspace
		
		/*Botones, {New Game, Load Game, Exit}*/
		DrawRectangle(MARGIN_WIDHT+intra_space, MARGIN_HIGHT+intra_space, WIDHT_SCREEN-MARGIN_WIDHT-(intra_space), MARGIN_HIGHT+(intra_space)+h_boton, LIGHTGRAY); //Workspace
		DrawRectangle(MARGIN_WIDHT+intra_space, MARGIN_HIGHT+(intra_space*2)+h_boton,WIDHT_SCREEN-MARGIN_WIDHT-(intra_space), MARGIN_HIGHT+(intra_space*2)+(h_boton*2), SKYBLUE); //Workspace
		DrawRectangle(MARGIN_WIDHT+(intra_space*3), MARGIN_HIGHT+(intra_space*3)+(h_boton*2), WIDHT_SCREEN-MARGIN_WIDHT-(intra_space), HIGHT_SCREEN-MARGIN_HIGHT-intra_space, RED); //Workspace


		EndDrawing();


	}
	
  
  	CloseWindow();
}
void graphic_engine_menu_numens(Graphic_engine *ge, Game *game)
{}

void graphic_engine_paint_game(Graphic_engine *ge, Game *game){
    if(!ge || !game) return;
    Player* player = game_get_player(game);
    Space*  spaces[MAX_SPACES];
    Numen*  numens[MAX_NUMENS];
    Numen*  numens[MAX_NUMENS];


    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
   

}


/*=========== IMPLEMENTATION FUNTION =====================*/

Entity_ge* entity_ge_create()
{
    Entity_ge* e = (Entity_ge*)malloc(sizeof(Entity_ge));
    if(!e) return NULL;

    e->img = NULL;
    e->position.x = ERROR_POSITION;
    e->position.y = ERROR_POSITION;
    
    return e;
}
void entity_ge_destroy(Entity_ge* entity)
{
    if(entity){
        if(entity->img) free(entity->img);
        free(entity);
    }
}
