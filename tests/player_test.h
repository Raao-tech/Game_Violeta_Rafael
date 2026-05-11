/**
 * @brief It declares the tests for the player module
 *
 * @file player_test.h
 * @author Rafael y Violeta
 * @version 2
 * @date 28-02-2026
 * @copyright GNU Public License
 */

#ifndef PLAYER_TEST_H
#define PLAYER_TEST_H

/* player_create */
void test1_player_create();
void test2_player_create();

/* player_destroy */
void test1_player_destroy();
void test2_player_destroy();

/* player_set_id / get_id */
void test1_player_set_id();
void test2_player_set_id();
void test1_player_get_id();
void test2_player_get_id();

/* player_set_name / get_name */
void test1_player_set_name();
void test2_player_set_name();
void test3_player_set_name();
void test1_player_get_name();
void test2_player_get_name();

/* player_has_name */
void test1_player_has_name();
void test2_player_has_name();
void test3_player_has_name();

/* player_set_health / get_health */
void test1_player_set_health();
void test2_player_set_health();
void test1_player_get_health();
void test2_player_get_health();

/* player_set_max_objects */
void test1_player_set_max_objects();
void test2_player_set_max_objects();
void test3_player_set_max_objects();

/* player_add_object / contains / delete / get_n */
void test1_player_set_attack();
void test2_player_set_attack();
void test1_player_get_attack();
void test2_player_get_attack();

/* player_add_object / contains / delete / get_n */
void test1_player_add_object();
void test2_player_add_object();
void test1_player_add_object_full_inventory();
void test1_player_contains_object();
void test2_player_contains_object();
void test3_player_contains_object();
void test1_player_delete_object();
void test2_player_delete_object();
void test1_player_get_n_objects();
void test2_player_get_n_objects();
void test1_player_empty_inventory();

/* player_set_location / get_location */
void test1_player_set_location();
void test2_player_set_location();
void test3_player_set_location();
void test1_player_get_location();
void test2_player_get_location();

/* player_set_gdesc / get_gdesc */
void test1_player_set_gdesc();
void test2_player_set_gdesc();
void test1_player_get_gdesc();
void test2_player_get_gdesc();

/* player_set_message / get_message */
void test1_player_set_message();
void test2_player_set_message();
void test1_player_get_message();
void test2_player_get_message();

/* player_set_zone / get_zone */
void test1_player_set_zone();
void test2_player_set_zone();
void test3_player_set_zone();
void test1_player_get_zone();
void test2_player_get_zone();

#endif
