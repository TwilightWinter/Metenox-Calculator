#include "../src/yield_table.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>

// Test single-mineral ore
START_TEST(test_single_mineral_ore) {
	int count = load_yields("data/yields.json");
	ck_assert_int_gt(count, 0);

	const OreYield *oy = find_yield("Bitumens");
	ck_assert_ptr_nonnull(oy);
	ck_assert_str_eq(oy->ore, "Bitumens");
	ck_assert_str_eq(oy->tier, "R4");
	ck_assert_int_eq(oy->mineral_count, 1);

	ck_assert_str_eq(oy->minerals[0].name, "Hydrocarbons");
	ck_assert_int_eq(oy->minerals[0].units_per_1000m3, 65);
}
END_TEST

// Test multi-mineral ore
START_TEST(test_multi_mineral_ore) {
	int count = load_yields("data/yields.json");
	ck_assert_int_gt(count, 0);

	const OreYield *oy = find_yield("Vanadinite");
	ck_assert_ptr_nonnull(oy);
	ck_assert_str_eq(oy->ore, "Vanadinite");
	ck_assert_str_eq(oy->tier, "R16");
	ck_assert_int_eq(oy->mineral_count, 2);

	// Order depends on JSON parser, so check by name:
	int found_vanadium = 0, found_silicates = 0;
	for (int i = 0; i < oy->mineral_count; i++) {
		if (strcmp(oy->minerals[i].name, "Vanadium") == 0) {
			ck_assert_int_eq(oy->minerals[i].units_per_1000m3, 40);
			found_vanadium = 1;
		}
		if (strcmp(oy->minerals[i].name, "Silicates") == 0) {
			ck_assert_int_eq(oy->minerals[i].units_per_1000m3, 10);
			found_silicates = 1;
		}
	}
	ck_assert(found_vanadium);
	ck_assert(found_silicates);
}
END_TEST

Suite *yield_table_suite(void) {
	Suite *s = suite_create("YieldTable");

	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, test_single_mineral_ore);
	tcase_add_test(tc_core, test_multi_mineral_ore);
	suite_add_tcase(s, tc_core);

	return s;
}
