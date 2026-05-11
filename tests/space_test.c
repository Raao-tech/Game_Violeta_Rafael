/**
 * @brief It tests the space module (I3)
 *
 * Updated for I3: removed north/south/east/west tests (topology
 * is now handled by the Link module), added discovered tests (F12).
 *
 * @file space_test.c
 * @author Rafael y Violeta
 * @version 3
 * @date 12-04-2026
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "space.h"
#include "space_test.h"
#include "test.h"

#define MAX_TESTS 39

int main(int argc, char **argv) {
  int test = 0;
  int all = 1;

  if (argc < 2) {
    printf("Running all tests for module Space:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 || test > MAX_TESTS) {
      printf("Error: unknown test %d\n", test);
      exit(EXIT_SUCCESS);
    }
  }

  /* space_create / destroy */
  if (all || test == 1)  test1_space_create();
  if (all || test == 2)  test2_space_create();
  if (all || test == 3)  test1_space_destroy();
  if (all || test == 4)  test2_space_destroy();

  /* space_set_id / get_id */
  if (all || test == 5)  test1_space_set_id();
  if (all || test == 6)  test2_space_set_id();
  if (all || test == 7)  test1_space_get_id();
  if (all || test == 8)  test2_space_get_id();

  /* space_set_name / get_name */
  if (all || test == 9)  test1_space_set_name();
  if (all || test == 10) test2_space_set_name();
  if (all || test == 11) test3_space_set_name();
  if (all || test == 12) test1_space_get_name();
  if (all || test == 13) test2_space_get_name();

  /* space objects */
  if (all || test == 14) test1_space_set_object();
  if (all || test == 15) test2_space_set_object();
  if (all || test == 16) test1_space_contains_object();
  if (all || test == 17) test2_space_contains_object();
  if (all || test == 18) test3_space_contains_object();
  if (all || test == 19) test1_space_remove_object();
  if (all || test == 20) test2_space_remove_object();
  if (all || test == 21) test1_space_get_n_objects();
  if (all || test == 22) test2_space_get_n_objects();

  /* space characters */
  if (all || test == 23) test1_space_set_character();
  if (all || test == 24) test2_space_set_character();
  if (all || test == 25) test1_space_contains_character();
  if (all || test == 26) test2_space_contains_character();
  if (all || test == 27) test1_space_remove_character();
  if (all || test == 28) test2_space_remove_character();
  if (all || test == 29) test1_space_get_n_characters();
  if (all || test == 30) test2_space_get_n_characters();

  /* space discovered (F12) */
  if (all || test == 31) test1_space_set_discovered();
  if (all || test == 32) test2_space_set_discovered();
  if (all || test == 33) test1_space_get_discovered();
  if (all || test == 34) test2_space_get_discovered();

  /* space gdesc */
  if (all || test == 35) test1_space_set_gdesc_line();
  if (all || test == 36) test2_space_set_gdesc_line();
  if (all || test == 37) test3_space_set_gdesc_line();
  if (all || test == 38) test1_space_get_gdesc();
  if (all || test == 39) test2_space_get_gdesc();

  PRINT_PASSED_PERCENTAGE;

  return 0;
}


/* ========== space_create ========== */

void test1_space_create() {
  Space *s = space_create();
  PRINT_TEST_RESULT(s != NULL);
  space_destroy(s);
}

void test2_space_create() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_get_id(s) == NO_ID);
  space_destroy(s);
}


/* ========== space_destroy ========== */

void test1_space_destroy() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_destroy(s) == OK);
}

void test2_space_destroy() {
  PRINT_TEST_RESULT(space_destroy(NULL) == ERROR);
}


/* ========== space_set_id / get_id ========== */

void test1_space_set_id() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_set_id(s, 25) == OK);
  space_destroy(s);
}

void test2_space_set_id() {
  PRINT_TEST_RESULT(space_set_id(NULL, 25) == ERROR);
}

void test1_space_get_id() {
  Space *s = space_create();
  space_set_id(s, 25);
  PRINT_TEST_RESULT(space_get_id(s) == 25);
  space_destroy(s);
}

void test2_space_get_id() {
  PRINT_TEST_RESULT(space_get_id(NULL) == NO_ID);
}


/* ========== space_set_name / get_name ========== */

void test1_space_set_name() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_set_name(s, "Entry") == OK);
  space_destroy(s);
}

void test2_space_set_name() {
  PRINT_TEST_RESULT(space_set_name(NULL, "Entry") == ERROR);
}

void test3_space_set_name() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_set_name(s, NULL) == ERROR);
  space_destroy(s);
}

void test1_space_get_name() {
  Space *s = space_create();
  space_set_name(s, "Entry");
  PRINT_TEST_RESULT(strcmp(space_get_name(s), "Entry") == 0);
  space_destroy(s);
}

void test2_space_get_name() {
  PRINT_TEST_RESULT(space_get_name(NULL) == NULL);
}


/* ========== space objects: set / contains / remove / get_n ========== */

void test1_space_set_object() {
  Space *s = space_create();
  Position dummy = {0, 0};
  PRINT_TEST_RESULT(space_set_object(s, 21, dummy) == OK);
  space_destroy(s);
}

void test2_space_set_object() {
  Position dummy = {0, 0};
  PRINT_TEST_RESULT(space_set_object(NULL, 21, dummy) == ERROR);
}

void test1_space_contains_object() {
  Space *s = space_create();
  Position dummy = {0, 0};
  space_set_object(s, 21, dummy);
  PRINT_TEST_RESULT(space_contains_object(s, 21) == TRUE);
  space_destroy(s);
}

void test2_space_contains_object() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_contains_object(s, 999) == FALSE);
  space_destroy(s);
}

void test3_space_contains_object() {
  PRINT_TEST_RESULT(space_contains_object(NULL, 21) == FALSE);
}

void test1_space_remove_object() {
  Space *s = space_create();
  Position dummy = {0, 0};
  space_set_object(s, 21, dummy);
  PRINT_TEST_RESULT(space_remove_object(s, 21, dummy) == OK && space_contains_object(s, 21) == FALSE);
  space_destroy(s);
}

void test2_space_remove_object() {
  Space *s = space_create();
  Position dummy = {0, 0};
  PRINT_TEST_RESULT(space_remove_object(s, 999, dummy) == ERROR);
  space_destroy(s);
}

void test1_space_get_n_objects() {
  Space *s = space_create();
  Position dummy1 = {0, 0};
  Position dummy2 = {1, 0};
  space_set_object(s, 21, dummy1);
  space_set_object(s, 22, dummy2);
  PRINT_TEST_RESULT(space_get_n_objects(s) == 2);
  space_destroy(s);
}

void test2_space_get_n_objects() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_get_n_objects(s) == 0);
  space_destroy(s);
}


/* ========== space characters: set / contains / remove / get_n ========== */

void test1_space_set_character() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_set_character(s, 50) == OK);
  space_destroy(s);
}

void test2_space_set_character() {
  PRINT_TEST_RESULT(space_set_character(NULL, 50) == ERROR);
}

void test1_space_contains_character() {
  Space *s = space_create();
  space_set_character(s, 50);
  PRINT_TEST_RESULT(space_contains_character(s, 50) == TRUE);
  space_destroy(s);
}

void test2_space_contains_character() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_contains_character(s, 999) == FALSE);
  space_destroy(s);
}

void test1_space_remove_character() {
  Space *s = space_create();
  space_set_character(s, 50);
  PRINT_TEST_RESULT(space_remove_character(s, 50) == OK && space_contains_character(s, 50) == FALSE);
  space_destroy(s);
}

void test2_space_remove_character() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_remove_character(s, 999) == ERROR);
  space_destroy(s);
}

void test1_space_get_n_characters() {
  Space *s = space_create();
  space_set_character(s, 50);
  space_set_character(s, 51);
  PRINT_TEST_RESULT(space_get_n_characters(s) == 2);
  space_destroy(s);
}

void test2_space_get_n_characters() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_get_n_characters(s) == 0);
  space_destroy(s);
}


/* ========== space_set_discovered / get_discovered (F12) ========== */

/* Test 1: set discovered to TRUE returns OK */
void test1_space_set_discovered() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_set_discovered(s, TRUE) == OK);
  space_destroy(s);
}

/* Test 2: set discovered with NULL returns ERROR */
void test2_space_set_discovered() {
  PRINT_TEST_RESULT(space_set_discovered(NULL, TRUE) == ERROR);
}

/* Test 1: get discovered returns the value that was set */
void test1_space_get_discovered() {
  Space *s = space_create();
  space_set_discovered(s, TRUE);
  PRINT_TEST_RESULT(space_get_discovered(s) == TRUE);
  space_destroy(s);
}

/* Test 2: newly created space starts as NOT discovered */
void test2_space_get_discovered() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_get_discovered(s) == FALSE);
  space_destroy(s);
}


/* ========== space_set_gdesc_line / get_gdesc ========== */

void test1_space_set_gdesc_line() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_set_gdesc(s, "  _ !#") == OK);
  space_destroy(s);
}

void test2_space_set_gdesc_line() {
  PRINT_TEST_RESULT(space_set_gdesc(NULL, "  _ !#") == ERROR);
}

/* NULL desc */
void test3_space_set_gdesc_line() {
  Space *s = space_create();
  PRINT_TEST_RESULT(space_set_gdesc(s, NULL) == ERROR);
  space_destroy(s);
}

void test1_space_get_gdesc() {
  Space *s = space_create();
  space_set_gdesc(s, "  _ !#");
  PRINT_TEST_RESULT(space_get_gdesc(s) != NULL && strcmp(space_get_gdesc(s), "  _ !#") == 0);
  space_destroy(s);
}

void test2_space_get_gdesc() {
  PRINT_TEST_RESULT(space_get_gdesc(NULL) == NULL);
}