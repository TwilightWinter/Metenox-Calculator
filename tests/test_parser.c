#include "../src/parser.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Helpers to create temp files ---
static const char *testfile1 = "tests/tmp1.txt";
static const char *testfile2 = "tests/tmp2.txt";

static void write_file(const char *path, const char *contents) {
	FILE *f = fopen(path, "w");
	if (!f) {
		perror("fopen");
		exit(1);
	}
	fputs(contents, f);
	fclose(f);
}

// --- Tests ---

START_TEST(test_parse_one_moon) {
	const char *input = "TESTSYSTEM I - Moon 1\n"
			    "    Bitumens 0.5 junk1 junk2\n"
			    "    Vanadinite 0.25 junk\n";

	write_file(testfile1, input);

	MoonDB *db = parse_file(testfile1);
	ck_assert_ptr_nonnull(db);
	ck_assert_int_eq(db->moon_count, 1);

	Moon *m = &db->moons[0];
	ck_assert_str_eq(m->name, "TESTSYSTEM I - Moon 1");
	ck_assert_int_eq(m->ore_count, 2);

	ck_assert_str_eq(m->ores[0].name, "Bitumens");
	ck_assert_double_eq_tol(m->ores[0].fraction, 0.5, 1e-6);

	ck_assert_str_eq(m->ores[1].name, "Vanadinite");
	ck_assert_double_eq_tol(m->ores[1].fraction, 0.25, 1e-6);

	free_moondb(db);
}
END_TEST

START_TEST(test_parse_two_moons) {
	const char *input = "SYS1 II - Moon 2\n"
			    "    Zeolites 0.1 junk\n"
			    "SYS2 III - Moon 5\n"
			    "    Titanite 0.2 morejunk\n"
			    "    Sylvite 0.3 here\n";

	write_file(testfile2, input);

	MoonDB *db = parse_file(testfile2);
	ck_assert_ptr_nonnull(db);
	ck_assert_int_eq(db->moon_count, 2);

	Moon *m1 = &db->moons[0];
	ck_assert_str_eq(m1->name, "SYS1 II - Moon 2");
	ck_assert_int_eq(m1->ore_count, 1);
	ck_assert_str_eq(m1->ores[0].name, "Zeolites");
	ck_assert_double_eq_tol(m1->ores[0].fraction, 0.1, 1e-6);

	Moon *m2 = &db->moons[1];
	ck_assert_str_eq(m2->name, "SYS2 III - Moon 5");
	ck_assert_int_eq(m2->ore_count, 2);
	ck_assert_str_eq(m2->ores[0].name, "Titanite");
	ck_assert_double_eq_tol(m2->ores[0].fraction, 0.2, 1e-6);
	ck_assert_str_eq(m2->ores[1].name, "Sylvite");
	ck_assert_double_eq_tol(m2->ores[1].fraction, 0.3, 1e-6);

	free_moondb(db);
}
END_TEST

Suite *parser_suite(void) {
	Suite *s = suite_create("Parser");

	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_parse_one_moon);
	tcase_add_test(tc_core, test_parse_two_moons);
	suite_add_tcase(s, tc_core);

	return s;
}
