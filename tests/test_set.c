/**
 * @brief Professional Unit Testing for the Set Module
 * @file test_set.c
 * @author Rafael y Violeta
 */

#include <stdio.h>
#include <stdlib.h>
#include "set.h"


void print_test(char *msg, int result) {
    printf("Test: %-45s [%s]\n", msg, result ? VERDE"PASS"RESET : ROJO"FAIL"RESET);
}

int main() {
    Set *s = NULL;
    Id id_out;
    Status st;

    printf("--- STARTING INTENSIVE SET MODULE TESTS ---\n\n");

    /* 1. TEST: set_creat */
    s = set_creat();
    print_test("1A: set_creat (pointer not NULL)", s != NULL);
    print_test("1B: set_creat (initial state n_ids == 0)", set_is_empty(s) == TRUE);

    /* 2. TEST: set_is_empty */
    print_test("2A: set_is_empty (New Set is TRUE)", set_is_empty(s) == TRUE);
    set_add(s, 100);
    print_test("2B: set_is_empty (After add is FALSE)", set_is_empty(s) == FALSE);

    /* 3. TEST: set_add (Limit Testing) */
    st = set_add(s, 200);
    print_test("3A: set_add (Valid ID return OK)", st == OK);
    st = set_add(NULL, 300);
    print_test("3B: set_add (NULL pointer return ERROR)", st == ERROR);

    /* 4. TEST: set_pop (LIFO and Underflow) */
    id_out = set_pop(s); // Should pop 200
    print_test("4A: set_pop (Valid LIFO - expected 200)", id_out == 200);
    
    set_pop(s); // Pops 100, now empty
    id_out = set_pop(s);
    print_test("4B: set_pop (Underflow on empty set)", id_out == NO_ID);

    /* 5. TEST: Memory Stress (Adding/Popping multiple) */
    printf("\n--- Stress Testing ---\n");
    int i, count = 0;
    for(i=1; i<=10; i++) {
        if(set_add(s, (Id)i) == OK) count++;
    }
    print_test("5A: Massive add (10 elements)", count == 10);
    
    count = 0;
    for(i=0; i<10; i++) {
        if(set_pop(s) != NO_ID) count++;
    }
    print_test("5B: Massive pop (10 elements)", count == 10);
    print_test("5C: Set is empty after stress", set_is_empty(s) == TRUE);

    /* 6. TEST: set_destroy */
    print_test("6A: set_destroy (Valid pointer)", set_destroy(s) == OK);
    print_test("6B: set_destroy (NULL pointer safety)", set_destroy(NULL) == ERROR);

    /* 7. TEST: set_print (Visual check) */
    printf("\n--- Visual Check (Printing Empty Set) ---\n");
    s = set_creat();
    set_add(s, 777);
    set_print(stdout, s);
    set_destroy(s);

    printf("\n--- ALL SET TESTS COMPLETED ---\n");
    return 0;
}