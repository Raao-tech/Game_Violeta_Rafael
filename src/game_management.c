/**
 * @brief It reads information from files to create the game and saves it when
 * required
 *
 * @file game_management.c
 * @author Violeta, Rafael, Salvador and Javier Jarque
 * @version 3
 * @date 20-04-2026
 * @copyright GNU Public License
 */

#include "game_management.h"
#include "links.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Prefix tags used to identify each entity type in the .dat file.
 *
 * Every line in the .dat file begins with one of these prefixes,
 * followed by the fields of that entity separated by '|'.
 */
#define F_PLAY "#p:"
#define F_SPAC "#s:"
#define F_OBJE "#o:"
#define F_CHAR "#c:"
#define F_LINK "#l:"
#define F_NUMEN "#n:"

/**
 * Private function prototypes — one loader per entity type.
 * Each function opens the file, scans for its prefix, parses the
 * fields, builds the TAD, and registers it in the Game.
 */
static Status game_load_spaces (Game* game, char* filename);
static Status game_load_objects (Game* game, char* filename);
static Status game_load_characters (Game* game, char* filename);
static Status game_load_players (Game* game, char* filename);
static Status game_load_numens (Game* game, char* filename);
static Status game_load_links (Game* game, char* filename);

/*
 * Falta EL leido y guardado del space->grid
 *
 */
static Status
game_load_spaces (Game* game, char* filename)
{
    FILE* file            = NULL;
    char line[WORD_SIZE]  = "";
    char name[WORD_SIZE]  = "";
    char gdesc[WORD_SIZE] = "";
    char OST[WORD_SIZE]   = "";
    char* toks            = NULL;
    Id id                 = NO_ID;
    Space* space          = NULL;
    Status status         = OK;
    int tam_format;
    int i;

    if (!game || !filename) return ERROR;

    file = fopen (filename, "r");
    if (file == NULL) return ERROR;

    tam_format = strlen (F_SPAC);

    while (fgets (line, WORD_SIZE, file))
        {
            if (strncmp (F_SPAC, line, tam_format) == 0)
                {

                    /* --- id --- */
                    toks = strtok (line + tam_format, "|");
                    if (!toks) continue;
                    id = atol (toks);

                    /* --- name --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    strcpy (name, toks);
                    /* --- gdesc (optional) --- */
                    toks = strtok (NULL, "|");

                    if (!toks)
                        {
                            strcpy (gdesc, "./img_src/background/default.jpg");
                        } /*default gdesc */
                    else
                        {
                            strcpy (gdesc, toks);
                        }

                    /* --- OST (optional) --- */
                    toks = strtok (NULL, "|\n");

                    if (toks)
                        {
                            strcpy (OST, toks);
                        }
                    else
                        {
                            OST[0] = '\0';
                        }

#ifdef DEBUG
                    printf ("Leido space: s:%ld|%s\n", id, name);
#endif

                    space = space_create ();
                    if (space != NULL)
                        {
                            space_set_id (space, id);
                            space_set_name (space, name);
                            space_set_gdesc (space, gdesc);
                            if (OST[0] != '\0') space_set_ost (space, OST);

                            game_add_space (game, space);
                        }
                }
        }

    if (ferror (file)) status = ERROR;

    fclose (file);
    return status;
}

static Status
game_load_objects (Game* game, char* filename)
{
    FILE* file                  = NULL;
    char line[WORD_SIZE]        = "";
    char name[WORD_SIZE]        = "";
    char description[WORD_SIZE] = "";
    char gdesc[WORD_SIZE]       = "";
    char* toks                  = NULL;
    Id id = NO_ID, space_id = NO_ID, dependency = NO_ID;
    Object* obj    = NULL;
    Space* space   = NULL;
    Player *player = NULL, *players_game[MAX_OBJECTS];
    Status status  = OK;
    Bool consumable, movable;
    int tam_format, health, attack, energy, speed, pos_x, pos_y, check, bucle, n_play;

    if (!game || !filename) return ERROR;

    tam_format = strlen (F_OBJE);
    file       = fopen (filename, "r");
    if (file == NULL) return ERROR;

    while (fgets (line, WORD_SIZE, file))
        {
            if (strncmp (F_OBJE, line, tam_format) == 0)
                {

                    /* --- id --- */
                    toks = strtok (line + tam_format, "|");
                    if (!toks) continue;
                    id = atol (toks);

                    /* --- name --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    strcpy (name, toks);

                    /* --- space_id --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    space_id = atol (toks);
                    /* --- pos_x --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    pos_x = atoi (toks);
                    /* --- pos_y --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    pos_y = atoi (toks);
                    /* --- description (rest of the line, optional) --- */
                    toks = strtok (NULL, "|");
                    if (toks)
                        {
                            strcpy (description, toks);
                        }
                    else
                        {
                            description[0] = '\0';
                        }
                    /* --- gdesc --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    strcpy (gdesc, toks);

                    /* --- consumable (0 or 1) --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;

                    consumable = atoi (toks) ? TRUE : FALSE;

                    /* --- health --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;

                    health = atoi (toks);

                    /* --- attack --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;

                    attack = atoi (toks);

                    /* --- energy --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;

                    energy = atoi (toks);

                    /* --- speed --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    speed = atoi (toks);

                    /* --- dependency (optional) --- */
                    toks = strtok (NULL, "|");
                    if (toks)
                        {
                            dependency = atol (toks);
                        }
                    else
                        {
                            dependency = NO_ID;
                        }

                    /* --- movable (0 or 1) --- */
                    toks = strtok (NULL, "|\n");
                    if (!toks) continue;

                    movable = atoi (toks) ? TRUE : FALSE;

#ifdef DEBUG
                    printf ("Leido object: o:%ld|%s|%ld|%s\n", id, name, space_id, description);
#endif

                    obj = obj_create ();
                    if (obj != NULL)
                        {
                            obj_set_id (obj, id);
                            obj_set_name (obj, name);
                            if (description[0] != '\0')
                                {
                                    obj_set_description (obj, description);
                                }
                            game_add_object (game, obj);

                            /* Place object in its space */
                            space = game_get_space (game, space_id);
                            if (space)
                                {
                                    space_set_object (space, id);
                                }
                            else
                                {
                                    check = game_get_n_players (game);
                                    for (bucle = 0; bucle < check; bucle++)
                                        {
                                            players_game[bucle] = game_get_object_at (game, bucle);
                                        }
                                    for (bucle = 0; bucle < check; bucle++)
                                        {
                                            if (player_contains_object (players_game[bucle], id) == TRUE)
                                                {
                                                    player_add_object (players_game[bucle], obj);
                                                }
                                        }
                                }

                            obj_set_health (obj, health);
                            obj_set_attack (obj, attack);
                            obj_set_energy (obj, energy);
                            obj_set_speed (obj, speed);
                            obj_set_consumable (obj, consumable);
                            obj_set_movable (obj, movable);
                            obj_set_dependency (obj, dependency);
                        }
                }
        }
    if (ferror (file)) status = ERROR;

    fclose (file);
    return status;
}

static Status
game_load_characters (Game* game, char* filename)
{
    FILE* file              = NULL;
    char line[WORD_SIZE]    = "";
    char name[WORD_SIZE]    = "";
    char gdesc[WORD_SIZE]   = "";
    char message[WORD_SIZE] = "";
    char* toks              = NULL;
    Character* ch           = NULL;
    Space* space            = NULL;
    Id id = NO_ID, space_id = NO_ID;
    int health = 0, friendly = 1;
    Status status = OK;
    int tam_format;

    if (!game || !filename) return ERROR;

    tam_format = strlen (F_CHAR);
    file       = fopen (filename, "r");
    if (file == NULL) return ERROR;

    while (fgets (line, WORD_SIZE, file))
        {
            if (strncmp (F_CHAR, line, tam_format) == 0)
                {

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
                    if (!toks) continue;
                    strcpy (gdesc, toks);

                    /* --- space_id (position) --- I3 ORDER: before health */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    space_id = atol (toks);

                    /* --- health --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    health = atoi (toks);

                    /* --- friendly (0 or 1) --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    friendly = atoi (toks);

                    /* --- message (rest of line, may contain spaces) --- */
                    toks = strtok (NULL, "\n");
                    if (toks)
                        {
                            strcpy (message, toks);
                        }
                    else
                        {
                            message[0] = '\0';
                        }

#ifdef DEBUG
                    printf ("Leido character: c:%ld|%s|%s|%ld|%d|%d|%s\n", id, name, gdesc, space_id, health, friendly, message);
#endif

                    ch = character_create ();
                    if (ch != NULL)
                        {
                            character_set_id (ch, id);
                            character_set_name (ch, name);
                            character_set_gdesc (ch, gdesc);
                            character_set_health (ch, health);
                            character_set_friendly (ch, friendly ? TRUE : FALSE);
                            character_set_message (ch, message);
                            game_add_character (game, ch);

                            /* Place character in its space */
                            space = game_get_space (game, space_id);
                            if (space) space_set_character (space, id);
                        }
                }
        }

    if (ferror (file)) status = ERROR;

    fclose (file);
    return status;
}

static Status
game_load_players (Game* game, char* filename)
{
    FILE* file            = NULL;
    char line[WORD_SIZE]  = "";
    char name[WORD_SIZE]  = "";
    char gdesc[WORD_SIZE] = "";
    char* toks            = NULL;
    Player* player        = NULL;
    Space* space          = NULL;
    Id id                 = NO_ID;
    Id zone               = NO_ID;
    int max_numens        = 0;
    int max_bag           = 0;
    Status status         = OK;
    int tam_format, pos_x = 0, pos_y = 0;

    if (!game || !filename) return ERROR;

    tam_format = strlen (F_PLAY);
    file       = fopen (filename, "r");
    if (!file) return ERROR;

    while (fgets (line, WORD_SIZE, file))
        {
            if (strncmp (F_PLAY, line, tam_format) == 0)
                {

                    /* --- id --- */
                    toks = strtok (line + tam_format, "|");
                    if (!toks) continue;
                    id = atol (toks);

                    /* --- name --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    strcpy (name, toks);

                    /* --- zone --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    zone = atol (toks);
                    /* --- pos_x --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    pos_x = atoi (toks);
                    /* --- pos_y --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    pos_y = atoi (toks);

                    /* --- gdesc --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    strcpy (gdesc, toks);

                    /* --- max_bag --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    max_bag = atoi (toks);

                    /* --- max_numens --- */
                    toks = strtok (NULL, "|\n");
                    if (!toks) continue;
                    max_numens = atoi (toks);

#ifdef DEBUG
                    printf ("Leido player: p:%ld|%s|%s|%ld|%d|%d\n", id, name, gdesc, zone, max_numens, max_bag);
#endif

                    player = player_create ();
                    if (player != NULL)
                        {
                            player_set_id (player, id);
                            player_set_name (player, name);
                            player_set_gdesc (player, gdesc);
                            player_set_zone (player, zone);
                            player_set_pos_x (player, pos_x); /*Por implementar en player.c y player.h junto al
                                                                 set y su inicialización (están en entity)*/
                            player_set_pos_y (player, pos_y); /*Por implementar en player.c y player.h junto al
                                                                 set y su inicialización (están en entity)*/

                            player_set_max_numens (player, max_numens);
                            player_set_max_objects (player, max_bag);

                            game_add_player (game, player);

                            space = game_get_space (game, zone);
                            if (space) space_set_discovered (space, TRUE);
                        }
                }
        }

    if (ferror (file)) status = ERROR;

    fclose (file);
    return status;
}

static Status
game_load_numens (Game* game, char* filename)
{
    FILE* file            = NULL;
    char line[WORD_SIZE]  = "";
    char name[WORD_SIZE]  = "";
    char gdesc[WORD_SIZE] = "";
    char* toks            = NULL;
    Numen* numen          = NULL;
    Space* space          = NULL;
    Id id = NO_ID, space_id = NO_ID;
    int health = 0, attack = 0, energy = 0, speed = 0, skills[4] = { NO_ID, NO_ID, NO_ID, NO_ID }, pos_x, pos_y;
    Id following  = NO_ID;
    Status status = OK;
    int tam_format;

    if (!game || !filename) return ERROR;

    tam_format = strlen (F_NUMEN);
    file       = fopen (filename, "r");
    if (file == NULL) return ERROR;

    while (fgets (line, WORD_SIZE, file))
        {
            if (strncmp (F_NUMEN, line, tam_format) == 0)
                {

                    /* --- id --- */
                    toks = strtok (line + tam_format, "|");
                    if (!toks) continue;
                    id = atol (toks);

                    /* --- name --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    strcpy (name, toks);

                    /* --- space_id (position) --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    space_id = atol (toks);

                    /* --- pos_x --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    pos_x = atoi (toks);

                    /* --- pos_y --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    pos_y = atoi (toks);

                    /* --- gdesc --- */
                    toks = strtok (NULL, "");
                    if (!toks) continue;
                    else strcpy (gdesc, toks);

                    /* --- health --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    health = atoi (toks);

                    /* --- energy --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    energy = atoi (toks);

                    /* --- attack --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    attack = atoi (toks);

                    /* --- speed --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    speed = atoi (toks);

                    /* --- skills (up to 4, separated by '|', optional) --- */
                    for (int i = 0; i < 4; i++)
                        {
                            toks = strtok (NULL, "|");
                            if (toks)
                                {
                                    skills[i] = atol (toks);
                                }
                            else
                                {
                                    skills[i] = NO_ID;
                                }
                        }

                    /* --- following (optional) --- */
                    toks = strtok (NULL, "|\n");
                    if (toks)
                        {
                            following = atol (toks);
                        }

                    numen = numen_create ();
                    if (numen != NULL)
                        {
                            numen_set_id (numen, id);
                            numen_set_name (numen, name);
                            numen_set_gdesc (numen, gdesc);
                            numen_set_health (numen, health);
                            numen_set_energy (numen, energy);
                            numen_set_attack (numen, attack);
                            numen_set_speed (numen, speed);
                            for (int i = 0; i < 4; i++)
                                numen_set_skill (numen, skills[i]);

                            numen_set_following (numen, following);
                            if (following != NO_ID)
                                player_add_numen (game_get_player_by_id (game, following), id); /* check if following id exists as a player, if not,
                                                                                                   it will be set to NO_ID in numen_set_following */
                            game_add_numens (game, numen);

                            /* Place numen in its space */
                            space = game_get_space (game, space_id);
                            if (space) space_set_numen (space, id);
                        }
                }
        }

    if (ferror (file)) status = ERROR;

    fclose (file);
    return status;
}

static Status
game_load_links (Game* game, char* filename)
{
    FILE* file           = NULL;
    char line[WORD_SIZE] = "";
    char name[WORD_SIZE] = "";
    char* toks           = NULL;
    Links* link          = NULL;
    Id id                = NO_ID;
    Id origin = NO_ID, dest = NO_ID;
    int dir_num   = 0;
    int open_otd  = 1; /* open origin-to-dest */
    int open_dto  = 1; /* open dest-to-origin */
    Status status = OK;
    int tam_format;

    if (!game || !filename) return ERROR;

    tam_format = strlen (F_LINK);
    file       = fopen (filename, "r");
    if (!file) return ERROR;

    while (fgets (line, WORD_SIZE, file))
        {
            if (strncmp (F_LINK, line, tam_format) == 0)
                {

                    /* --- id --- */
                    toks = strtok (line + tam_format, "|");
                    if (!toks) continue;
                    id = atol (toks);

                    /* --- name --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    strcpy (name, toks);

                    /* --- origin id --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    origin = atol (toks);

                    /* --- destination id --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    dest = atol (toks);

                    /* --- direction (numeric: N=0 S=1 E=2 W=3) --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    dir_num = atoi (toks);

                    /* --- open_orig_to_dest --- */
                    toks = strtok (NULL, "|");
                    if (!toks) continue;
                    open_otd = atoi (toks);

                    /* --- open_dest_to_orig (bidirectional extension) --- */
                    toks = strtok (NULL, "|\n");
                    if (toks)
                        {
                            open_dto = atoi (toks);
                        }
                    else
                        {
                            /* if only 6 fields are present (standard enunciado format),
                            use the same value for both directions, making the link symmetric.*/
                            open_dto = open_otd;
                        }

#ifdef DEBUG
                    printf ("Leido link: l:%ld|%s|%ld|%ld|%d|%d|%d\n", id, name, origin, dest, dir_num, open_otd, open_dto);
#endif

                    link = link_create ();
                    if (link != NULL)
                        {
                            link_set_id (link, id);
                            link_set_name (link, name);
                            link_set_origin_id (link, origin);
                            link_set_destination_id (link, dest);

                            /*
                             * Cast the integer to Direction.  The enum values are:
                             *   N=0, S=1, E=2, W=3, U=4
                             * If dir_num is out of range, link_set_direction will
                             * reject U and return ERROR, but we continue anyway
                             * (the link simply won't work in-game).
                             */
                            link_set_direction (link, (Direction)dir_num);

                            link_set_open_origin_to_dest (link, open_otd ? TRUE : FALSE);
                            link_set_open_dest_to_origin (link, open_dto ? TRUE : FALSE);

                            game_add_links (game, link);
                        }
                }
        }

    if (ferror (file)) status = ERROR;
    fclose (file);
    return status;
}

Status
game_management_create_from_file (Game** game, char* filename)
{
    if (!game || !filename) return ERROR;

    *game = game_create ();
    if (*game == NULL) return ERROR;

    if (game_load_spaces (*game, filename) == ERROR)
        {
            game_destroy (*game);
            *game = NULL;
            return ERROR;
        }

    if (game_load_players (*game, filename) == ERROR)
        {
            game_destroy (*game);
            *game = NULL;
            return ERROR;
        }

    if (game_load_objects (*game, filename) == ERROR)
        {
            game_destroy (*game);
            *game = NULL;
            return ERROR;
        }

    if (game_load_characters (*game, filename) == ERROR)
        {
            game_destroy (*game);
            *game = NULL;
            return ERROR;
        }

    if (game_load_links (*game, filename) == ERROR)
        {
            game_destroy (*game);
            *game = NULL;
            return ERROR;
        }

    if (game_load_numens (*game, filename) == ERROR)
        {
            game_destroy (*game);
            *game = NULL;
            return ERROR;
        }

    return OK;
}

Status
game_management_save_file (Game** game)
{
    Space* space   = NULL;
    Object* object = NULL;
    Object* players_game[MAX_OBJECTS];
    Player* player = NULL;
    Numen* numen   = NULL;
    Links* link    = NULL;
    Direction dir  = U;
    Id id          = NO_ID;
    Id zone        = NO_ID;
    Id origin      = NO_ID;
    Id dest        = NO_ID;
    Id dependency  = NO_ID;
    Id following   = NO_ID;
    Set* skills[NUM_SKILLS];
    int check = 0;
    int bucle2;
    int bucle;
    int pos_x = 0;
    int pos_y = 0;
    int n_play;
    int health = MIN_LIFE;
    int attack = MIN_ATTACK;
    int energy = MIN_ENGY;
    int speed  = 0;
    int max_bag;
    int max_numens;
    Bool friendly;
    Bool open_otd;
    Bool open_dto;
    Bool consumable;
    Bool movable;
    char* name      = NULL;
    char* gdesc     = NULL;
    char* OST       = NULL;
    char* message   = NULL;
    FILE* new_sfile = NULL;

    if (!game) return ERROR;
    new_sfile = fopen ("../savefile/savefile.dat", "w");

    if (!new_sfile) return ERROR;

    check = game_get_n_players (game);
    for (bucle = 0; bucle < check; bucle++)
        players_game[bucle] = game_get_object_at (game, bucle);

    check = game_get_n_spaces (*game);
    for (bucle = 0; bucle < check; bucle++)
        {
            id    = game_get_space_id_at (game, bucle);
            space = game_get_space (*game, id);
            name  = space_get_name (space);
            gdesc = space_get_gdesc (space);
            OST   = space_get_ost (space); /*Por implementar en space.c y space.h, además de
                                              definir en la estructura space la variable char
                                              *ost y su inicialización y destrucción */

            fprintf (new_sfile, "#s:%ld|%s|%s|%s|\n", id, name, gdesc == NULL ? "" : gdesc, OST == NULL ? "" : OST);

            free (name);
            free (gdesc);
            free (OST);
        }

    n_play = game_get_n_players (*game);
    check  = game_get_n_players (*game);
    for (bucle = 0; bucle < check; bucle++)
        {
            player = game_get_player_at (game, bucle);
            id     = player_get_id (player);
            name   = player_get_name (player);
            zone   = player_get_zone (player);
            pos_x  = player_get_pos_x (player);           /*Por implementar en player.c y player.h junto al set y su
                                                             inicialización (están en entity)*/
            pos_y = player_get_pos_y (player);            /*Por implementar en player.c y player.h junto al set y su
                                                             inicialización (están en entity)*/
            gdesc      = player_get_gdesc (player);       /*Hay que cambialo por una ruta string*/
            max_bag    = player_get_max_objects (player); /*Por implementar en player.c y player.h*/
            max_numens = player_get_max_numens (player);  /*Por implementar en player.c y player.h junto al set y su
                                                             inicialización (están en entity)*/

            fprintf (new_sfile, "#p:%ld|%s|%ld|%d|%d|%s|%d|%d|\n", id, name, zone, pos_x, pos_y, gdesc == NULL ? "" : gdesc, max_bag, max_numens);

            free (name);
            free (gdesc);
        }

    check = game_get_n_objects (*game);
    for (bucle = 0; bucle < check; bucle++)
        {
            object = game_get_object_at (game, bucle);
            id     = obj_get_id (object);
            name   = obj_get_name (object);
            zone   = game_get_object_zone (game, id);
            if (zone == NO_ID)
                {
                    for (bucle2 = 0; bucle2 < n_play; bucle2++)
                        {
                            if (player_contains_object (players_game[bucle2], id) == TRUE)
                                {
                                    zone = player_get_id (players_game[bucle2]);
                                }
                        }
                }
            pos_x = obj_get_pos_x (object); /*Por implementar en object.c y object.h junto al
                                               set y su inicialización(están en entity)*/
            pos_y = obj_get_pos_y (object); /*Por implementar en object.c y object.h junto al
                                               set y su inicialización (están en entity)*/
            message = obj_get_description (object);
            gdesc   = obj_get_gdesc (object); /*Por implementar en object.c y object.h junto al
                                                 set y su inicialización (están en entity)*/
            health = obj_get_health (object);
            attack = obj_get_attack (object); /*Por implementar en object.c y object.h junto al set y su
                                                 inicialización (están en entity)*/
            energy = obj_get_energy (object); /*Por implementar en object.c y object.h junto al set y su
                                                 inicialización (están en entity)*/
            speed = obj_get_speed (object);   /*Por implementar en object.c y object.h junto al
                                                 set y su inicialización (están en entity)*/
            movable    = obj_get_movable (object);
            dependency = obj_get_dependency (object); /*Por implementar en object.c y object.h junto al set y su
                                                         inicialización (están en entity)*/
            consumable = obj_get_consumable (object); /*Por implementar en object.c y object.h y
                                                         añadir boolean consumable a la estructura
                                                         object e inicializarlo en el create*/

            fprintf (new_sfile, "#o:%ld|%s|%ld|%d|%d|%s|%s|%d|%d|%d|%d|%d|%d|%d|%d|\n", id, name, zone, pos_x, pos_y, message,
                     gdesc == NULL ? "" : gdesc, consumable == TRUE ? 1 : 0, health, energy, attack, speed, dependency == NO_ID ? "" : dependency,
                     movable == TRUE ? 1 : 0);
            free (name);
            free (gdesc);
            free (message);
        }

    /**Hay que implementar todas las funciones de numen aquí junto a las create y
     * set (tomar de referencia character) */
    check = game_get_n_numens (*game);
    for (bucle = 0; bucle < check; bucle++)
        {
            numen = game_get_numen_at (game, bucle);
            id    = numen_get_id (numen);
            name  = numen_get_name (numen);
            zone  = game_get_numen_zone (game, id); /*Por implementar en game.c y game.h,
                                                       junto al array de numens en space */
            pos_x  = numen_get_pos_x (numen);
            pos_y  = numen_get_pos_y (numen);
            gdesc  = numen_get_gdesc (numen);
            health = numen_get_health (numen);
            attack = numen_get_attack (numen);
            energy = numen_get_energy (numen);
            speed  = numen_get_speed (numen);

            for (int j = 0; j < MAX_HELD_SKILLS; j++)
                skills[j] = numen_get_skill_by_index (numen, j);
            following = numen_get_following (numen);

            fprintf (new_sfile, "#n:%ld|%s|%ld|%d|%d|%s|%d|%d|%d|%d|%ld|%ld|%ld|%ld|%ld|\n", id, name, zone, pos_x, pos_y, gdesc == NULL ? "" : gdesc,
                     health, energy, attack, speed, skills[0], skills[1], skills[2], skills[3], following);
            free (name);
            free (gdesc);
        }

    check = game_get_n_links (*game);
    for (bucle = 0; bucle < check; bucle++)
        {
            link     = game_get_link_at (game, bucle);
            id       = link_get_id (link);
            name     = link_get_name (link);
            origin   = link_get_origin_id (link);
            dest     = link_get_destination_id (link);
            dir      = link_get_direction (link);
            open_otd = link_get_open_origin_to_dest (link);
            open_dto = link_get_open_dest_to_origin (link);

            fprintf (new_sfile, "#l:%ld|%s|%ld|%ld|%d|%d|%d|\n", id, name, origin, dest, dir, open_otd == TRUE ? 1 : 0, open_dto == TRUE ? 1 : 0);
            free (name);
        }

    fclose (new_sfile);
    return OK;
}