/**
 * @brief It implements the space module
 *
 * @file space.c
 * @author Profesores PPROG, Violeta, Salvador y Rafa
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */

#include "space.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Space structure
 *
 * It defines the structure of a space, which is a location in the game world.
 *
 *
 */
struct _Space
{
    Id id;                     /*!< Id number of the space, must be unique */
    char name[WORD_SIZE + 1];  /*!< Name of the space */
    char gdesc[WORD_SIZE + 1]; /*!< Graphic description, only a filename */
    char OST[WORD_SIZE + 1];   /*!< Background music, only a filename */
    int* grid[HIGHT];          /*!< Grid of graphi engine*/
    Set* objs_id;              /*!< Set of object IDs in this space */
    Set* characters_id;        /*!< Set of character IDs in this space */
    Set* numens_id;            /*!< Set of numen IDs in this space */
    Bool discovered;           /*!< Has this space been visited by a player? */
};

/* ========== Create / Destroy ========== */

Space*
space_create ()
{
    Space* newSpace = NULL;
    int i;

    newSpace = (Space*)calloc (1, sizeof (Space));
    if (!newSpace) return NULL;

    newSpace->id         = NO_ID;
    newSpace->name[0]    = '\0';
    newSpace->gdesc[0]   = '\0';
    newSpace->OST[0]     = '\0';
    newSpace->discovered = FALSE;
    newSpace->objs_id    = set_creat ();
    if (!newSpace->objs_id)
        {
            free (newSpace);
            return NULL;
        }

    /*Creamos la cuadrilla "grid" para la movilidad intra-space (Walk)*/
    for (i = 0; i < HIGHT; i++) newSpace->grid[i] = NULL;

    newSpace->characters_id = set_creat ();
    if (!newSpace->characters_id)
        {
            set_destroy (newSpace->objs_id);
            free (newSpace);
            return NULL;
        }

    newSpace->numens_id = set_creat ();
    if (!newSpace->numens_id)
        {
            set_destroy (newSpace->characters_id);
            set_destroy (newSpace->objs_id);
            free (newSpace);
            return NULL;
        }

    return newSpace;
}

Status
space_destroy (Space* space)
{
    int i;
    if (!space) return ERROR;

    set_destroy (space->characters_id);
    set_destroy (space->objs_id);
    set_destroy (space->numens_id);

    for (i = 0; i < HIGHT; i++)
    {
        if (space->grid[i])
        {
            free (space->grid[i]);
            space->grid[i] = NULL;
        }
    }

    free (space);
    return OK;
}
/* ========== Id ========== */

Status
space_set_id (Space* space, Id new_id)
{
    if (!space) return ERROR;
    space->id = new_id;
    return OK;
}

Id
space_get_id (Space* space)
{
    if (!space) return NO_ID;
    return space->id;
}

/* ========== Name ========== */

Status
space_set_name (Space* space, char* name)
{
    if (!space || !name) return ERROR;

    strncpy (space->name, name, WORD_SIZE);
    space->name[WORD_SIZE] = '\0';

    return OK;
}

const char*
space_get_name (Space* space)
{
    if (!space) return NULL;
    return space->name;
}

/* ========== Objects ========== */

Status
space_set_object (Space* space, Id new_id, Position obj_pos)
{
    int cell_x, cell_y;

    if (!space) return ERROR;

    /* Convertimos pixeles → celda. Solo tocamos el grid si las
     * coordenadas estan dentro del rango valido. Pero si vienen como
     * NO_POS o fuera de pantalla, simplemente añadimos al set
     * sin tocar el grid. */
    if (obj_pos.pos_x != NO_POS && obj_pos.pos_y != NO_POS)
    {
        cell_x = obj_pos.pos_x / SCALE;
        cell_y = obj_pos.pos_y / SCALE;

        if (cell_x >= 0 && cell_x < WIDHT && cell_y >= 0 && cell_y < HIGHT && space->grid[cell_y] != NULL)
        {if (space->grid[cell_y][cell_x] != 0)space->grid[cell_y][cell_x] = 0;}
    }

    return set_add (space->objs_id, new_id);
}

Bool
space_contains_object (Space* space, Id id_obj)
{
    if (!space) return FALSE;
    return set_contains_id (space->objs_id, id_obj);
}

Status
space_remove_object (Space* space, Id obj_id, Position obj_pos)
{
    int cell_x, cell_y;

    if (!space) return ERROR;

    if (obj_pos.pos_x != NO_POS && obj_pos.pos_y != NO_POS)
    {
        cell_x = obj_pos.pos_x / SCALE;
        cell_y = obj_pos.pos_y / SCALE;

        if (cell_x >= 0 && cell_x < WIDHT &&
            cell_y >= 0 && cell_y < HIGHT &&
            space->grid[cell_y] != NULL)
        {
        if (space->grid[cell_y][cell_x] == 0)
            space->grid[cell_y][cell_x] = 1;
        }
    }

    return set_delete_id (space->objs_id, obj_id);
}

int
space_get_n_objects (Space* space)
{
    if (!space) return ERROR_MAIN;
    return set_get_n_ids (space->objs_id);
}

Id
space_get_object_id_at (Space* space, int position)
{
    if (!space) return NO_ID;
    return set_get_id_at (space->objs_id, position);
}

/* ========== Characters ========== */

Status
space_set_character (Space* space, Id new_id)
{
    if (!space) return ERROR;
    return set_add (space->characters_id, new_id);
}

Bool
space_contains_character (Space* space, Id id_chara)
{
    if (!space) return FALSE;
    return set_contains_id (space->characters_id, id_chara);
}

Status
space_remove_character (Space* space, Id id_chara)
{
    if (!space) return ERROR;
    return set_delete_id (space->characters_id, id_chara);
}

int
space_get_n_characters (Space* space)
{
    if (!space) return ERROR_MAIN;
    return set_get_n_ids (space->characters_id);
}

/* ========== Grid ================ */

Status
space_set_grid_by_line (Space* space, int line, int* l)
{
    if (!space || !l) return ERROR;
    if (line < 0 || line >= HIGHT) return ERROR;

    /* Liberamos la fila anterior si la habia (caso de re-asignacion) */
    if (space->grid[line]) free (space->grid[line]);

    space->grid[line] = l;
    return OK;
}

Status
space_set_grid_by_position (Space* space, Position position, int value)
{
    if(!space||position.pos_x<0||position.pos_y<0||position.pos_x>WIDHT_MAP||position.pos_y>HIGHT_MAP||value<0||value>9) return ERROR;
    space->grid[position.pos_y/SCALE][position.pos_x/SCALE]=value;
    return OK;
}

int*
space_get_grid_by_line (Space* space, int line)
{
    if (!space || line < 0 || line >= HIGHT) return NULL;
    return space->grid[line];
}

/* ========== OST ====================== */

Status
space_set_ost (Space* space, char* ost)
{
    if (!space || !ost) return ERROR;

    strncpy (space->OST, ost, WORD_SIZE);
    space->OST[WORD_SIZE] = '\0';

    return OK;
}

char*
space_get_ost (Space* space)
{
    if (!space) return NULL;
    return space->OST;
}

/* ========== Numens ========== */
Set*
space_get_numens (Space* space)
{
    if (!space) return NULL;
    return space->numens_id;
}

int
space_get_n_numens (Space* space)
{
    if (!space) return ERROR_MAIN;
    return set_get_n_ids (space->numens_id);
}

Status
space_set_numen (Space* space, Id new_id, Position numen_pos)
{
    int cell_x, cell_y;

    if (!space) return ERROR;

    /* Conversion pixeles -> celda. Solo tocamos el grid si las
     * coordenadas son validas. Si vienen NO_POS o fuera de rango,
     * simplemente añadimos al set sin marcar el grid. */
    if (numen_pos.pos_x != NO_POS && numen_pos.pos_y != NO_POS)
    {
        cell_x = numen_pos.pos_x / SCALE;
        cell_y = numen_pos.pos_y / SCALE;

        if (cell_x >= 0 && cell_x < WIDHT &&
            cell_y >= 0 && cell_y < HIGHT &&
            space->grid[cell_y] != NULL)
        {
            if (space->grid[cell_y][cell_x] == 1)
                space->grid[cell_y][cell_x] = 2;
        }
    }

    return set_add (space->numens_id, new_id);
}

Status
space_remove_numen (Space* space, Id id_numen, Position numen_pos)
{
    int cell_x, cell_y;

    if (!space) return ERROR;

    if (numen_pos.pos_x != NO_POS && numen_pos.pos_y != NO_POS)
    {
        cell_x = numen_pos.pos_x / SCALE;
        cell_y = numen_pos.pos_y / SCALE;

        if (cell_x >= 0 && cell_x < WIDHT &&
            cell_y >= 0 && cell_y < HIGHT &&
            space->grid[cell_y] != NULL)
        {
            if (space->grid[cell_y][cell_x] == 2)
                space->grid[cell_y][cell_x] = 1;
        }
    }

    return set_delete_id (space->numens_id, id_numen);
}

Bool
space_contains_numen (Space* space, Id id_numen)
{
    if (!space) return FALSE;
    return set_contains_id (space->numens_id, id_numen);
}
/* ========== Discovered ========== */

Status
space_set_discovered (Space* space, Bool value)
{
    if (!space) return ERROR;
    space->discovered = value;
    return OK;
}

Bool
space_get_discovered (Space* space)
{
    if (!space) return FALSE;
    return space->discovered;
}

/* ========== Graphic description ========== */

Status
space_set_gdesc (Space* space, char* desc)
{
    if (!space || !desc) return ERROR;
    strncpy (space->gdesc, desc, WORD_SIZE);
    space->gdesc[WORD_SIZE] = '\0';
    return OK;
}

Status
space_print_gdesc (FILE* output, Space* space)
{
    if (!output || !space) return ERROR;

    fprintf (output, "Img of space: %s\n", space->gdesc);
    return OK;
}

char*
space_get_gdesc (Space* space)
{
    if (!space) return NULL;
    return strdup (space->gdesc);
}

/* ========== Print ========== */

Status
space_print (Space* space)
{
    if (!space) return ERROR;

    fprintf (stdout, "--> Space (Id: %ld; Name: %s; Discovered: %s)\n", space->id, space->name, space->discovered == TRUE ? "YES" : "NO");

    /* Objects */
    if (space_get_n_objects (space) > 0)
        {
            fprintf (stdout, "---> Objects in the space:\n");
            set_print (stdout, space->objs_id);
        }
    else
        {
            fprintf (stdout, "---> No objects in the space.\n");
        }

    /* Characters */
    if (space_get_n_characters (space) > 0)
        {
            fprintf (stdout, "---> Characters in the space:\n");
            set_print (stdout, space->characters_id);
        }
    else
        {
            fprintf (stdout, "---> No characters in the space.\n");
        }

    /* Graphic description */
    space_print_gdesc (stdout, space);

    return OK;
}