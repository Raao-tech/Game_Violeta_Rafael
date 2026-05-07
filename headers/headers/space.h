/**
 * @brief It defines the space module interface
 *
 * @file space.h
 * @author Profesores PPROG, Violeta y Rafa
 * @version 3
 * @date 08-04-2026
 * @copyright GNU Public License
 */
#ifndef SPACE_H
#define SPACE_H

#include "set.h"
#include "types.h"

#define MAX_LINE 5 /*!< Number of lines in the graphic description */
#define MAX_CHAR 9 /*!< Max characters per gdesc line */

typedef struct _Space Space;

/* ========== Create / Destroy ========== */

/**
 * @brief It creates a new space, allocating memory and initializing its members
 * @author Profesores PPROG
 *
 * @return a new space, initialized, or NULL if allocation fails
 */
Space* space_create ();

/**
 * @brief It destroys a space, freeing the allocated memory
 * @author Profesores PPROG
 *
 * @param space a pointer to the space that must be destroyed
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_destroy (Space* space);

/* ========== Id ========== */

/**
 * @brief It sets the id of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param new_id a value with the id of space
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_id (Space* space, Id new_id);

/**
 * @brief It gets the id of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return the id of space
 */
Id space_get_id (Space* space);

/* ========== Name ========== */

/**
 * @brief It sets the name of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param name a string with the name to store
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_name (Space* space, char* name);

/**
 * @brief It gets the name of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return a string with the name of the space
 */
const char* space_get_name (Space* space);

/* ========== Objects (Set of Ids) ========== */

/**
 * @brief It adds an object id to the space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @param new_id the id of the object to add
 * @param obj_pos the position of the object to add
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_object (Space* space, Id new_id, Position obj_pos);

/**
 * @brief It checks whether an object id is in the space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @param id_obj the id of the object to search for
 * @return TRUE if found, FALSE otherwise
 */
Bool space_contains_object (Space* space, Id id_obj);

/**
 * @brief It removes an object id from the space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @param id_obj the id of the object to remove
 * @param obj_pos the position of the object to remove
 * @return OK if removed, ERROR if not found or space is NULL
 */
Status space_remove_object (Space* space, Id obj_id, Position obj_pos);

/**
 * @brief It returns the number of objects in the space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @return number of objects, or ERROR_MAIN if space is NULL
 */
int space_get_n_objects (Space* space);

/**
 * @brief It gets the object id at a specific position in the space
 * @author Rafael
 *
 * @param space a pointer to the space
 * @param position zero-based index
 * @return the object Id at that position, or NO_ID if out of range
 */
Id space_get_object_id_at (Space* space, int position);

/* ========== Characters (Set of Ids) ========== */

/**
 * @brief It adds a character id to the space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @param new_id the id of the character to add
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_character (Space* space, Id new_id);

/**
 * @brief It checks whether a character id is in the space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @param id_chara the id of the character to search for
 * @return TRUE if found, FALSE otherwise
 */
Bool space_contains_character (Space* space, Id id_chara);

/**
 * @brief It removes a character id from the space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @param id_chara the id of the character to remove
 * @return OK if removed, ERROR if not found or space is NULL
 */
Status space_remove_character (Space* space, Id id_chara);

/**
 * @brief It returns the number of characters in the space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @return number of characters, or ERROR_MAIN if space is NULL
 */
int space_get_n_characters (Space* space);
/* ========== Grid ====================== */

/**
 * @brief It sets a line of the grid for the space
 * @author Rafael and Salvador
 *
 * @param space a pointer to the space
 * @param line the line number to set
 * @param content a pointer to the array of integers representing the line
 * @return OK if set, ERROR if space is NULL or line is out of bounds
 */
Status space_set_grid_by_line (Space* space, int line, int* content);
/**
 * @brief It gets a line of the grid for the space
 * @author Rafael
 *
 * @param space a pointer to the space
 * @param line the line number to get
 * @return a pointer to the array of integers representing the line, or NULL if space is NULL or line is out of bounds
 */
int* space_get_grid_by_line (Space* space, int line);

/* ========== OST ====================== */

/**
 * @brief It sets the OST filename for the space
 * @author Salvador
 *
 * @param space a pointer to the space
 * @param ost the OST filename
 * @return OK if set, ERROR if space is NULL or ost is invalid
 */
Status space_set_ost (Space* space, char* ost);
/**
 * @brief It returns the OST filename stored in the space
 * @author Salvador
 *
 * @param space a pointer to the space
 * @return pointer to the OST filename, or NULL if space is NULL
 */
char* space_get_ost (Space* space);

/* ========== Numens (Set of Ids) ========== */

/**
 * @brief It returns the set of numens contained in the space
 * @author Salvador
 *
 * @param space a pointer to the space
 * @return pointer to the set of numens_id contained in space, or NULL if space is NULL
 */
Set* space_get_numens (Space* space);

/**
 * @brief It returns the number of numens contained in the space
 * @author Salvador
 *
 * @param space a pointer to the space
 * @return number of numens, or ERROR_MAIN if space is NULL
 */
int space_get_n_numens (Space* space);

/**
 * @brief It adds a numen id to the space
 * @author Salvador
 *
 * @param space a pointer to the space
 * @param new_id the id of the numen to add
 * @return OK if added, ERROR if space is NULL or id is invalid
 */
Status space_set_numen (Space* space, Id new_id, Position numen_pos);

/**
 * @brief It deletes a numen id from the space
 * @author Salvador
 *
 * @param space a pointer to the space
 * @param id_numen the id of the numen to delete
 * @return OK if deleted, ERROR if space is NULL or id is invalid
 */
Status space_remove_numen (Space* space, Id id_numen, Position numen_pos);

/**
 * @brief It checks whether a numen id is in the space
 * @author Salvador
 *
 * @param space a pointer to the space
 * @param id_numen the id of the numen to search for
 * @return TRUE if found, FALSE otherwise
 */
Bool space_contains_numen (Space* space, Id id_numen);

/* ========== Discovered (F12) ========== */

/**
 * @brief It sets whether the space has been discovered
 * @author Rafael
 *
 * @param space a pointer to the space
 * @param value TRUE if discovered, FALSE otherwise
 * @return OK if successful, ERROR if space is NULL
 */
Status space_set_discovered (Space* space, Bool value);

/**
 * @brief It gets whether the space has been discovered
 * @author Rafael
 *
 * @param space a pointer to the space
 * @return TRUE if discovered, FALSE if not or if space is NULL
 */
Bool space_get_discovered (Space* space);

/* ========== Graphic description ========== */

/**
 * @brief It sets one line of the graphic description of a space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @param desc a string with the content for that line (max MAX_CHAR chars)
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_set_gdesc (Space* space, char* desc);

/**
 * @brief It prints the full graphic description of a space to an output stream
 * @author Violeta y Rafa
 *
 * @param output file pointer where the gdesc will be printed
 * @param space a pointer to the space
 * @return OK if successful, ERROR otherwise
 */
Status space_print_gdesc (FILE* output, Space* space);

/**
 * @brief It gets one line of the graphic description of a space
 * @author Violeta y Rafa
 *
 * @param space a pointer to the space
 * @return a pointer to copy of the string, or NULL if error
 */
char* space_get_gdesc (Space* space);

/* ========== Print ========== */

/**
 * @brief It prints all the space information
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
Status space_print (Space* space);

#endif