/**
 * @brief Test for Entity module
 * @file test_entity.c
 * @author Rafael y Violeta
 */
#include "entity.h"


void print_test(char *name, int result) {
    printf("Test: %-40s [%s]\n", name, result ? VERDE"PASS"RESET : ROJO"FAIL"RESET);
}

int main() {
    Entity *e = NULL;
    char *name_tmp = NULL, *gdesc_tmp = NULL, *msg_tmp = NULL;
    Status st;

    printf("--- STARTING ENTITY MODULE TESTS ---\n\n");

    /* 1. CREATION */
    e = entity_create();
    print_test("Entity creation", e != NULL);
    if (!e) return 1;

    /* 2. NAME (Dynamic Allocation) */
    st = entity_set_name(e, "Warrior");
    name_tmp = entity_get_name(e);
    print_test("Set/Get Name (Warrior)", st == OK && name_tmp != NULL && strcmp(name_tmp, "Warrior") == 0);
    free(name_tmp); /* Crucial: entity_get_name returns strdup */

    /* 3. GDESC (Dynamic Allocation) */
    st = entity_set_gdesc(e, "A brave knight");
    gdesc_tmp = entity_get_gdesc(e);
    print_test("Set/Get GDesc", st == OK && gdesc_tmp != NULL && strcmp(gdesc_tmp, "A brave knight") == 0);
    free(gdesc_tmp);

    /* 4. MESSAGE (Dynamic Allocation) */
    st = entity_set_message(e, "Hello traveler!");
    msg_tmp = entity_get_message(e);
    print_test("Set/Get Message", st == OK && msg_tmp != NULL && strcmp(msg_tmp, "Hello traveler!") == 0);
    free(msg_tmp);

    /* 5. HEALTH BOUNDARIES (Min: 0, Max: 10) */
    print_test("Health (Set 5 - Valid)", entity_set_health(e, 5) == OK);
    print_test("Health (Get 5)", entity_get_health(e) == 5);
    
    print_test("Health (Set 11 - Invalid)", entity_set_health(e, MAX_LIFE + 1) == ERROR);
    print_test("Health (Set -1 - Invalid)", entity_set_health(e, MIN_LIFE - 1) == ERROR);
    
    /* 6. OVERWRITING STRINGS (Testing memory cleanup) */
    st = entity_set_name(e, "Berserker");
    name_tmp = entity_get_name(e);
    print_test("Overwriting Name (Memory Cleanup)", st == OK && strcmp(name_tmp, "Berserker") == 0);
    free(name_tmp);

    /* 7. NULL POINTER PROTECTION */
    print_test("Null Entity protection (Set)", entity_set_name(NULL, "Test") == ERROR);
    print_test("Null Entity protection (Get)", entity_get_name(NULL) == NULL);

    /* 8. DESTRUCTION */
    st = entity_destroy(e);
    print_test("Entity destruction", st == OK);

    printf("\n--- TESTS COMPLETED ---\n");
    printf("Suggestion: Run with 'valgrind --leak-check=full ./test_entity' to verify strdup cleanups.\n");

    return 0;
}
