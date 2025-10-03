#include <check.h>
#include <stdlib.h>

// Forward declarations of suite creators
Suite *calculator_suite(void);
Suite *janice_api_suite(void);
Suite *yield_table_suite(void);
Suite *parser_suite(void);

int main(void) {
	int number_failed = 0;

	SRunner *sr = srunner_create(yield_table_suite());
	srunner_add_suite(sr, janice_api_suite());
	srunner_add_suite(sr, parser_suite());
	srunner_add_suite(sr, calculator_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
