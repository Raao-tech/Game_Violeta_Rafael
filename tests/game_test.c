/**
 * @brief It tests the game module
 *
 * @file game_test.c
 * @author Rafael y Violeta
 * @version 2
 * @date 28-02-2026
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_test.h"
#include "test.h"

#define MAX_TESTS 45

int main(int argc, char **argv) {
  int test = 0;
  int all = 1;

  if (argc < 2) {
    printf("Running all tests for module Game:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 || test > MAX_TESTS) {
      printf("Error: unknown test %d\n", test);
      exit(EXIT_SUCCESS);
    }
  }

  if (all || test == 1)  test1_game_create();
  if (all || test == 2)  test2_game_create();
  if (all || test == 3)  test1_game_destroy();
  if (all || test == 4)  test2_game_destroy();
  if (all || test == 5)  test1_game_get_player();
  if (all || test == 6)  test2_game_get_player();
  if (all || test == 7)  test1_game_add_space();
  if (all || test == 8)  test2_game_add_space();
  if (all || test == 9)  test1_game_get_space();
  if (all || test == 10) test2_game_get_space();
  if (all || test == 11) test3_game_get_space();
  if (all || test == 12) test1_game_add_object();
  if (all || test == 13) test2_game_add_object();
  if (all || test == 14) test1_game_get_object_by_id();
  if (all || test == 15) test2_game_get_object_by_id();
  if (all || test == 16) test1_game_get_object_by_name();
  if (all || test == 17) test2_game_get_object_by_name();
  if (all || test == 18) test1_game_get_object_location();
  if (all || test == 19) test2_game_get_object_location();
  if (all || test == 20) test3_game_get_object_location();
  if (all || test == 21) test1_game_add_character();
  if (all || test == 22) test2_game_add_character();
  if (all || test == 23) test1_game_get_character_by_id();
  if (all || test == 24) test2_game_get_character_by_id();
  if (all || test == 25) test1_game_get_character_by_name();
  if (all || test == 26) test2_game_get_character_by_name();
  if (all || test == 27) test1_game_get_character_location();
  if (all || test == 28) test2_game_get_character_location();
  if (all || test == 29) test1_game_get_object_at();
  if (all || test == 30) test2_game_get_object_at();
  if (all || test == 31) test1_game_get_character_at();
  if (all || test == 32) test2_game_get_character_at();
  if (all || test == 33) test1_game_get_space_id_at();
  if (all || test == 34) test2_game_get_space_id_at();
  if (all || test == 35) test1_game_get_n_spaces();
  if (all || test == 36) test2_game_get_n_spaces();
  if (all || test == 37) test1_game_get_n_objects();
  if (all || test == 38) test1_game_get_n_characters();
  if (all || test == 39) test1_game_set_finished();
  if (all || test == 40) test2_game_get_finished();
  if (all || test == 41) test1_game_set_last_command();
  if (all || test == 42) test1_game_set_last_cmd_status();
  if (all || test == 43) test2_game_get_last_cmd_status();

  PRINT_PASSED_PERCENTAGE;

  return 0;
}


/* ========== game_create / destroy ========== */

void test1_game_create() {
  Game *g = game_create();
  PRINT_TEST_RESULT(g != NULL);
  game_destroy(g);
}

void test2_game_create() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_n_spaces(g) == 0 && game_get_n_objects(g) == 0 && game_get_n_characters(g) == 0);
  game_destroy(g);
}

void test1_game_destroy() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_destroy(g) == OK);
}

void test2_game_destroy() {
  PRINT_TEST_RESULT(game_destroy(NULL) == ERROR);
}


/* ========== game_get_player ========== */

void test1_game_get_player() {
  Game *g = game_create();
  Player *p = player_create();
  player_set_id(p, 1);
  game_add_player(g, p);
  PRINT_TEST_RESULT(game_get_player(g) == p);
  game_destroy(g);
}

void test2_game_get_player() {
  PRINT_TEST_RESULT(game_get_player(NULL) == NULL);
}


/* ========== game_add_space / get_space ========== */

void test1_game_add_space() {
  Game *g = game_create();
  Space *s = space_create();
  space_set_id(s, 11);
  PRINT_TEST_RESULT(game_add_space(g, s) == OK);
  game_destroy(g);
}

void test2_game_add_space() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_add_space(g, NULL) == ERROR);
  game_destroy(g);
}

void test1_game_get_space() {
  Game *g = game_create();
  Space *s = space_create();
  space_set_id(s, 11);
  game_add_space(g, s);
  PRINT_TEST_RESULT(game_get_space(g, 11) == s);
  game_destroy(g);
}

void test2_game_get_space() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_space(g, 999) == NULL);
  game_destroy(g);
}

void test3_game_get_space() {
  PRINT_TEST_RESULT(game_get_space(NULL, 11) == NULL);
}


/* ========== game_add_object / get_object_by_id / by_name ========== */

void test1_game_add_object() {
  Game *g = game_create();
  Object *o = obj_create();
  obj_set_id(o, 21);
  PRINT_TEST_RESULT(game_add_object(g, o) == OK);
  game_destroy(g);
}

void test2_game_add_object() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_add_object(g, NULL) == ERROR);
  game_destroy(g);
}

void test1_game_get_object_by_id() {
  Game *g = game_create();
  Object *o = obj_create();
  obj_set_id(o, 21);
  game_add_object(g, o);
  PRINT_TEST_RESULT(game_get_object_by_id(g, 21) == o);
  game_destroy(g);
}

void test2_game_get_object_by_id() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_object_by_id(g, 999) == NULL);
  game_destroy(g);
}

void test1_game_get_object_by_name() {
  Game *g = game_create();
  Object *o = obj_create();
  obj_set_id(o, 21);
  obj_set_name(o, "Sword");
  game_add_object(g, o);
  PRINT_TEST_RESULT(game_get_object_by_name(g, "Sword") == o);
  game_destroy(g);
}

void test2_game_get_object_by_name() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_object_by_name(g, "NonExistent") == NULL);
  game_destroy(g);
}


/* ========== game_get_object_location ========== */

void test1_game_get_object_location() {
  Game *g = game_create();
  Space *s = space_create();
  Object *o = obj_create();
  space_set_id(s, 11);
  obj_set_id(o, 21);
  game_add_space(g, s);
  game_add_object(g, o);
  Position dummy = {0, 0};
  space_set_object(s, 21, dummy);
  PRINT_TEST_RESULT(game_get_object_location(g, 21) == 11);
  game_destroy(g);
}

/* Object not in any space (in player inventory) */
void test2_game_get_object_location() {
  Game *g = game_create();
  Object *o = obj_create();
  obj_set_id(o, 21);
  game_add_object(g, o);
  PRINT_TEST_RESULT(game_get_object_location(g, 21) == NO_ID);
  game_destroy(g);
}

void test3_game_get_object_location() {
  PRINT_TEST_RESULT(game_get_object_location(NULL, 21) == NO_ID);
}


/* ========== game_add_character / get_character_by_id / by_name ========== */

void test1_game_add_character() {
  Game *g = game_create();
  Character *c = character_create();
  character_set_id(c, 50);
  PRINT_TEST_RESULT(game_add_character(g, c) == OK);
  game_destroy(g);
}

void test2_game_add_character() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_add_character(g, NULL) == ERROR);
  game_destroy(g);
}

void test1_game_get_character_by_id() {
  Game *g = game_create();
  Character *c = character_create();
  character_set_id(c, 50);
  game_add_character(g, c);
  PRINT_TEST_RESULT(game_get_character_by_id(g, 50) == c);
  game_destroy(g);
}

void test2_game_get_character_by_id() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_character_by_id(g, 999) == NULL);
  game_destroy(g);
}

void test1_game_get_character_by_name() {
  Game *g = game_create();
  Character *c = character_create();
  character_set_id(c, 50);
  character_set_name(c, "Goblin");
  game_add_character(g, c);
  PRINT_TEST_RESULT(game_get_character_by_name(g, "Goblin") == c);
  game_destroy(g);
}

void test2_game_get_character_by_name() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_character_by_name(g, "NonExistent") == NULL);
  game_destroy(g);
}


/* ========== game_get_character_location ========== */

void test1_game_get_character_location() {
  Game *g = game_create();
  Space *s = space_create();
  Character *c = character_create();
  space_set_id(s, 11);
  character_set_id(c, 50);
  game_add_space(g, s);
  game_add_character(g, c);
  space_set_character(s, 50);
  PRINT_TEST_RESULT(game_get_character_location(g, 50) == 11);
  game_destroy(g);
}

void test2_game_get_character_location() {
  PRINT_TEST_RESULT(game_get_character_location(NULL, 50) == NO_ID);
}


/* ========== game_get_object_at / character_at / space_id_at ========== */

void test1_game_get_object_at() {
  Game *g = game_create();
  Object *o = obj_create();
  obj_set_id(o, 21);
  game_add_object(g, o);
  PRINT_TEST_RESULT(game_get_object_at(g, 0) == o);
  game_destroy(g);
}

void test2_game_get_object_at() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_object_at(g, 99) == NULL);
  game_destroy(g);
}

void test1_game_get_character_at() {
  Game *g = game_create();
  Character *c = character_create();
  character_set_id(c, 50);
  game_add_character(g, c);
  PRINT_TEST_RESULT(game_get_character_at(g, 0) == c);
  game_destroy(g);
}

void test2_game_get_character_at() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_character_at(g, 99) == NULL);
  game_destroy(g);
}

void test1_game_get_space_id_at() {
  Game *g = game_create();
  Space *s = space_create();
  space_set_id(s, 11);
  game_add_space(g, s);
  PRINT_TEST_RESULT(game_get_space_id_at(g, 0) == 11);
  game_destroy(g);
}

void test2_game_get_space_id_at() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_space_id_at(g, 99) == NO_ID);
  game_destroy(g);
}


/* ========== game_get_n_* ========== */

void test1_game_get_n_spaces() {
  Game *g = game_create();
  Space *s = space_create();
  space_set_id(s, 11);
  game_add_space(g, s);
  PRINT_TEST_RESULT(game_get_n_spaces(g) == 1);
  game_destroy(g);
}

void test2_game_get_n_spaces() {
  PRINT_TEST_RESULT(game_get_n_spaces(NULL) == -1);
}

void test1_game_get_n_objects() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_n_objects(g) == 0);
  game_destroy(g);
}

void test1_game_get_n_characters() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_n_characters(g) == 0);
  game_destroy(g);
}


/* ========== game state ========== */

void test1_game_set_finished() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_set_finished(g, TRUE) == OK && game_get_finished(g) == TRUE);
  game_destroy(g);
}

void test2_game_get_finished() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_get_finished(g) == FALSE);
  game_destroy(g);
}

void test1_game_set_last_command() {
  Game *g = game_create();
  Command *cmd = game_get_last_command(g);
  PRINT_TEST_RESULT(cmd != NULL);
  game_destroy(g);
}

void test1_game_set_last_cmd_status() {
  Game *g = game_create();
  PRINT_TEST_RESULT(game_set_last_cmd_status(g, ERROR) == OK);
  game_destroy(g);
}

void test2_game_get_last_cmd_status() {
  Game *g = game_create();
  game_set_last_cmd_status(g, ERROR);
  PRINT_TEST_RESULT(game_get_last_cmd_status(g) == ERROR);
  game_destroy(g);
}
