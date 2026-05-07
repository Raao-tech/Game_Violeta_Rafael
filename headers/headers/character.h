/**
 * @brief It defines the character module interface
 *
 * @file character.h
 * @author Violeta y Rafa
 * @version 2
 * @date 04-02-2025
 * @copyright GNU Public License
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.h"
#include "types.h"

typedef struct _Character Character;

/**
 * @brief It creates a new character, allocating memory and initializing it
 * @author Violeta y Rafa
 *
 * @return a new character, initialized, or NULL if memory fails
 */
Character* character_create ();

/**
 * @brief It destroys a character, freeing the allocated memory
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_destroy (Character* character);

/**
 * @brief It sets the id of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param new_id the new id value
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_id (Character* character, Id new_id);

/**
 * @brief It gets the id of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return the character's id, or NO_ID if character is NULL
 */
Id character_get_id (Character* character);

/**
 * @brief It sets the name of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param name the name of the character
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_name (Character* character, char* name);

/**
 * @brief It checks whether the character has the given name
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param name the name to compare against
 * @return TRUE if the names match, FALSE otherwise or if any parameter is NULL
 */
Bool character_has_name (Character* character, char* name);

/**
 * @brief It gets a copy of the name of a character (caller must free it)
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return a copy of the name, or NULL if character is NULL
 */
char* character_get_name (Character* character);

/**
 * @brief It sets the graphic description of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param desc a string with the graphic description (max 6 chars for F4)
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_gdesc (Character* character, char* desc);

/**
 * @brief It gets a copy of the graphic description of a character (caller must free it)
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return a copy of the gdesc, or NULL if character is NULL
 */
char* character_get_gdesc (Character* character);

/**
 * @brief It sets the message of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param messg a string with the message
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_message (Character* character, char* messg);

/**
 * @brief It gets a copy of the message of a character (caller must free it)
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return a copy of the message, or NULL if character is NULL
 */
char* character_get_message (Character* character);

/**
 * @brief It sets the health of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param life the health points to set
 * @return OK, if everything goes well, or ERROR if out of bounds
 */
Status character_set_health (Character* character, int life);

/**
 * @brief It gets the health of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return the health value, or ERROR_LIFE if character is NULL
 */
int character_get_health (Character* character);
/**
 * @brief It sets the energy value of a character
 * @author Rafa
 *
 * @param character a pointer to the character
 * @param value the energy value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_energy (Character* character, int energy);

/**
 * @brief It gets the attack value of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return the energy value, or ERROR_ATTACK if character is NULL
 */
int character_get_energy (Character* character);
/**
 * @brief It sets the attack value of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param value the attack value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_attack (Character* character, int value);

/**
 * @brief It gets the attack value of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return the attack value, or ERROR_ATTACK if character is NULL
 */
int character_get_attack (Character* character);

/**
 * @brief It sets the position of a character
 * @author Violeta y Rafa
 *
 * @param cha a pointer to the character
 * @param x the x value to set
 * @param y the y value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status  character_set_position (Character* cha, int x, int y);
/**
 * @brief It gets the position value of a character
 * @author Violeta y Rafa
 *
 * @param cha a pointer to the character
 * @return the position value, or NO_POS in the x and y value if character is NULL
 */
Position    character_get_position (Character* cha);
/**
 * @brief It sets the x position of a character
 * @author Violeta y Rafa
 *
 * @param cha a pointer to the character
 * @param x the x value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status  character_set_pos_x (Character* cha, int pos_x);
/**
 * @brief It gets the x position value of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return the x position value, or NO_POS if character is NULL
 */
int character_get_pos_x (Character* cha);
/**
 * @brief It sets the y position of a character
 * @author Violeta y Rafa
 *
 * @param cha a pointer to the character
 * @param y the y value to set
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status  character_set_pos_y (Character* cha, int pos_x);
/**
 * @brief It gets the y position value of a character
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return the y position value, or NO_POS if character is NULL
 */
int character_get_pos_y (Character* cha);

/**
 * @brief It sets whether the character is friendly or not
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @param value TRUE for friendly, FALSE for unfriendly
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_friendly (Character* character, Bool value);

/**
 * @brief It gets whether the character is friendly or not
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return TRUE if friendly, defaults to TRUE if character is NULL
 */
Bool character_get_friendly (Character* character);
/**
 * @brief It sets the id of the character that this character is following
 * @author Salvador
 *
 * @param character a pointer to the character
 * @param id_following the id of the character to follow
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status character_set_following (Character* character, Id id_following);

/**
 * @brief It gets the id of the character that this character is following
 * @author Salvador
 *
 * @param character a pointer to the character
 * @return the id of the character being followed, or NO_ID if character is NULL
 */
Id character_get_following (Character* character);

/* SETEA Y DEVUELVE EL SPEED */
/**
 * @brief It sets the speed of the character that this character is following
 * @author Salvador
 *
 * @param character a pointer to the character
 * @param speed the speed of the character to follow
 * @return OK, if everything goes well, or ERROR if there was some mistake
 */
Status  character_set_speed (Character* character, int speed);
/**
 * @brief It gets the speed of the character that this character is following
 * @author Salvador
 *
 * @param character a pointer to the character
 * @return the speed of the character being followed, or 0 if character is NULL
 */
int character_get_speed (Character* character);

/**
 * @brief It prints all the character information for debugging
 * @author Violeta y Rafa
 *
 * @param character a pointer to the character
 * @return OK, if everything goes well, or ERROR if character is NULL
 */
Status character_print (Character* character);



#endif
