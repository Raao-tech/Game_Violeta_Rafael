/**
 * @brief It defines the numen module interface
 *
 * @file Numens.h
 * @author Rafa
 * @version 0
 * @date 18-04-2026
 * @copyright GNU Public License
 */

#ifndef NUMEN_H
#define NUMEN_H

#include "character.h"
#include "skills.h"
#include "types.h"

typedef struct _Numen Numen;

/*======== (Create/Destroy) Numen ================*/
Numen* numen_create ();
Status numen_destroy (Numen* numen);

/*======== (set/get) name (char*) ================*/
Status numen_set_name (Numen* numen, char* name);
char* numen_get_name (Numen* numen);
Bool numen_has_name (Numen* numen, char* name);

/*======== (set/get) is_errante (Bool) ================*/
Bool numen_is_errant (Numen* numen);

/*======== (set/get) position ================*/
Status numen_set_pos_x (Numen* numen, int pos_x); /* tiene que estar entre los límites x */
int numen_get_pos_x (Numen* numen);

Status numen_set_pos_y (Numen* numen, int pos_y); /* tiene que estar entre los límites y */
int numen_get_pos_y (Numen* numen);

/*======== (set/get) gdesc() ================*/
Status numen_set_gdesc (Numen* numen, char* gdesc); // Será una línea de texto
char* numen_get_gdesc (Numen* numen);

/*======== (set/get) health ================*/
Status numen_set_health (Numen* numen, int health); // tiene que ser mayor o igual a 0 siempre, sí fuera a ser negativo, se mantiene en 0
int numen_get_health (Numen* numen);

/*======== (set/get) attack ================*/
Status numen_set_attack (Numen* numen, int attack);
int numen_get_attack (Numen* numen);

/*======== (set/get) energy ================*/
Status numen_set_energy (Numen* numen, int energy);
int numen_get_energy (Numen* numen);

/*======== (set/get) speed ================*/
Status numen_set_speed (Numen* numen, int speed);
int numen_get_speed (Numen* numen);

/*======== (set/get) skills ================*/
Status numen_set_skill (Numen* numen, Id skill_id);
Id numen_get_skill (Numen* numen, Id skill_id);

/*======== (set/get) following ================*/
Status numen_set_following (Numen* numen, Id following);
Id numen_get_following (Numen* numen);

/*======= (set/get) Id ========================*/
Status numen_set_id (Numen* numen, Id new_id);
Id numen_get_id (Numen* numen);

/*====== (set/get) corrupt ===================*/
Status numen_set_corrupt (Numen* numen, Bool corrupt);
Bool numen_get_corrupt (Numen* numen);

Status numen_print (Numen* numen);
#endif /* NUMEN_H */