#include "../src/calculator.h"
#include "../src/yield_table.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper to make a fake moon
static Moon make_moon(const char *name, const char *ore, double fraction) {
	Moon m;
	memset(&m, 0, sizeof(m));
	strncpy(m.name, name, sizeof(m.name));
	m.ore_count = 1;
	strncpy(m.ores[0].name, ore, sizeof(m.ores[0].name));
	m.ores[0].fraction = fraction;
	return m;
}

// --- Tests ---

START_TEST(test_bitumens_full_fraction) {
	load_yields("data/yields.json");

	Moon m = make_moon("TestMoon1", "Bitumens", 1.0);
	MineralDB db = compute_moon(&m, 0.4);

	ck_assert_int_eq(db.count, 1);
	ck_assert_str_eq(db.minerals[0].name, "Hydrocarbons");

	// Expected: 21,600,000 * 0.4 / 1000 = 8640 units
	// Each 1000 m3 → 65 Hydrocarbons
	long long expected = 8640LL * 65;
	ck_assert_int_eq(db.minerals[0].quantity, expected);
}
END_TEST

START_TEST(test_vanadinite_full_fraction) {
	load_yields("data/yields.json");

	Moon m = make_moon("TestMoon2", "Vanadinite", 1.0);
	MineralDB db = compute_moon(&m, 0.4);

	// Vanadinite yields 40 Vanadium, 10 Silicates per 1000 m3
	long long units = 8640LL; // same as above

	int found_v = 0, found_s = 0;
	for (int i = 0; i < db.count; i++) {
		if (strcmp(db.minerals[i].name, "Vanadium") == 0) {
			ck_assert_int_eq(db.minerals[i].quantity, units * 40);
			found_v = 1;
		}
		if (strcmp(db.minerals[i].name, "Silicates") == 0) {
			ck_assert_int_eq(db.minerals[i].quantity, units * 10);
			found_s = 1;
		}
	}
	ck_assert(found_v);
	ck_assert(found_s);
}
END_TEST

START_TEST(test_fractional_bitumens) {
	load_yields("data/yields.json");

	Moon m = make_moon("TestMoon3", "Bitumens", 0.25);
	MineralDB db = compute_moon(&m, 0.4);

	long long units = (long long)((21600000.0 * 0.25 * 0.4) / 1000.0);
	long long expected = units * 65;

	ck_assert_int_eq(db.count, 1);
	ck_assert_str_eq(db.minerals[0].name, "Hydrocarbons");
	ck_assert_int_eq(db.minerals[0].quantity, expected);
}
END_TEST

Suite *calculator_suite(void) {
	Suite *s = suite_create("Calculator");

	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_bitumens_full_fraction);
	tcase_add_test(tc_core, test_vanadinite_full_fraction);
	tcase_add_test(tc_core, test_fractional_bitumens);
	suite_add_tcase(s, tc_core);

	return s;
}
