/**
 * @brief It reads information from files to create the game and saves it when
 * required
 *
 * @file game_management.c
 * @author Violeta, Rafael, Salvador and Javier Jarque
 * @version 4
 * @date 03-05-2026
 * @copyright GNU Public License
 */

#include "game_management.h"
#include "links.h"
#include "skills.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_PLAY  "#p:"
#define F_SPAC  "#s:"
#define F_OBJE  "#o:"
#define F_CHAR  "#c:"
#define F_LINK  "#l:"
#define F_NUMEN "#n:"

static Status game_load_spaces     (Game* game, char* filename);
static Status game_load_objects    (Game* game, char* filename);
static Status game_load_characters (Game* game, char* filename);
static Status game_load_players    (Game* game, char* filename);
static Status game_load_numens     (Game* game, char* filename);
static Status game_load_links      (Game* game, char* filename);

/* ========================================================================= */
/*                            LOAD SPACES                                    */
/* ========================================================================= */
static Status
game_load_spaces (Game* game, char* filename)
{
    FILE*  file              = NULL;
    char   line[WORD_SIZE]   = "";
    char   name[WORD_SIZE]   = "";
    char   gdesc[WORD_SIZE]  = "";
    char   OST[WORD_SIZE]    = "";
    char*  toks              = NULL;
    Id     id                = NO_ID;
    Space* space             = NULL;
    Status status            = OK;
    int    tam_format;
    int    *l;
    int    i, loop;
    Bool   grid_ok;

    if (!game || !filename) return ERROR;

    file = fopen (filename, "r");
    if (!file) return ERROR;

    tam_format = strlen (F_SPAC);

    while (fgets (line, WORD_SIZE, file))
    {
        if (strncmp (F_SPAC, line, tam_format) != 0) continue;

        /* --- id --- */
        toks = strtok (line + tam_format, "|");
        if (!toks) continue;
        id = atol (toks);

        /* --- name --- */
        toks = strtok (NULL, "|");
        if (!toks) continue;
        strcpy (name, toks);

        /* --- gdesc --- */
        toks = strtok (NULL, "|");
        if (!toks || strcmp (toks, "none") == 0)
            strcpy (gdesc, "./img_src/background/default.jpg");
        else
            strcpy (gdesc, toks);

        /* --- OST --- */
        toks = strtok (NULL, "|");
        if (toks && strcmp (toks, "none") != 0)
            strcpy (OST, toks);
        else
            OST[0] = '\0';

#ifdef DEBUG
        printf ("Leido space: s:%ld|%s\n", id, name);
#endif

        space = space_create ();
        if (!space) continue;

        space_set_id    (space, id);
        space_set_name  (space, name);
        space_set_gdesc (space, gdesc);
        if (OST[0] != '\0') space_set_ost (space, OST);

        /* --------- GRID: 25 lineas, una por strtok --------- */
        grid_ok = TRUE;
        for (i = 0; i < HIGHT; i++)
        {
            toks = strtok (NULL, "|");
            if (!toks || strlen (toks) < (size_t)WIDHT) { grid_ok = FALSE; break; }

            l = (int*)malloc (WIDHT * sizeof (int));
            if (!l) { grid_ok = FALSE; break; }

            for (loop = 0; loop < WIDHT; loop++)
                l[loop] = (int)toks[loop] % GRID_MODULE;

            /* space TOMA PROPIEDAD de l (no copia) */
            space_set_grid_by_line (space, i, l);
        }

        if (grid_ok == TRUE)
            game_add_space (game, space);
        else
        {
            space_destroy (space);
            status = ERROR;
        }
    }

    if (ferror (file)) status = ERROR;
    fclose (file);
    return status;
}

/* ========================================================================= */
/*                            LOAD OBJECTS                                   */
/* ========================================================================= */
static Status
game_load_objects (Game* game, char* filename)
{
    FILE*   file                = NULL;
    char    line[WORD_SIZE]     = "";
    char    name[WORD_SIZE]     = "";
    char    description[WORD_SIZE] = "";
    char    gdesc[WORD_SIZE]    = "";
    char*   toks                = NULL;
    Id      id = NO_ID, space_id = NO_ID, dependency = NO_ID, active_obj_id = NO_ID;
    Object* obj    = NULL;
    Space*  space  = NULL;
    Player* players_game[MAX_PLAYERS];
    Status  status = OK;
    Bool    consumable, movable;
    Effect  effect;
    int     tam_format, health, attack, energy, speed, pos_x, pos_y;
    int     n_players, bucle;

    if (!game || !filename) return ERROR;

    /* Inicializamos players_game[] UNA VEZ y antes del while.
     * Asi el bucle final que limpia active_obj_id siempre tiene
     * datos validos. */
    n_players = game_get_n_players (game);
    for (bucle = 0; bucle < n_players && bucle < MAX_PLAYERS; bucle++)
        players_game[bucle] = game_get_player_at (game, bucle);

    tam_format = strlen (F_OBJE);
    file = fopen (filename, "r");
    if (!file) return ERROR;

    while (fgets (line, WORD_SIZE, file))
    {
        if (strncmp (F_OBJE, line, tam_format) != 0) continue;

        /* id */
        toks = strtok (line + tam_format, "|"); if (!toks) continue;
        id = atol (toks);
        /* name */
        toks = strtok (NULL, "|"); if (!toks) continue;
        strcpy (name, toks);
        /* space_id */
        toks = strtok (NULL, "|"); if (!toks) continue;
        space_id = atol (toks);
        /* pos_x */
        toks = strtok (NULL, "|"); if (!toks) continue;
        pos_x = atoi (toks);
        /* pos_y */
        toks = strtok (NULL, "|"); if (!toks) continue;
        pos_y = atoi (toks);
        /* description */
        toks = strtok (NULL, "|");
        if (toks) strcpy (description, toks);
        else      description[0] = '\0';
        /* gdesc */
        toks = strtok (NULL, "|"); if (!toks) continue;
        strcpy (gdesc, toks);
        /* consumable */
        toks = strtok (NULL, "|"); if (!toks) continue;
        consumable = atoi (toks) ? TRUE : FALSE;
        /* health */
        toks = strtok (NULL, "|"); if (!toks) continue;
        health = atoi (toks);
        /* attack */
        toks = strtok (NULL, "|"); if (!toks) continue;
        attack = atoi (toks);
        /* energy */
        toks = strtok (NULL, "|"); if (!toks) continue;
        energy = atoi (toks);
        /* speed */
        toks = strtok (NULL, "|"); if (!toks) continue;
        speed = atoi (toks);
        /* dependency */
        toks = strtok (NULL, "|");
        dependency = (toks) ? atol (toks) : NO_ID;
        /* movable */
        toks = strtok (NULL, "|"); if (!toks) continue;
        movable = atoi (toks) ? TRUE : FALSE;
        /* effect */
        toks = strtok (NULL, "|\n"); if (!toks) continue;
        effect = atoi (toks);
        if (effect < NO_EFECT || effect > MAX_EFFECT) effect = NO_EFECT;

#ifdef DEBUG
        printf ("Leido object: o:%ld|%s|%ld|%s\n", id, name, space_id, description);
#endif

        obj = obj_create ();
        if (!obj) continue;

        obj_set_id          (obj, id);
        obj_set_name        (obj, name);
        obj_set_gdesc       (obj, gdesc);
        obj_set_position    (obj, pos_x, pos_y);
        obj_set_effect      (obj, effect);
        if (description[0] != '\0') obj_set_description (obj, description);
        obj_set_health      (obj, health);
        obj_set_attack      (obj, attack);
        obj_set_energy      (obj, energy);
        obj_set_speed       (obj, speed);
        obj_set_consumable  (obj, consumable);
        obj_set_movable     (obj, movable);
        obj_set_dependency  (obj, dependency);

        game_add_object (game, obj);

        /* Colocamos el objeto: primero intentamos en su space.
         * Si el space_id no existe, asumimos que se referia a un
         * player_id (el objeto arranca en el inventario de alguien). */
        space = game_get_space (game, space_id);
        if (space)
            space_set_object (space, id, obj_get_position (obj));
        else
            for (bucle = 0; bucle < n_players; bucle++)
                if (players_game[bucle] &&
                    player_get_id (players_game[bucle]) == space_id)
                {
                    player_add_object (players_game[bucle], id);
                    break;
                }
    }

    /* Validar que cada player tiene su active_object en la mochila;
     * si no, lo deseleccionamos. */
    for (bucle = 0; bucle < n_players; bucle++)
    {
        if (!players_game[bucle]) continue;
        active_obj_id = player_get_active_object (players_game[bucle]);
        if (active_obj_id != NO_ID &&
            player_contains_object (players_game[bucle], active_obj_id) == FALSE)
            player_set_active_object (players_game[bucle], NO_ID);
    }

    if (ferror (file)) status = ERROR;
    fclose (file);
    return status;
}

/* ========================================================================= */
/*                          LOAD CHARACTERS                                  */
/* ========================================================================= */
static Status
game_load_characters (Game* game, char* filename)
{
    FILE*       file              = NULL;
    char        line[WORD_SIZE]   = "";
    char        name[WORD_SIZE]   = "";
    char        gdesc[WORD_SIZE]  = "";
    char        message[WORD_SIZE]= "";
    char*       toks              = NULL;
    Character*  ch                = NULL;
    Space*      space             = NULL;
    Id          id = NO_ID, space_id = NO_ID;
    int         health = 0, friendly = 1;
    Status      status = OK;
    int         tam_format;

    if (!game || !filename) return ERROR;

    tam_format = strlen (F_CHAR);
    file = fopen (filename, "r");
    if (!file) return ERROR;

    while (fgets (line, WORD_SIZE, file))
    {
        if (strncmp (F_CHAR, line, tam_format) != 0) continue;

        toks = strtok (line + tam_format, "|"); if (!toks) continue; id = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; strcpy (name, toks);
        toks = strtok (NULL, "|"); if (!toks) continue; strcpy (gdesc, toks);
        toks = strtok (NULL, "|"); if (!toks) continue; space_id = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; health = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; friendly = atoi (toks);
        toks = strtok (NULL, "\n");
        if (toks) strcpy (message, toks); else message[0] = '\0';

#ifdef DEBUG
        printf ("Leido character: c:%ld|%s|%s|%ld|%d|%d|%s\n",
                id, name, gdesc, space_id, health, friendly, message);
#endif

        ch = character_create ();
        if (!ch) continue;

        character_set_id       (ch, id);
        character_set_name     (ch, name);
        character_set_gdesc    (ch, gdesc);
        character_set_health   (ch, health);
        character_set_friendly (ch, friendly ? TRUE : FALSE);
        character_set_message  (ch, message);

        game_add_character (game, ch);

        space = game_get_space (game, space_id);
        if (space) space_set_character (space, id);
    }

    if (ferror (file)) status = ERROR;
    fclose (file);
    return status;
}

/* ========================================================================= */
/*                            LOAD PLAYERS                                   */
/* ========================================================================= */
static Status
game_load_players (Game* game, char* filename)
{
    FILE*   file              = NULL;
    char    line[WORD_SIZE]   = "";
    char    name[WORD_SIZE]   = "";
    char    gdesc[WORD_SIZE]  = "";
    char*   toks              = NULL;
    Player* player            = NULL;
    Space*  space             = NULL;
    Id      id              = NO_ID;
    Id      active_num_id   = NO_ID;
    Id      active_obj_id   = NO_ID;
    Id      zone            = NO_ID;
    int     max_numens      = 0;
    int     max_bag         = 0;
    Status  status          = OK;
    int     tam_format, pos_x = 0, pos_y = 0, vision_x = 0, vision_y = 0;

    if (!game || !filename) return ERROR;

    tam_format = strlen (F_PLAY);
    file = fopen (filename, "r");
    if (!file) return ERROR;

    while (fgets (line, WORD_SIZE, file))
    {
        if (strncmp (F_PLAY, line, tam_format) != 0) continue;

        toks = strtok (line + tam_format, "|"); if (!toks) continue; id = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; strcpy (name, toks);
        toks = strtok (NULL, "|"); if (!toks) continue; zone = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; pos_x = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; pos_y = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; vision_x = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; vision_y = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; strcpy (gdesc, toks);
        toks = strtok (NULL, "|"); if (!toks) continue; max_bag = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; max_numens = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; active_num_id = atol (toks);
        toks = strtok (NULL, "|\n");
        active_obj_id = (toks) ? atol (toks) : NO_ID;

#ifdef DEBUG
        printf ("Leido player: p:%ld|%s|%s|%ld|%d|%d\n",
                id, name, gdesc, zone, max_numens, max_bag);
#endif

        player = player_create ();
        if (!player) continue;

        if (player_set_id          (player, id)               == ERROR ||
            player_set_name        (player, name)             == ERROR ||
            player_set_gdesc       (player, gdesc)            == ERROR ||
            player_set_zone        (player, zone)             == ERROR ||
            player_set_position    (player, pos_x, pos_y)     == ERROR ||
            player_set_max_numens  (player, max_numens)       == ERROR ||
            player_set_max_objects (player, max_bag)          == ERROR ||
            player_set_vision      (player, vision_x, vision_y) == ERROR)
        {
            player_destroy (player);
            fclose (file);
            return ERROR;
        }

        /* loading=TRUE permite asignar active_numen / active_object aunque
         * todavia no esten en la mochila (al cargar el .dat los inventarios
         * se llenan despues). */
        player_set_loading       (player, TRUE);
        player_set_active_numen  (player, active_num_id);
        if (active_obj_id != NO_ID)
            player_set_active_object (player, active_obj_id);
        player_set_loading       (player, FALSE);

        game_add_player (game, player);

        space = game_get_space (game, zone);
        if (space) space_set_discovered (space, TRUE);
    }

    if (ferror (file)) status = ERROR;
    fclose (file);
    return status;
}

/* ========================================================================= */
/*                            LOAD NUMENS                                    */
/* ========================================================================= */
static Status
game_load_numens (Game* game, char* filename)
{
    FILE*   file              = NULL;
    char    line[WORD_SIZE]   = "";
    char    name[WORD_SIZE]   = "";
    char    gdesc[WORD_SIZE]  = "";
    char*   toks              = NULL;
    Numen*  numen             = NULL;
    Space*  space             = NULL;
    Player* players_game[MAX_PLAYERS];
    Player* owner             = NULL;
    Id      id              = NO_ID;
    Id      space_id        = NO_ID;
    Id      following       = NO_ID;
    Id      active_numen_id = NO_ID;
    int     health = 0, attack = 0, energy = 0, speed = 0, pos_x, pos_y;
    Id      skills[N_SKILLS] = { NO_ID, NO_ID, NO_ID, NO_ID };
    Status  status   = OK;
    int     tam_format, n_players, bucle, i;

    if (!game || !filename) return ERROR;

    n_players = game_get_n_players (game);
    for (bucle = 0; bucle < n_players && bucle < MAX_PLAYERS; bucle++)
        players_game[bucle] = game_get_player_at (game, bucle);

    tam_format = strlen (F_NUMEN);
    file = fopen (filename, "r");
    if (!file) return ERROR;

    while (fgets (line, WORD_SIZE, file))
    {
        if (strncmp (F_NUMEN, line, tam_format) != 0) continue;

        toks = strtok (line + tam_format, "|"); if (!toks) continue; id = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; strcpy (name, toks);
        toks = strtok (NULL, "|"); if (!toks) continue; space_id = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; pos_x = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; pos_y = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; strcpy (gdesc, toks);
        toks = strtok (NULL, "|"); if (!toks) continue; health = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; energy = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; attack = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; speed  = atoi (toks);

        for (i = 0; i < N_SKILLS; i++)
        {
            toks = strtok (NULL, "|");
            skills[i] = (toks) ? atol (toks) : NO_ID;
        }

        toks = strtok (NULL, "|\n");
        following = (toks) ? atol (toks) : NO_ID;

#ifdef DEBUG
        printf ("Leido numen: n:%ld|%s|%ld|%d|%d\n", id, name, space_id, pos_x, pos_y);
#endif

        numen = numen_create ();
        if (!numen) continue;

        numen_set_id     (numen, id);
        numen_set_pos_x  (numen, pos_x);
        numen_set_pos_y  (numen, pos_y);
        numen_set_name   (numen, name);
        numen_set_gdesc  (numen, gdesc);
        numen_set_health (numen, health);
        numen_set_energy (numen, energy);
        numen_set_attack (numen, attack);
        numen_set_speed  (numen, speed);

        for (i = 0; i < N_SKILLS; i++)
            if (skills[i] != NO_ID) numen_add_skill (numen, skills[i]);

        numen_set_following (numen, following);

        /* Si tiene following y el following es un player existente,
         * el numen entra en su backpack_numens. */
        if (following != NO_ID)
        {
            owner = game_get_player_by_id (game, following);
            if (owner) player_add_numen (owner, id);
        }

        game_add_numen (game, numen);

        /* Colocamos el numen en su space (o en el inventario del player si
         * el "space_id" referenciaba a un player_id). */
        space = game_get_space (game, space_id);
        if (space)
            space_set_numen (space, id, numen_get_position (numen));
        else
            for (bucle = 0; bucle < n_players; bucle++)
                if (players_game[bucle] &&
                    player_get_id (players_game[bucle]) == space_id)
                {
                    player_add_numen (players_game[bucle], id);
                    break;
                }
    }

    /* Cada player debe tener al menos un numen y un active_numen valido. */
    for (bucle = 0; bucle < n_players; bucle++)
    {
        if (!players_game[bucle]) continue;

        active_numen_id = player_get_active_numen (players_game[bucle]);
        if (active_numen_id != NO_ID &&
            player_contains_numen (players_game[bucle], active_numen_id) == FALSE)
        {
            player_set_loading (players_game[bucle], TRUE);
            player_set_active_numen (players_game[bucle], NO_ID);
            player_set_loading (players_game[bucle], FALSE);
        }
    }
    if (ferror (file)) status = ERROR;
    fclose (file);
    return status;
}

/* ========================================================================= */
/*                             LOAD LINKS                                    */
/* ========================================================================= */
static Status
game_load_links (Game* game, char* filename)
{
    FILE*  file              = NULL;
    char   line[WORD_SIZE]   = "";
    char   name[WORD_SIZE]   = "";
    char*  toks              = NULL;
    Links* link              = NULL;
    Id     id                = NO_ID;
    Id     origin = NO_ID, dest = NO_ID;
    int    dir_num   = 0;
    int    open_otd  = 1;
    int    open_dto  = 1;
    Status status    = OK;
    int    tam_format;

    if (!game || !filename) return ERROR;

    tam_format = strlen (F_LINK);
    file = fopen (filename, "r");
    if (!file) return ERROR;

    while (fgets (line, WORD_SIZE, file))
    {
        if (strncmp (F_LINK, line, tam_format) != 0) continue;

        toks = strtok (line + tam_format, "|"); if (!toks) continue; id = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; strcpy (name, toks);
        toks = strtok (NULL, "|"); if (!toks) continue; origin = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; dest = atol (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; dir_num = atoi (toks);
        toks = strtok (NULL, "|"); if (!toks) continue; open_otd = atoi (toks);
        toks = strtok (NULL, "|\n");
        open_dto = (toks) ? atoi (toks) : open_otd;

#ifdef DEBUG
        printf ("Leido link: l:%ld|%s|%ld|%ld|%d|%d|%d\n",
                id, name, origin, dest, dir_num, open_otd, open_dto);
#endif

        link = link_create ();
        if (!link) continue;

        link_set_id              (link, id);
        link_set_name            (link, name);
        link_set_origin_id       (link, origin);
        link_set_destination_id  (link, dest);
        link_set_direction       (link, (Direction)dir_num);
        link_set_open_origin_to_dest (link, open_otd ? TRUE : FALSE);
        link_set_open_dest_to_origin (link, open_dto ? TRUE : FALSE);

        game_add_links (game, link);
    }

    if (ferror (file)) status = ERROR;
    fclose (file);
    return status;
}

/* ========================================================================= */
/*                  PUBLIC: CREATE GAME FROM FILE                            */
/* ========================================================================= */
Status
game_management_create_from_file (Game** game, char* filename)
{
    if (!game || !filename) return ERROR;

    *game = game_create ();
    if (*game == NULL) return ERROR;

    if (game_load_spaces (*game, filename) == ERROR) {
        fprintf (stderr, "FAIL: spaces\n");
        game_destroy (*game); *game = NULL; return ERROR;
    }
    fprintf (stderr, "OK: spaces (%d cargados)\n", game_get_n_spaces (*game));

    if (game_load_players (*game, filename) == ERROR) {
        fprintf (stderr, "FAIL: players\n");
        game_destroy (*game); *game = NULL; return ERROR;
    }
    fprintf (stderr, "OK: players (%d cargados)\n", game_get_n_players (*game));

    if (game_load_objects (*game, filename) == ERROR) {
        fprintf (stderr, "FAIL: objects\n");
        game_destroy (*game); *game = NULL; return ERROR;
    }
    fprintf (stderr, "OK: objects (%d cargados)\n", game_get_n_objects (*game));

    if (game_load_characters (*game, filename) == ERROR) {
        fprintf (stderr, "FAIL: characters\n");
        game_destroy (*game); *game = NULL; return ERROR;
    }
    fprintf (stderr, "OK: characters\n");

    if (game_load_links (*game, filename) == ERROR) {
        fprintf (stderr, "FAIL: links\n");
        game_destroy (*game); *game = NULL; return ERROR;
    }
    fprintf (stderr, "OK: links\n");

    if (game_load_numens (*game, filename) == ERROR) {
        fprintf (stderr, "FAIL: numens\n");
        game_destroy (*game); *game = NULL; return ERROR;
    }
    fprintf (stderr, "OK: numens\n");

    return OK;
}

/* ========================================================================= */
/*                  PUBLIC: SAVE GAME TO FILE                                */
/* ========================================================================= */
Status
game_management_save_file (Game** game)
{
    Space*    space   = NULL;
    Object*   object  = NULL;
    Player*   player  = NULL;
    Player*   players_game[MAX_PLAYERS];
    Numen*    numen   = NULL;
    Links*    link    = NULL;
    Direction dir     = U;
    Effect    effect  = NO_EFECT;
    Id        id          = NO_ID;
    Id        zone        = NO_ID;
    Id        origin      = NO_ID;
    Id        dest        = NO_ID;
    Id        dependency  = NO_ID;
    Id        following   = NO_ID;
    Skills_id skills[N_SKILLS];
    int       check, bucle, bucle2;
    int       n_play  = 0;
    int       pos_x   = 0;
    int       pos_y   = 0;
    int       health  = 0;
    int       attack  = 0;
    int       energy  = 0;
    int       speed   = 0;
    int       max_bag = 0;
    int       max_numens = 0;
    int*      grid_line;
    Bool      open_otd, open_dto, consumable, movable;
    char*     name      = NULL;
    char*     gdesc     = NULL;
    char*     OST       = NULL;
    char*     message   = NULL;
    FILE*     new_sfile = NULL;

    if (!game || !*game) return ERROR;

    new_sfile = fopen ("./Save_old_games/savefile.dat", "w");
    if (!new_sfile) return ERROR;

    /* ============================== SPACES ============================== */
    check = game_get_n_spaces (*game);
    for (bucle = 0; bucle < check; bucle++)
    {
        id    = game_get_space_id_at (*game, bucle);
        space = game_get_space       (*game, id);
        if (!space) continue;

        name  = space_get_name  (space);
        gdesc = space_get_gdesc (space);
        OST   = space_get_ost   (space);

        fprintf (new_sfile, "#s:%ld|%s|%s|%s",
                 id,
                 name  ? name  : "",
                 gdesc ? gdesc : "none",
                 OST   ? OST   : "none");

        /* Cada fila del grid es WIDHT enteros: los serializamos juntos
         * separados por '|'. Usamos bucle2 para no pisar 'bucle'. */
        for (bucle2 = 0; bucle2 < HIGHT; bucle2++)
        {
            int col;
            grid_line = space_get_grid_by_line (space, bucle2);
            fprintf (new_sfile, "|");
            if (grid_line)
                for (col = 0; col < WIDHT; col++)
                    fprintf (new_sfile, "%d", grid_line[col]);
        }
        fprintf (new_sfile, "|\n");
    }

    /* ============================== PLAYERS ============================== */
    check = game_get_n_players (*game);
    n_play = 0;
    for (bucle = 0; bucle < check && bucle < MAX_PLAYERS; bucle++)
    {
        player = game_get_player_at (*game, bucle);
        if (!player) continue;
        players_game[n_play++] = player;

        id         = player_get_id          (player);
        name       = player_get_name        (player);
        zone       = player_get_zone        (player);
        pos_x      = player_get_pos_x       (player);
        pos_y      = player_get_pos_y       (player);
        gdesc      = player_get_gdesc       (player);
        max_bag    = player_get_max_objects (player);
        max_numens = player_get_max_numens  (player);

        fprintf (new_sfile, "#p:%ld|%s|%ld|%d|%d|%s|%d|%d|\n",
                 id,
                 name  ? name  : "",
                 zone, pos_x, pos_y,
                 gdesc ? gdesc : "",
                 max_bag, max_numens);
    }

    /* ============================== OBJECTS ============================== */
    check = game_get_n_objects (*game);
    for (bucle = 0; bucle < check; bucle++)
    {
        object = game_get_object_at (*game, bucle);
        if (!object) continue;

        id         = obj_get_id          (object);
        name       = obj_get_name        (object);
        zone       = game_get_object_location (*game, id);

        /* Si zone es NO_ID, el objeto vive en la mochila de algun player.
         * Buscamos ese player y guardamos su id como zone. */
        if (zone == NO_ID)
            for (bucle2 = 0; bucle2 < n_play; bucle2++)
                if (player_contains_object (players_game[bucle2], id) == TRUE)
                {
                    zone = player_get_id (players_game[bucle2]);
                    break;
                }

        pos_x      = obj_get_pos_x       (object);
        pos_y      = obj_get_pos_y       (object);
        message    = obj_get_description (object);
        gdesc      = obj_get_gdesc       (object);
        health     = obj_get_health      (object);
        attack     = obj_get_attack      (object);
        energy     = obj_get_energy      (object);
        speed      = obj_get_speed       (object);
        movable    = obj_get_movable     (object);
        dependency = obj_get_dependency  (object);
        consumable = obj_get_consumable  (object);
        effect     = obj_get_effect      (object);

        fprintf (new_sfile,
                 "#o:%ld|%s|%ld|%d|%d|%s|%s|%d|%d|%d|%d|%d|%ld|%d|%d|\n",
                 id,
                 name    ? name    : "",
                 zone, pos_x, pos_y,
                 message ? message : "",
                 gdesc   ? gdesc   : "",
                 consumable == TRUE ? 1 : 0,
                 health, attack, energy, speed,
                 dependency,
                 movable    == TRUE ? 1 : 0,
                 (int)effect);
    }

    /* ============================== NUMENS ============================== */
    check = game_get_n_numens (*game);
    for (bucle = 0; bucle < check; bucle++)
    {
        numen = game_get_numen_at (*game, bucle);
        if (!numen) continue;

        id     = numen_get_id     (numen);
        name   = numen_get_name   (numen);
        zone   = game_get_numen_location (*game, id);
        pos_x  = numen_get_pos_x  (numen);
        pos_y  = numen_get_pos_y  (numen);
        gdesc  = numen_get_gdesc  (numen);
        health = numen_get_health (numen);
        attack = numen_get_attack (numen);
        energy = numen_get_energy (numen);
        speed  = numen_get_speed  (numen);

        for (bucle2 = 0; bucle2 < N_SKILLS; bucle2++)
            skills[bucle2] = numen_get_skill_by_index (numen, bucle2);
        following = numen_get_following (numen);

        fprintf (new_sfile,
                 "#n:%ld|%s|%ld|%d|%d|%s|%d|%d|%d|%d|%d|%d|%d|%d|%ld|\n",
                 id,
                 name  ? name  : "",
                 zone, pos_x, pos_y,
                 gdesc ? gdesc : "",
                 health, energy, attack, speed,
                 (int)skills[0], (int)skills[1],
                 (int)skills[2], (int)skills[3],
                 following);
    }

    /* ============================== LINKS ============================== */
    check = game_get_n_links (*game);
    for (bucle = 0; bucle < check; bucle++)
    {
        link = game_get_link_at (*game, bucle);
        if (!link) continue;

        id       = link_get_id                  (link);
        name     = link_get_name                (link);
        origin   = link_get_origin_id           (link);
        dest     = link_get_destination_id      (link);
        dir      = link_get_direction           (link);
        open_otd = link_get_open_origin_to_dest (link);
        open_dto = link_get_open_dest_to_origin (link);

        fprintf (new_sfile, "#l:%ld|%s|%ld|%ld|%d|%d|%d|\n",
                 id,
                 name ? name : "",
                 origin, dest, (int)dir,
                 open_otd == TRUE ? 1 : 0,
                 open_dto == TRUE ? 1 : 0);
    }

    fclose (new_sfile);
    return OK;
}