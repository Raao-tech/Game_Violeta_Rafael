/**
 * @brief Graphic engine (Pokemon-style top-down view + sprite player)
 *
 * 
 * 
 * @file graphic_engine.c
 * @author Rafael
 * @version 3
 * @date 02-05-2026
 * @copyright GNU Public License
 */

#include "graphic_engine.h"
#include "command.h"
#include "game.h"
#include "player.h"
#include "space.h"
#include "object.h"
#include "character.h"
#include "numen.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <inventory.h>




/* ====================================================================== */
/*                     DIMENSIONES TOTALES DE LA PANTALLA                 */
/* ====================================================================== */
#define BIG_TEXT_SIZE			 28
#define MEDIUM_TEXT_SIZE		 20
#define SMALL_TEXT_SIZE 		 16
/*------------------------OVERLAY (HUD MINIMO ARRIBA)---------------------*/
#define OVERLAY_H        SCALE*2      /* franja superior con HP, nombre y objeto activo*/
#define OVERLAY_PAD      8
/*------------------------RIGHT SIDE PANEL (HUD DERECHA)-------------------*/
#define RIGHT_SIDE_PANEL_W  100          /* Franja derecha con numens activos y objetos*/
/*------------------------SKILL PANEL (HUD ABAJO)-------------------*/
#define SKILL_PANEL_H       OVERLAY_H    /* Franja inferior con los 4 ataques */
/*------------------------TOTAL DE LA PANTALLA-------------------*/
#define WIDTH_SCREEN  (WIDHT_MAP + RIGHT_SIDE_PANEL_W)
#define HIGHT_SCREEN  (HIGHT_MAP + SKILL_PANEL_H)
/* ====================================================================== */
/*                     COLORES (OVERALYS, TEXTO, TITULOS, ETC )           */
/* ====================================================================== */

#define COLOR_OVERLAY    (Color){ 0, 0, 0, 160 }
#define COLOR_HP_OK      GREEN
#define COLOR_HP_LOW     RED
#define COLOR_TITLE      GOLD
#define COLOR_TEXT       RAYWHITE

/* Color de fallback cuando una textura no existe — un verde tipo cesped. */
#define COLOR_FALLBACK_BG     (Color){ 60, 120, 60, 255 }
#define COLOR_FALLBACK_PLAYER (Color){ 220, 200, 50, 255 }

/* ====================================================================== */
/*                          STRUCT INTERNA                                 */
/* ====================================================================== */

/* Cada slot empareja un id (o nombre) con su Texture2D cargada. */
typedef struct
{
	Id        id;          /* NO_ID si slot vacio */
	Texture2D tex;
} TexEntry;

typedef struct
{
	char      name[WORD_SIZE];    /* "" si slot vacio */
	Texture2D tex;
} TexEntryName;

struct _Graphic_engine
{
	/* Cache de texturas — se carga una vez en load_textures. */
	TexEntry      space_textures [MAX_SPACE_TEX];
	TexEntryName  player_textures[MAX_PLAYER_TEX];
	TexEntryName  numen_textures [MAX_NUMEN_TEX];
	TexEntryName  object_textures [MAX_OBJECT_TEX];

	Bool textures_loaded;

	/* Música del menú y pantalla final */
	Music menu_music;
	Music gameover_music;
	Bool  menu_music_loaded;
	Bool  gameover_music_loaded;

	/* Música en partida — cambia con el space */
	Music game_music;
	Bool  game_music_loaded;
	Id    game_music_space_id;   /* space cuya OST está sonando; NO_ID = ninguna */
};

/* ====================================================================== */
/*                       PRIVATE FUNCTION PROTOTYPES                       */
/* ====================================================================== */

static Texture2D* ge_get_space_texture  (Graphic_engine* ge, Id space_id);
static Texture2D* ge_get_player_texture (Graphic_engine* ge, const char* name);
static Texture2D* ge_get_numen_texture (Graphic_engine* ge, const char* name);
static Texture2D* ge_get_object_texture (Graphic_engine* ge, const char* name);

static Bool ge_texture_is_valid 		(Texture2D* tex);



static void ge_paint_background  		(Graphic_engine* ge, Game* game, Player* player);
static void ge_paint_player      		(Graphic_engine* ge, Player* player);
static void ge_paint_overlay     		(Game* game, Player* player);
static void ge_paint_right_side_panel	(Graphic_engine* ge, Game* game);
static void ge_paint_numen_right_panel	(Graphic_engine* ge, Numen* numen, int n_painted);
static void ge_paint_object_right_panel	(Graphic_engine* ge, Object* object, int n_painted);
static void ge_paint_active_numen		(Graphic_engine* ge, Game* game, Player* player);
static void ge_paint_objects			(Graphic_engine* ge, Game* game, Player* player);
static void ge_paint_space_numens		(Graphic_engine* ge, Game* game, Player* player, float* opacity);
static void ge_paint_skill_panel     	(Game* game, Player* player);




static const char* ge_status_to_str (Status s);
static const char* ge_cmd_to_str    (CommandCode c);
static void        ge_update_space_music (Graphic_engine* ge, Game* game, Player* player);

static int		ge_find_idx_in_list (Id active_id, int n, Id (*get_at)(Player*, int), Player* player);
static void		ge_cycle_active_object (Player* player);
static void		ge_cycle_active_numen  (Game* game, Player* player);
static 	int		_ge_max_radio_skill_of_numen (Numen* numen);
static void		_ge_draw_bars (Numen* numen);
static void		_ge_draw_effect_attack (Numen* numen, float opacity);

/* ====================================================================== */
/*                         PUBLIC: CREATE / DESTROY                        */
/* ====================================================================== */

Graphic_engine*
graphic_engine_create (void)
{
	Graphic_engine* ge = (Graphic_engine*)calloc (1, sizeof (Graphic_engine));
	int i;

	if (!ge) return NULL;

	/* calloc ya pone todo a 0, pero somos explicitos para que se lea
	 * lo que esta pasando: ningun id, ningun nombre, ninguna textura. */
	for (i = 0; i < MAX_SPACE_TEX;  i++) ge->space_textures[i].id  = NO_ID;
	for (i = 0; i < MAX_PLAYER_TEX; i++) ge->player_textures[i].name[0] = '\0';
	for (i = 0; i < MAX_NUMEN_TEX;  i++) ge->numen_textures[i].name[0]  = '\0';
	for (i = 0; i < MAX_OBJECT_TEX; i++) ge->object_textures[i].name[0] = '\0';

	ge->textures_loaded       = FALSE;
	ge->menu_music_loaded     = FALSE;
	ge->gameover_music_loaded = FALSE;
	ge->game_music_loaded     = FALSE;
	ge->game_music_space_id   = NO_ID;

	InitWindow (WIDTH_SCREEN, HIGHT_SCREEN, TITLE);
	InitAudioDevice ();
	SetTargetFPS (FPS);
	return ge;
}

void
graphic_engine_destroy (Graphic_engine* ge)
{
	int i;
	if (!ge) return;

	/* Liberamos las texturas SOLO si la ventana sigue viva. */
	if (IsWindowReady () && ge->textures_loaded)
	{
		for (i = 0; i < MAX_SPACE_TEX; i++)
			if (ge->space_textures[i].id != NO_ID)
				UnloadTexture (ge->space_textures[i].tex);
		for (i = 0; i < MAX_PLAYER_TEX; i++)
			if (ge->player_textures[i].name[0] != '\0')
				UnloadTexture (ge->player_textures[i].tex);
		for (i = 0; i < MAX_NUMEN_TEX; i++)
			if (ge->numen_textures[i].name[0] != '\0')
				UnloadTexture (ge->numen_textures[i].tex);
		for (i = 0; i < MAX_OBJECT_TEX; i++)
			if (ge->object_textures[i].name[0] != '\0')
				UnloadTexture (ge->object_textures[i].tex);
	}

	/* Descargamos la música si fue cargada */
	if (ge->menu_music_loaded)     UnloadMusicStream (ge->menu_music);
	if (ge->gameover_music_loaded) UnloadMusicStream (ge->gameover_music);
	if (ge->game_music_loaded)     UnloadMusicStream (ge->game_music);

	CloseAudioDevice ();

	/* La ventana se cierra DESPUES de descargar texturas */
	if (IsWindowReady ()) CloseWindow ();

	free (ge);
}

/* ====================================================================== */
/*                       PUBLIC: MENU INIT (sin cambios)                   */
/* ====================================================================== */

MenuResult
graphic_engine_init (Graphic_engine* ge)
{
	MenuResult result;
	Bool exit_on = FALSE;

	result.data_name[0] = '\0';
	result.init_numen   = NO_ID;
	result.menu_out     = OUT_ERR;

	if (!ge) return result;

	/* Música del menú */
	ge->menu_music        = LoadMusicStream ("./msc_src/OST/musica_chill_persona_5_v1.mp3");
	ge->menu_music_loaded = TRUE;
	SetMusicVolume (ge->menu_music, 0.5f);
	PlayMusicStream (ge->menu_music);

	while (!WindowShouldClose () && exit_on == FALSE)
	{
		UpdateMusicStream (ge->menu_music);

		BeginDrawing ();
		ClearBackground (RAYWHITE);

		if (result.menu_out == OUT_ERR)
		{
			DrawText (TITLE, (int)(WIDHT_MAP * (3.0 / 8.0)), 100, 20, DARKGRAY);

			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (3.0 / 8.0)), 150, 200, 50 }, "NEW GAME"))
				result.menu_out = NEW_GAME;
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (3.0 / 8.0)), 220, 200, 50 }, "LOAD GAME"))
				result.menu_out = LOAD_GAME;
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (3.0 / 8.0)), 290, 200, 50 }, "Exit"))
			{
				result.menu_out = EXIT_Q;
				exit_on         = TRUE;
			}
		}
		else if (result.menu_out == NEW_GAME)
		{
			DrawText ("Choose your Initial Numen", (int)(WIDHT_MAP * (3.0 / 8.0)), 100, 20, DARKGRAY);

			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (6.0 / 8.0)), 10, 100, 30 }, "BACK"))
				result.menu_out = OUT_ERR;
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 150, 200, 50 }, "BROCOCHACHO"))
			{
				strncpy (result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = First_numen;
				exit_on                     = TRUE;
			}
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 220, 200, 50 }, "SQUIRTLE"))
			{
				strncpy (result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = Second_numen;
				exit_on                     = TRUE;
			}
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 290, 200, 50 }, "CHARMANDER"))
			{
				strncpy (result.data_name, F_DATA_N, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = third_numen;
				exit_on                     = TRUE;
			}
		}
		else if (result.menu_out == LOAD_GAME)
		{
			DrawText ("WHITCH YOUR GAME?", 300, 100, 20, DARKGRAY);

			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (6.0 / 8.0)), 10, 100, 30 }, "BACK"))
				result.menu_out = OUT_ERR;
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 150, 200, 50 }, "LOAD GAME #1"))
			{
				strncpy (result.data_name, F_DATA_S_1, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = NO_ID;
				exit_on                     = TRUE;
			}
			if (GuiButton ((Rectangle){ (int)(WIDHT_MAP * (2.0 / 8.0)), 220, 200, 50 }, "LOAD GAME #2"))
			{
				strncpy (result.data_name, F_DATA_S_2, WORD_SIZE);
				result.data_name[WORD_SIZE] = '\0';
				result.init_numen           = NO_ID;
				exit_on                     = TRUE;
			}
		}

		EndDrawing ();
	}

	if (WindowShouldClose () && result.menu_out == OUT_ERR)
		result.menu_out = EXIT_Q;

	/* Detenemos la música del menú */
	if (ge->menu_music_loaded)
	{
		StopMusicStream (ge->menu_music);
		UnloadMusicStream (ge->menu_music);
		ge->menu_music_loaded = FALSE;
	}

	return result;
}

/* ====================================================================== */
/*                  PUBLIC: LOAD TEXTURES (una sola vez)                   */
/* ====================================================================== */

Status
graphic_engine_load_textures (Graphic_engine* ge, Game* game)
{
	int   i, n_spaces, slot;
	char  path[WORD_SIZE];
	Space* sp = NULL;
	Player* pl = NULL;
	Numen*  nu = NULL;

	if (!ge || !game) return ERROR;
	if (ge->textures_loaded == TRUE) return OK;   /* idempotente */

	/* ---- SPACES ---- */
	n_spaces = game_get_n_spaces (game);
	for (i = 0, slot = 0; i < n_spaces && slot < MAX_SPACE_TEX; i++)
	{
		Id space_id = game_get_space_id_at (game, i);
		sp = game_get_space (game, space_id);
		if (!sp) continue;

		/* El path lo construimos con el name del space, no con el gdesc.
		 * Asi un space "Loby" siempre busca ./img_src/background/Loby.png
		 * sin depender de lo que ponga en el .dat. */
		snprintf (path, sizeof (path), "%s", space_get_gdesc (sp));

		ge->space_textures[slot].id  = space_id;
		ge->space_textures[slot].tex = LoadTexture (path);
		slot++;
	}

	/* ---- PLAYERS ---- */
	{
		int n_players = game_get_n_players (game);
		for (i = 0, slot = 0; i < n_players && slot < MAX_PLAYER_TEX; i++)
		{
			pl = game_get_player_at (game, i);
			if (!pl) continue;

			snprintf (path, sizeof (path), "./img_src/sprites/players/%s.png", player_get_name (pl));

			strncpy (ge->player_textures[slot].name,
					 player_get_name (pl), 63);
			ge->player_textures[slot].name[63] = '\0';
			ge->player_textures[slot].tex = LoadTexture (path);
			slot++;
		}
	}

	/* ---- NUMENS  ---- */
	{
		int n_numens = game_get_n_numens (game);
		for (i = 0, slot = 0; i < n_numens && slot < MAX_NUMEN_TEX; i++)
		{
			nu = game_get_numen_at (game, i);
			if (!nu) continue;
			snprintf (path, sizeof (path),
					  "./img_src/sprites/numens/%s.png", numen_get_gdesc (nu));
			strncpy (ge->numen_textures[slot].name,
					 numen_get_name (nu), 63);
			ge->numen_textures[slot].name[63] = '\0';
			ge->numen_textures[slot].tex = LoadTexture (path);
			slot++;
		}
	}
	/*====== objects =========*/
	{
		int n_objs = game_get_n_objects (game);
		Object* o;
		for (i = 0, slot = 0; i < n_objs && slot < MAX_OBJECT_TEX; i++)
		{
			o = game_get_object_at (game, i);
			if (!o) continue;
		
			/* gdesc holds the image filename; name is the cache key */
			snprintf (path, sizeof (path), "./img_src/sprites/objects/%s.png", obj_get_gdesc (o));

			strncpy (ge->object_textures[slot].name, obj_get_name (o), 63);
			ge->object_textures[slot].name[63] = '\0';
			ge->object_textures[slot].tex = LoadTexture (path);
			slot++;
		}
	}

	ge->textures_loaded = TRUE;
	return OK;
}

/* ====================================================================== */
/*           PRIVATE: SPACE MUSIC — carga y reproduce el OST del space    */
/* ====================================================================== */

static void
ge_update_space_music (Graphic_engine* ge, Game* game, Player* player)
{
	Id     zone_id;
	Space* sp;
	char*  ost_path;

	if (!ge || !game || !player) return;

	zone_id = player_get_zone (player);
	if (zone_id == ge->game_music_space_id)
	{
		/* Mismo space: solo actualizar el stream */
		if (ge->game_music_loaded)
			UpdateMusicStream (ge->game_music);
		return;
	}

	/* Space distinto: parar y descargar la música anterior */
	if (ge->game_music_loaded)
	{
		StopMusicStream   (ge->game_music);
		UnloadMusicStream (ge->game_music);
		ge->game_music_loaded   = FALSE;
		ge->game_music_space_id = NO_ID;
	}

	sp = game_get_space (game, zone_id);
	if (!sp) return;

	ost_path = space_get_ost (sp);
	if (!ost_path || ost_path[0] == '\0') return;

	ge->game_music        = LoadMusicStream (ost_path);
	ge->game_music_loaded = TRUE;
	ge->game_music_space_id = zone_id;
	SetMusicVolume (ge->game_music, 0.45f);
	PlayMusicStream (ge->game_music);
}

/* ====================================================================== */
/*                  PUBLIC: PAINT GAME (cada frame)                        */
/* ====================================================================== */

void
graphic_engine_paint_game (Graphic_engine* ge, Game* game)
{
	Player* player;
	float	opacity = 0.0f;

	if (!ge || !game) return;
	player = game_get_player_by_turn (game);
	if (!player) return;

	/* Actualizar / cambiar música del space actual */
	ge_update_space_music (ge, game, player);

	ClearBackground (BLACK);

	ge_paint_background  (ge, game, player);
	ge_paint_objects       (ge, game, player);
	ge_paint_space_numens  (ge, game, player, &opacity);
	ge_paint_active_numen (ge, game, player);
	ge_paint_player      (ge, player);
	ge_paint_overlay     (game, player);
	ge_paint_right_side_panel(ge, game);
	ge_paint_skill_panel     	(game, player);

}

/* ====================================================================== */
/*                  PUBLIC: NON-COMMAND UI INPUT                           */
/* ====================================================================== */

void
graphic_engine_handle_ui_input (Graphic_engine* ge, Game* game)
{
	Player* player;
	(void)ge;

	if (!game) return;
	player = game_get_player_by_turn (game);
	if (!player) return;

	if (IsKeyPressed (KEY_TAB)) 			ge_cycle_active_object (player);
	if (IsKeyPressed (KEY_LEFT_SHIFT))		ge_cycle_active_numen  (game, player);
}

/* ====================================================================== */
/*                       PRIVATE: BACKGROUND                               */
/* ====================================================================== */

static void
ge_paint_background (Graphic_engine* ge, Game* game_unused, Player* player)
{
	Id         space_id;
	Texture2D* tex;
	Rectangle  src, dst;

	(void)game_unused;
	space_id = player_get_zone (player);
	tex      = ge_get_space_texture (ge, space_id);

	if (ge_texture_is_valid (tex))
	{
		/* Escalamos la textura a la pantalla entera.
		 * src abarca toda la imagen original, dst es toda la pantalla. */
		src = (Rectangle){ 0, 0, (float)tex->width, (float)tex->height };
		dst = (Rectangle){ 0, 0, (float)WIDHT_MAP, (float)HIGHT_MAP };
		DrawTexturePro (*tex, src, dst,
						(Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Sin imagen: rectangulo verde plano para no romper la lectura. */
		DrawRectangle (0, 0, WIDHT_MAP, HIGHT_MAP, COLOR_FALLBACK_BG);
	}


	{
		int i;
		for (i = 1; i < WIDHT_MAP;  i++)
			DrawLine (i * SCALE, 0, i * SCALE, HIGHT * SCALE,
					  (Color){ 255, 255, 255, 30 });
		for (i = 1; i < HIGHT_MAP;  i++)
			DrawLine (0, i * SCALE, WIDHT * SCALE, i * SCALE,
					  (Color){ 255, 255, 255, 30 });
	}
}

/* ====================================================================== */
/*                       PRIVATE: PLAYER                                    */
/* ====================================================================== */

static void
ge_paint_player (Graphic_engine* ge, Player* player)
{
	int        px, py;
	Texture2D* tex;

	px  = player_get_pos_x (player);
	py  = player_get_pos_y (player);
	tex = ge_get_player_texture (ge, player_get_name (player));

	if (ge_texture_is_valid (tex))
	{
		Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
		Rectangle dst = { (float)px, (float)py, (float)SCALE, (float)SCALE };
		DrawTexturePro (*tex, src, dst,(Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Cuadrado amarillo de fallback */
		DrawRectangle (px, py, SCALE, SCALE, COLOR_FALLBACK_PLAYER);
		DrawRectangleLines (px, py, SCALE, SCALE, BLACK);
	}
}

/* ====================================================================== */
/*                       PRIVATE: OVERLAY (HUD ARRIBA)                     */
/* ====================================================================== */

static void
ge_paint_overlay (Game* game, Player* player)
{
	Space*       sp = NULL;
	Numen*       numen=NULL;
	Object*      object=NULL;
	Command*     last_cmd = NULL;
	const char*  space_name = NULL;
	const char*  cmd_label = NULL;
	const char*  status_label = NULL;
	const char*  numen_name = NULL;
	const char*  object_name = NULL;
	Id           Id_act_num=NO_ID, Id_act_obj=NO_ID;
	Color        hp_color, sp_color;
	int          hp_numen, sp_numen;


	if (!game || !player) return;

	Id_act_num=player_get_active_numen(player);
	numen=game_get_numen_by_id(game, Id_act_num);
	if(numen)
	{
		/* Franja semi-transparente arriba */
		DrawRectangle (0, 0, WIDHT_MAP + RIGHT_SIDE_PANEL_W, OVERLAY_H, COLOR_OVERLAY);

		/*Nombre del numen a la izquierda*/
		numen_name=numen_get_name(numen) != NULL ?  numen_get_name(numen) : "";
		DrawText(numen_name, OVERLAY_PAD ,7, MEDIUM_TEXT_SIZE, COLOR_TEXT);
		/* HP a la izquierda */
		hp_numen = numen_get_health (numen);
		hp_color  = (hp_numen > 3) ? COLOR_HP_OK : COLOR_HP_LOW;
		DrawText (TextFormat ("HP %d", hp_numen),
				  OVERLAY_PAD+20, 36, SMALL_TEXT_SIZE, hp_color);
		/*SP al lado del HP*/
		sp_numen = numen_get_energy (numen);
		sp_color  = COLOR_TITLE;
		DrawText (TextFormat ("SP %d", sp_numen),
				  (OVERLAY_PAD+68), 36, SMALL_TEXT_SIZE, sp_color);
	}





	/* Nombre del space en el centro */
	sp         = game_get_space (game, player_get_zone (player));
	if (sp)
	{
		space_name = sp ? space_get_name (sp) : "?";
		DrawText (space_name,
			  WIDHT_MAP / 2 - MeasureText (space_name, BIG_TEXT_SIZE) / 2,
			  27, BIG_TEXT_SIZE, COLOR_TITLE);
	}

	/* Resultado del ultimo comando a la derecha */
	last_cmd = game_get_last_command (game);
	if (last_cmd)
	{
		cmd_label    = ge_cmd_to_str (command_get_code (last_cmd));
		status_label = ge_status_to_str (game_get_last_cmd_status (game));
		DrawText (TextFormat ("%s:%s", cmd_label, status_label),
				  WIDHT_MAP - 60, 40, SMALL_TEXT_SIZE, COLOR_TEXT);
	}

	/*Si existe el objeto*/
	Id_act_obj=player_get_active_object(player);
	object=game_get_object_by_id(game, Id_act_obj);
	if(object)
	{
		/*Nombre del object a la izquierda*/
		object_name=obj_get_name(object);
		DrawText(object_name, WIDHT_MAP - MeasureText (object_name, MEDIUM_TEXT_SIZE) / 2, 21, MEDIUM_TEXT_SIZE, COLOR_TEXT); 
	}	
}

 
/* ====================================================================== */
/*                 PRIVATE: RIHT SIDE PANEL (HUD DERECHA)                 */
/* ====================================================================== */
static void ge_paint_right_side_panel (Graphic_engine* ge, Game* game)
{
    Player* player = NULL;
    Numen*  numen  = NULL;
    Object* object = NULL;
    int n_nums, n_objs, n, o;

    player = game_get_player_at(game, PLAYER);
    if (!player) return;

    n_nums = player_get_n_numens(player);
    n_objs = player_get_n_objects(player);


	/* Franja semi-transparente arriba */
	DrawRectangle (WIDHT_MAP, 0, WIDTH_SCREEN, HIGHT_SCREEN, DARKGRAY);

    for (n = 0; n < n_nums; n++)
    {
        numen = game_get_numen_by_id(game, player_get_numen_at_inventory(player, n));
        if (numen) ge_paint_numen_right_panel(ge, numen, n);
        if (n == 3) break;
    }

    for (o = 0; o < n_objs; o++)
    {
        object = game_get_object_by_id(game, player_get_object_at_inventory(player, o));
        if (object) ge_paint_object_right_panel(ge, object, n + o + 2);
        if (o == 3) break;
    }
}
/* ====================================================================== */
/*                       PRIVATE: NUMEN (RIGHT PANEL)                     */
/* ====================================================================== */
static void
ge_paint_numen_right_panel (Graphic_engine* ge, Numen* numen, int n_painted)
{
	int        px, py;
	Texture2D* tex;

	if (!ge || !numen ) return;
	
	tex = ge_get_numen_texture (ge, numen_get_name (numen));
	if (!tex) return;
	
	px  = WIDHT_MAP;

	py  =(int)(tex->height+SCALE)*n_painted;
   

	if (ge_texture_is_valid (tex))
	{
		Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
		Rectangle dst = { (float)px, (float)py, (float)SCALE*2, (float)SCALE*2 };
		DrawTexturePro (*tex, src, dst,	(Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Cuadrado amarillo de fallback */
		DrawRectangle (px, py, SCALE, SCALE, COLOR_FALLBACK_PLAYER);
		DrawRectangleLines (px, py, SCALE, SCALE, BLACK);
	}
}

/* ====================================================================== */
/*                       PRIVATE: OBJECT (RIGHT PANEL)                     */
/* ====================================================================== */
static void
ge_paint_object_right_panel(Graphic_engine*ge, Object* object, int n_painted)
{
	int        px, py;
	Texture2D* tex;
	if (!ge || !object ) return;
	
	tex = ge_get_object_texture (ge, obj_get_name (object));
	if(!tex) return;
	
	px  = WIDHT_MAP;

	py  =(SCALE)*n_painted;
   

	if (ge_texture_is_valid (tex))
	{
		Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
		Rectangle dst = { (float)px, (float)py, (float)SCALE*2, (float)SCALE*2 };
		DrawTexturePro (*tex, src, dst,	(Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Cuadrado amarillo de fallback */
		DrawRectangle (px, py, SCALE*2, SCALE*2, COLOR_FALLBACK_PLAYER);
		DrawRectangleLines (px, py, SCALE*2, SCALE*2, BLACK);
	}
}
/* ====================================================================== */
/*                   PRIVATE: SKILL PANEL (HUD INFERIOR)                  */
/*                                                                         */
/* Dibuja N_SKILLS slots horizontales justo debajo del mapa (y=HIGHT_MAP). */
/* Cada slot muestra: número de hotkey, nombre del skill y daño.           */
/* ====================================================================== */
static void
ge_paint_skill_panel (Game* game, Player* player)
{
	Numen*    numen    = NULL;
	Id        id_numen = NO_ID;
	Skills_id skill;
	char*     name;
	int       i, slot_x, dmg, slot_w;
	Color     bg_active, bg_empty, border_col;

	if (!game || !player) return;

	slot_w    = WIDHT_MAP / N_SKILLS;
	bg_active = (Color){ 40, 40, 65, 230 };
	bg_empty  = (Color){ 25, 25, 35, 200 };

	/* Fondo del panel completo */
	DrawRectangle (0, HIGHT_MAP, WIDHT_MAP, SKILL_PANEL_H, (Color){ 15, 15, 25, 240 });
	DrawLine (0, HIGHT_MAP, WIDHT_MAP, HIGHT_MAP, GOLD);

	id_numen = player_get_active_numen (player);
	numen    = game_get_numen_by_id (game, id_numen);

	for (i = 0; i < N_SKILLS; i++)
	{
		slot_x = i * slot_w;

		skill = (numen) ? numen_get_skill_by_index (numen, i) : NO_SKILL;
		name  = (skill != NO_SKILL) ? skill_get_name (skill) : NULL;
		dmg   = (skill != NO_SKILL) ? skill_get_damage (skill) : 0;

		/* Fondo del slot */
		DrawRectangle (slot_x + 2, HIGHT_MAP + 2, slot_w - 4, SKILL_PANEL_H - 4,
		               (skill != NO_SKILL) ? bg_active : bg_empty);

		border_col = (skill != NO_SKILL) ? GOLD : DARKGRAY;
		DrawRectangleLines (slot_x + 2, HIGHT_MAP + 2, slot_w - 4, SKILL_PANEL_H - 4, border_col);

		/* Número de hotkey */
		DrawText (TextFormat ("%d", i + 1),
		          slot_x + 6, HIGHT_MAP + 5,
		          SMALL_TEXT_SIZE,
		          (skill != NO_SKILL) ? COLOR_TITLE : DARKGRAY);

		/* Nombre del skill */
		DrawText (name ? name : "---",
		          slot_x + 22, HIGHT_MAP + 5,
		          SMALL_TEXT_SIZE,
		          (skill != NO_SKILL) ? COLOR_TEXT : DARKGRAY);

		/* Daño */
		if (skill != NO_SKILL)
			DrawText (TextFormat ("DMG %d", dmg),
			          slot_x + 6, HIGHT_MAP + 26,
			          SMALL_TEXT_SIZE, COLOR_HP_LOW);

		/* Separador vertical entre slots */
		if (i < N_SKILLS - 1)
			DrawLine (slot_x + slot_w, HIGHT_MAP, slot_x + slot_w, HIGHT_SCREEN,
			          (Color){ 80, 80, 80, 180 });
	}
}
/* ====================================================================== */
/*                       PRIVATE: TEXTURE LOOKUP                           */
/* ====================================================================== */

static Texture2D*
ge_get_space_texture (Graphic_engine* ge, Id space_id)
{
	int i;
	if (!ge || space_id == NO_ID) return NULL;
	for (i = 0; i < MAX_SPACE_TEX; i++)
	{
		if (ge->space_textures[i].id == space_id)	return (&ge->space_textures[i].tex);
	}
	return NULL;
}

static Texture2D*
ge_get_player_texture (Graphic_engine* ge, const char* name)
{
	int i;
	if (!ge || !name) return NULL;
	for (i = 0; i < MAX_PLAYER_TEX; i++)
	{
		if (ge->player_textures[i].name[0] != '\0' &&
			strcmp (ge->player_textures[i].name, name) == 0)
			return &ge->player_textures[i].tex;
	}
	return NULL;
}

/* Una textura "default" de raylib (cuando no se ha cargado fichero)
 * tiene id 0 y dimensiones 0x0 o 1x1. La rechazamos para usar fallback. */
static Bool
ge_texture_is_valid (Texture2D* tex)
{
	if (!tex) return FALSE;
	if (tex->id == 0)  return FALSE;
	if (tex->width <= 1 && tex->height <= 1) return FALSE;
	return TRUE;
}

/* ====================================================================== */
/*                  PRIVATE: CICLAR active_object / active_numen           */
/* ====================================================================== */

static int
ge_find_idx_in_list (Id active_id, int n, Id (*get_at)(Player*, int), Player* player)
{
	int i;
	if (!get_at || !player) return -1;
	for (i = 0; i < n; i++)
	{
		if (get_at (player, i) == active_id) return i;
	}
	return -1;
}

static void
ge_cycle_active_numen (Game* game, Player* player)
{
    Id      active_id, new_active_id;
    Numen*  new_active;
	Numen*  old_active=NULL;
    Position p;
    int     n, current_idx, next_idx;

    if (!game || !player) return;
    n = player_get_n_numens (player);
    if (n <= 1) return;

    active_id   = player_get_active_numen (player);
    current_idx = ge_find_idx_in_list (active_id, n,
                                       player_get_numen_at_inventory, player);
    next_idx    = (current_idx + 1) % n;
    new_active_id = player_get_numen_at_inventory (player, next_idx);

    if (active_id != NO_ID)
    {
      	old_active = game_get_numen_by_id (game, active_id);
        if (old_active)
        {
            numen_set_pos_x (old_active, NO_POS);
            numen_set_pos_y (old_active, NO_POS);
        }
    }

    new_active = game_get_numen_by_id (game, new_active_id);
    if (new_active)
    {
        p = player_get_position (player);
        if (p.pos_x != NO_POS && p.pos_y != NO_POS)
        {
            numen_set_pos_x (new_active, p.pos_x - SCALE);
            numen_set_pos_y (new_active, p.pos_y);
        }
    }

    player_set_active_numen (player, new_active_id);
}

static void
ge_cycle_active_object (Player* player)
{
    Id      active_id	= NO_ID;
	Id		new_active_id = NO_ID;
    int     n, current_idx, next_idx;

    if (!player) return;
    n = player_get_n_objects (player);
    if (n <= 1) return;

    active_id   = player_get_active_object (player);
    current_idx = ge_find_idx_in_list (active_id, n,	player_get_object_at_inventory, player);
    next_idx    = (current_idx + 1) % n; /*Nos movemos al siguiente object en el inventario*/

	new_active_id = player_get_object_at_inventory (player, next_idx);

    player_set_active_object (player, new_active_id);
}

/* ====================================================================== */
/*                      PRIVATE: ENUM → STRING                             */
/* ====================================================================== */

static const char*
ge_status_to_str (Status estado)
{
	switch (estado)
	{
		case OK:                 return "OK";
		case ERROR:              return "ERROR";
		case ERROR_Attack:       return "ATK!";
		case ERROR_walk:         return "WALK!";
		case ERROR_Chat:         return "CHAT!";
		case ERROR_dir:          return "DIR!";
		case ERROR_take:         return "TAKE!";
		case ERROR_drop:         return "DROP!";
		case ERROR_inspect:      return "INSP!";
		case ERROR_use:          return "USE!";
		case ERROR_open:         return "OPEN!";
		case ERROR_save:         return "SAVE!";
		case ERROR_load:         return "LOAD!";
		case ERROR_recruit:      return "REC!";
		case ERROR_kick:         return "KICK!";
		case ERROR_enemy_attack: return "ENEMY";
		case ERROR_enemy_move:   return "EMOV";
		default:                 return "?";
	}
}

static const char*
ge_cmd_to_str (CommandCode code)
{
	switch (code)
	{
		case UNKNOWN: return "??";
		case EXIT:    return "exit";
		case MOVE:    return "move";
		case WALK:    return "walk";
		case TAKE:    return "take";
		case DROP:    return "drop";
		case ATTACK:  return "atk";
		case CHAT:    return "chat";
		case INSPECT: return "insp";
		case USE:     return "use";
		case SAVE:    return "save";
		case LOAD:    return "load";
		case RECRUIT: return "rec";
		case KICK:    return "kick";
		default:      return "";
	}
}
/* ====================================================================== */
/*                  PRIVATE: ACTIVE NUMEN (pintado)                        */
/* ====================================================================== */

static void
ge_paint_active_numen (Graphic_engine* ge, Game* game, Player* player)
{
	Numen*     		num = NULL;
	Texture2D* 		tex = NULL;
	Id         		active_id = NO_ID;
	Position        pos_numen;

	/*==================== GET ===============================*/
		active_id = player_get_active_numen (player);
		if (active_id == NO_ID) return;

		num = game_get_numen_by_id (game, active_id);
		if (!num) return;
	/*=======================================================*/
	/*==================== POSITION  ==========================*/
		pos_numen = numen_get_position (num);

		/* Si por algun motivo no tiene posicion valida, lo colocamos a la derecha del player. */
		if (types_position_is_valid (num,(Position (*) (void*)) numen_get_position, 0, 0, WIDHT_MAP, HIGHT_MAP) == FALSE)
		{
			pos_numen.pos_x = player_get_pos_x (player) + SCALE;
			pos_numen.pos_y = player_get_pos_y (player);
			numen_set_position (num, pos_numen.pos_x, pos_numen.pos_y);
		}
	/*=======================================================*/

	
	tex = ge_get_numen_texture (ge, numen_get_name (num));

	if (ge_texture_is_valid (tex))
	{
		Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
		Rectangle dst = { (float)pos_numen.pos_x, (float)pos_numen.pos_y, (float)SCALE, (float)SCALE };
		DrawTexturePro (*tex, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);
	}
	else
	{
		/* Fallback: circulo del color de los numens corruptos vs friendly.
		 * El "B" de Brococacho lo pintamos como letra encima del circulo
		 * para que se distinga sin sprite. */
		Color   col;
		char*   gd;

		col = (numen_get_corrupt (num) == TRUE)
			? (Color){ 200, 60, 60, 255 }     /* enemigo: rojo */
			: (Color){ 80, 200, 80, 255 };    /* amigo: verde  */

		DrawCircle (pos_numen.pos_x + (SCALE / 2), pos_numen.pos_y + (SCALE / 2), (SCALE / 2) - 2, col);
		DrawCircleLines (pos_numen.pos_x  + SCALE / 2, pos_numen.pos_y  + SCALE / 2, SCALE / 2 - 2, BLACK);

		gd = numen_get_gdesc (num);
		if (gd && gd[0])
			DrawText (TextFormat ("%c", gd[0]),
					  pos_numen.pos_x + SCALE / 2 - 4, pos_numen.pos_y + SCALE / 2 - 6, 12, BLACK);
	}
}

/* ====================================================================== */
/*                  PRIVATE: NUMEN TEXTURE LOOKUP                          */
/* ====================================================================== */
static Texture2D*
ge_get_numen_texture (Graphic_engine* ge, const char* name)
{
	int i;
	if (!ge || !name) return NULL;
	for (i = 0; i < MAX_NUMEN_TEX; i++)
	{
		if (ge->numen_textures[i].name[0] != '\0' &&
			strcmp (ge->numen_textures[i].name, name) == 0)
			return &ge->numen_textures[i].tex;
	}
	return NULL;
}
/* ====================================================================== */
/*           PRIVATE: OBJECTS DEL SPACE ACTUAL                             */
/* ====================================================================== */
static void
ge_paint_objects (Graphic_engine* ge, Game* game, Player* player)
{
	Space*     sp = NULL;
	Object*    obj = NULL;
	Texture2D* tex = NULL;
	Position 	vision, position;
	Id         space_id = NO_ID;
	int        i, n_objs;

	if (!ge || !game || !player) return;

	/*================ SPACE =======================================*/
		/*nos aseguramos de tener el objeto del sapce en el que está el player*/
		space_id = player_get_zone (player);
		sp       = game_get_space (game, space_id);
		if (!sp) return;
	/*===============================================================*/

	/*================ DIBUJAR RECUADRO SEMIBLANCO ================*/
		vision = player_get_vision(player);
		if (vision.pos_x != NO_POS && vision.pos_y != NO_POS)
			DrawRectangle(vision.pos_x, vision.pos_y, SCALE, SCALE, (Color){255,255,255,150});
	/*================ ==============================================*/

	/*================ PINTAMOS =======================================*/

		n_objs = game_get_n_objects (game);
		for (i = 0; i < n_objs; i++)
		{
			/*Obtenemos el objecto*/
			obj = game_get_object_at (game, i);
			if (!obj) continue;
			/* Si el objeto está en el epsacio, lo pintamos */
			if (space_contains_object (sp, obj_get_id (obj)) == FALSE) continue;


			/*Aduirimos su posición, y revismao que sea una posición valida*/
			position = obj_get_position (obj);
			if (types_position_is_valid (obj, (Position (*)(void *)) obj_get_position, 0,0, WIDHT_MAP, HIGHT_MAP) == FALSE) continue;



			/*Pintamos*/
			tex = ge_get_object_texture (ge, obj_get_name (obj));
			if (ge_texture_is_valid (tex))
			{
				Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
				Rectangle dst = { (float)position.pos_x, (float)position.pos_y, (float)SCALE, (float)SCALE };
				DrawTexturePro (*tex, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
			}
			else
			{
				/* cuadrado gris con la letra del gdesc */
				char* gd = obj_get_gdesc (obj);
				DrawRectangle (position.pos_x, position.pos_y, SCALE, SCALE, (Color){ 150, 150, 150, 200 });
				DrawRectangleLines (position.pos_x, position.pos_y, SCALE, SCALE, BLACK);
				if (gd && gd[0])
					DrawText (TextFormat ("%c", gd[0]),
							  position.pos_x + SCALE/2 - 4, position.pos_y + SCALE/2 - 6, 12, BLACK);
			}
		}

	/*===============================================================*/

	


}


static Texture2D*
ge_get_object_texture (Graphic_engine* ge, const char* name)
{
	int i;
	if (!ge || !name) return NULL;
	for (i = 0; i < MAX_OBJECT_TEX; i++)
	{
		if (ge->object_textures[i].name[0] != '\0' &&
			strcmp (ge->object_textures[i].name, name) == 0)
			return &ge->object_textures[i].tex;
	}
	return NULL;
}



/* ====================================================================== */
/*           PRIVATE: NUMENS DEL SPACE ACTUAL                              */
/*                                                                         */
/* Recorre los numens del juego y pinta los que viven en el space actual   */
/* del player y NO son el active_numen (que ya lo pinta paint_active_numen)*/
/*                                                                         */
/*   - Pinta una barra de HP y de energia encima de los numens              */
/*   - Pinta una barra de HP energia encima del enemigo.                    */
/*   - Si el numen activo del player esta dentro del radio de combate,     */
/*     resalta el enemigo con marco rojo brillante y une con linea al      */
/*     numen activo (feedback visual de "amenaza/objetivo").               */
/* ====================================================================== */
static void
ge_paint_space_numens (Graphic_engine* ge, Game* game, Player* player, float* opacity)
{
	Numen*     	num = NULL;
	Numen*     	active_num = NULL;
	Texture2D* 	tex = NULL;
	char*      	gd = NULL;
	Command*	last_cmd = NULL;
	Id         active_id, num_id, space_id;
	Position	pos_active, pos_numen;
	int        n_numens, i;
	int        dx, dy, dist_sq, range_sq;
	Bool       is_corrupt, is_errant, in_range;
	Color      col;
	int        max_radio;

	if (!ge || !game || !player) return;
	pos_active.pos_x = pos_active.pos_y = NO_POS;
	max_radio = 0;

	active_id = player_get_active_numen (player);
	space_id  = player_get_zone (player);
	if (space_id == NO_ID) return;

	last_cmd = game_get_last_command (game);

	active_num = (active_id != NO_ID)
	             ? game_get_numen_by_id (game, active_id)
	             : NULL;

	if (active_num)
	{
		_ge_draw_bars (active_num);
		pos_active = numen_get_position (active_num);
		max_radio  = _ge_max_radio_skill_of_numen (active_num);
	}
	range_sq = (max_radio > 0) ? max_radio * max_radio : (SCALE * 3) * (SCALE * 3);

	if (command_get_code (last_cmd) == ATTACK) (*opacity) = 1.0f;

	if (*opacity > 0.0f)
	{
		*opacity -= GetFrameTime ();
		_ge_draw_effect_attack (active_num, *opacity);
	}

	n_numens = game_get_n_numens (game);

	for (i = 0; i < n_numens; i++)
	{
		num = game_get_numen_at (game, i);
		if (!num) continue;

		num_id = numen_get_id (num);
		if (num_id == active_id) continue;

		if (game_get_numen_location (game, num_id) != space_id) continue;

		pos_numen = numen_get_position (num);
		if (pos_numen.pos_x == NO_POS || pos_numen.pos_y == NO_POS) continue;

		is_corrupt = numen_get_corrupt (num);
		is_errant  = numen_is_errant   (num);

		/* Distancia al numen activo — para resaltar enemigos en rango */
		dx       = pos_numen.pos_x - pos_active.pos_x;
		dy       = pos_numen.pos_y - pos_active.pos_y;
		dist_sq  = dx * dx + dy * dy;
		in_range = (is_corrupt == TRUE
		            && pos_active.pos_x != NO_POS
		            && dist_sq <= range_sq);

		/* Marco de alerta: rojo si en rango, naranja si errant fuera de rango */
		if (in_range)
			DrawRectangleLines (pos_numen.pos_x - 2, pos_numen.pos_y - 2,
			                    SCALE + 4, SCALE + 4, RED);
		else if (is_errant == TRUE && is_corrupt == FALSE)
			DrawRectangleLines (pos_numen.pos_x - 1, pos_numen.pos_y - 1,
			                    SCALE + 2, SCALE + 2, GREEN);





		/* === SPRITE / FALLBACK === */
		tex = ge_get_numen_texture (ge, numen_get_name (num));

		if (ge_texture_is_valid (tex) == TRUE)
		{
			Rectangle src = { 0, 0, (float)tex->width, (float)tex->height };
			Rectangle dst = { (float)pos_numen.pos_x, (float)pos_numen.pos_y, (float)(SCALE), (float)(SCALE) };
			DrawTexturePro (*tex, src, dst, (Vector2){ 0, 0 }, 0.0f, WHITE);

		}
		else
		{
			col = (is_corrupt == TRUE)
			    ? (Color){ 200, 60, 60, 255 }
			    : (Color){ 80, 200, 80, 255 };

			DrawCircle (pos_numen.pos_x + SCALE, pos_numen.pos_y + SCALE, SCALE - 2, col);
			DrawCircleLines (pos_numen.pos_x + SCALE, pos_numen.pos_y + SCALE, SCALE - 2, BLACK);

			gd = numen_get_gdesc (num);
			if (gd && gd[0])	DrawText (TextFormat ("%c", gd[0]),
				    											pos_numen.pos_x + SCALE - 4, 
																pos_numen.pos_y + SCALE - 6, 14, BLACK);


		}

		/*	=====BARRAS DE VIDA Y ENERGIA Del numen ===== */
			_ge_draw_bars (num); 
		/*	=================================== */
	}
		/*	=====BARRAS DE VIDA Y ENERGIA  DEL NUMEN ACTIVE===== */
			_ge_draw_bars (active_num);
		/*	=================================== */
}

static void
_ge_draw_effect_attack (Numen* numen, float opacity)
{
	Position position;
	int      max_radio = 0;
	int      cx, cy;

	if (!numen) return;

	position = numen_get_position (numen);
	if (types_position_is_valid (numen, (Position (*)(void *)) numen_get_position,
	                             0, 0, WIDHT_MAP, HIGHT_MAP) == FALSE)
		return;

	max_radio = _ge_max_radio_skill_of_numen (numen);
	if (max_radio <= 0) max_radio = SCALE * 3;

	cx = position.pos_x + SCALE / 2;
	cy = position.pos_y + SCALE / 2;

	/* Círculo relleno tenue + anillo sólido para mostrar el radio de ataque */
	DrawCircle      (cx, cy, (float)max_radio, Fade (ORANGE, opacity * 0.25f));
	DrawCircleLines (cx, cy, (float)max_radio, Fade (RED,    opacity));
}


static void		_ge_draw_bars (Numen* numen)
{
	int 	hp, max_life = MAX_LIFE;
	int 	energy, max_energy = MAX_ENGY;
	float	hp_ratio, eng_ratio;
	Color	color_bar_life   = GREEN;
	Color	color_bar_energy = YELLOW;
	Position	position;
	if (!numen) return;

	position.pos_x = NO_POS;
	position.pos_y = NO_POS;
	/*Características de los numnes corruptos*/
	if ( numen_get_corrupt (numen) == TRUE)
	{
		max_life = MAX_LIFE_CORRUPT;
		max_energy = MAX_ENGY_CORRUPT;
		color_bar_life = RED;
	}

	position = numen_get_position (numen);

	/*Cuanto de la barra  de vida hay llena?*/
	hp = numen_get_health (numen);
	hp_ratio = (hp > MIN_LIFE)
	         ? (float)hp / (float)max_life
	         : 0.0f;
	if (hp_ratio < 0.0f) hp_ratio = 0.0f;
	if (hp_ratio > 1.0f) hp_ratio = 1.0f;




	/*Cuanto de la barra  de energia hay llena?*/
	energy = numen_get_energy (numen);
	eng_ratio = (energy > MIN_ENGY)
	         	? (float)energy / (float)max_energy
	         	: 0.0f;
	if (eng_ratio < 0.0f) eng_ratio = 0.0f;
	if (eng_ratio > 1.0f) eng_ratio = 1.0f;




	/*================ DIBUJO DE LA BARRA DE VIDA =====================*/
		/* Fondo gris de la barra */
		DrawRectangle (position.pos_x, position.pos_y - 20, 
								SCALE, 4,
		               (Color){ 60, 60, 60, 200 });
		/* Relleno rojo proporcional */
		DrawRectangle (position.pos_x, position.pos_y - 20,
		               (int)(SCALE * hp_ratio), 4, /*<-----  RECORDAR PROBAR (SCALE * hp_ratio)*/
		               color_bar_life);
		/* Borde negro fino */
		DrawRectangleLines (position.pos_x, position.pos_y - 14, SCALE, 4, BLACK);
	/*===========================================================================*/




	/*================ DIBUJO DE LA BARRA DE ENERGIA =====================*/
		/* Fondo gris de la barra */
		DrawRectangle (position.pos_x, position.pos_y - 10, 
					   			SCALE, 4,
		               (Color){ 60, 60, 60, 200 });
		/* Relleno rojo proporcional */
		DrawRectangle (position.pos_x, position.pos_y - 10,
		               (int)(SCALE * eng_ratio ), 4, /*<-----  RECORDAR PROBAR (SCALE * energia_ratio)*/
		               color_bar_energy);
		/* Borde negro fino */
		DrawRectangleLines (position.pos_x, position.pos_y - 4, SCALE, 4, BLACK);
	/*===========================================================================*/




	return;
}


static int	_ge_max_radio_skill_of_numen (Numen* numen)
{
	Skills_id	skills[NUM_SKILLS];
	int	i, max_radio;

	if (!numen) return -1;

	/*Inicializamos todos los posibles skills para visualizar cada uno*/
	for (i = 0; i < NUM_SKILLS; i++)	{skills[i] = numen_get_skill_by_index(numen, i);}

	/*Buscamos el skill con el radio más grande*/
	for (i = 0, max_radio = skill_get_radio (skills[i]) ; i < NUM_SKILLS; i++)
	{
		if (max_radio <  skill_get_radio (skills[i])) max_radio =  skill_get_radio (skills[i]);
	}

	return max_radio;
}

/* ====================================================================== */
/*               PUBLIC: GAME-OVER / VICTORY SCREEN                        */
/*                                                                         */
/* Muestra una pantalla final con resultado (victoria o derrota) y música. */
/* Espera a que el jugador pulse ENTER, ESC o cierre la ventana.           */
/* ====================================================================== */
void
graphic_engine_game_over (Graphic_engine* ge, Game* game)
{
	/* Detectamos si fue victoria o derrota comparando las condiciones de juego */
	Bool won;
	const char* headline;
	const char* sub;
	Color       headline_color;
	int         hl_w, sub_w;

	if (!ge || !IsWindowReady ()) return;

	won = game_get_last_cmd_status (game) != ERROR;
	/* Usamos la condición guardada en el juego: si el jugador tiene numens vivos = victoria */
	{
		Player* p = game ? game_get_player (game) : NULL;
		won = (p && player_get_n_numens (p) > 0);
	}

	headline       = won ? "VICTORIA"   : "GAME OVER";
	sub            = won ? "Pulsa ENTER para salir"
	                     : "Pulsa ENTER para salir";
	headline_color = won ? GOLD : RED;

	/* Música del game-over
	 * El archivo "Judas" debe estar en ./msc_src/OST/Judas.mp3
	 * Si no existe, raylib carga silencio y el juego sigue sin crashear. */
	ge->gameover_music        = LoadMusicStream ("./msc_src/OST/the_final_countdown_musica_epica_boss.mp3");
	ge->gameover_music_loaded = TRUE;
	SetMusicVolume (ge->gameover_music, 0.6f);
	PlayMusicStream (ge->gameover_music);

	while (!WindowShouldClose ()
	       && !IsKeyPressed (KEY_ENTER)
	       && !IsKeyPressed (KEY_ESCAPE))
	{
		UpdateMusicStream (ge->gameover_music);

		BeginDrawing ();
		ClearBackground (BLACK);

		/* Fondo semi-transparente oscuro sobre todo */
		DrawRectangle (0, 0, WIDTH_SCREEN, HIGHT_SCREEN, (Color){ 0, 0, 0, 200 });

		/* Título */
		hl_w = MeasureText (headline, 60);
		DrawText (headline,
		          WIDTH_SCREEN / 2 - hl_w / 2,
		          HIGHT_SCREEN / 2 - 60,
		          60, headline_color);

		/* Subtítulo */
		sub_w = MeasureText (sub, MEDIUM_TEXT_SIZE);
		DrawText (sub,
		          WIDTH_SCREEN / 2 - sub_w / 2,
		          HIGHT_SCREEN / 2 + 20,
		          MEDIUM_TEXT_SIZE, RAYWHITE);

		/* Parpadeo "ENTER" */
		if (((int)(GetTime () * 2)) % 2 == 0)
		{
			int enter_w = MeasureText ("[ ENTER ]", BIG_TEXT_SIZE);
			DrawText ("[ ENTER ]",
			          WIDTH_SCREEN / 2 - enter_w / 2,
			          HIGHT_SCREEN / 2 + 60,
			          BIG_TEXT_SIZE, GRAY);
		}

		EndDrawing ();
	}

	StopMusicStream (ge->gameover_music);
	UnloadMusicStream (ge->gameover_music);
	ge->gameover_music_loaded = FALSE;
}