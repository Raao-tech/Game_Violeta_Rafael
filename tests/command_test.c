/**
 * @brief It tests the command module
 *
 * @file command_test.c
 * @author Rafael y Violeta
 * @version 2
 * @date 28-02-2026
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include "command.h"
#include "command_test.h"
#include "test.h"

#define MAX_TESTS 10

int main(int argc, char **argv) {
  int test = 0;
  int all = 1;

  if (argc < 2) {
    printf("Running all tests for module Command:\n");
  } else {
    test = atoi(argv[1]);
    all = 0;
    printf("Running test %d:\t", test);
    if (test < 1 || test > MAX_TESTS) {
      printf("Error: unknown test %d\n", test);
      exit(EXIT_SUCCESS);
    }
  }

  if (all || test == 1) test1_command_create();
  if (all || test == 2) test2_command_create();
  if (all || test == 3) test1_command_destroy();
  if (all || test == 4) test2_command_destroy();
  if (all || test == 5) test1_command_set_code();
  if (all || test == 6) test2_command_set_code();
  if (all || test == 7) test1_command_get_code();
  if (all || test == 8) test2_command_get_code();
  if (all || test == 9) test1_command_get_target();
  if (all || test == 10) test2_command_get_target();

  PRINT_PASSED_PERCENTAGE;

  return 0;
}

void test1_command_create() {
  Command *c = command_create();
  PRINT_TEST_RESULT(c != NULL);
  command_destroy(c);
}

void test2_command_create() {
  Command *c = command_create();
  PRINT_TEST_RESULT(command_get_code(c) == NO_CMD);
  command_destroy(c);
}

void test1_command_destroy() {
  Command *c = command_create();
  PRINT_TEST_RESULT(command_destroy(c) == OK);
}

void test2_command_destroy() {
  PRINT_TEST_RESULT(command_destroy(NULL) == ERROR);
}

void test1_command_set_code() {
  Command *c = command_create();
  PRINT_TEST_RESULT(command_set_code(c, TAKE) == OK);
  command_destroy(c);
}

void test2_command_set_code() {
  PRINT_TEST_RESULT(command_set_code(NULL, TAKE) == ERROR);
}

void test1_command_get_code() {
  Command *c = command_create();
  command_set_code(c, ATTACK);
  PRINT_TEST_RESULT(command_get_code(c) == ATTACK);
  command_destroy(c);
}

void test2_command_get_code() {
  PRINT_TEST_RESULT(command_get_code(NULL) == NO_CMD);
}

/* command_get_target: without user input, obj is NULL */
void test1_command_get_target() {
  Command *c = command_create();
  PRINT_TEST_RESULT(command_get_target(c) == NULL);
  command_destroy(c);
}

void test2_command_get_target() {
  PRINT_TEST_RESULT(command_get_target(NULL) == NULL);
}
