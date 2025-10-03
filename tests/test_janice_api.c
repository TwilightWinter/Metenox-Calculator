#include "../src/janice_api.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Build a synthetic MineralDB with 2 entries
static MineralDB make_test_db(void) {
	MineralDB db;
	db.count = 2;
	strncpy(db.minerals[0].name, "Hydrocarbons",
		sizeof(db.minerals[0].name));
	db.minerals[0].quantity = 1234567;
	strncpy(db.minerals[1].name, "Silicates", sizeof(db.minerals[1].name));
	db.minerals[1].quantity = 8900;
	return db;
}

START_TEST(test_body_formatting) {
	MineralDB db = make_test_db();

	char *body = minerals_to_janice_body(&db);
	ck_assert_ptr_nonnull(body);

	// Expect commas and newline after each line
	const char *expected1 = "1,234,567 Hydrocarbons\n";
	const char *expected2 = "8,900 Silicates\n";

	// Check substring presence (order matters here)
	ck_assert_msg(strstr(body, expected1) != NULL, "Missing first line");
	ck_assert_msg(strstr(body, expected2) != NULL, "Missing second line");

	// Make sure the lines are in the correct order
	char *pos1 = strstr(body, expected1);
	char *pos2 = strstr(body, expected2);
	ck_assert_ptr_nonnull(pos1);
	ck_assert_ptr_nonnull(pos2);
	ck_assert(pos1 < pos2);

	free(body);
}
END_TEST

Suite *janice_api_suite(void) {
	Suite *s = suite_create("JaniceAPI");

	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_body_formatting);
	suite_add_tcase(s, tc_core);

	return s;
}
