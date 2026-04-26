#include "numen.h"


/*Hay que definir más ataques, aunque sea 4 distintos ataques por 5 numens, osea 20 distintos*/
typedef enum{
    NO_SKILL,
    TAKLE,
    QUICK_ATTACK,
    STEAM_ERUPTION
}Skills_id;

struct _Skills
{
    Skills_id     id;
    char* name;
    int     radio;
    int     distancia;
    int    damage;
    int     coste;

};


typedef struct _Skills Skills;


/*
*    command
*    attack name_numen 2 1 
*/


