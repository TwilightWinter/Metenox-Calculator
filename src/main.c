#include "calculator.h"
#include "janice_api.h"
#include "parser.h"
#include "yield_table.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

// ANSI colors
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

static void print_prices(const JaniceResult *jr) {
	const char *color;

	if (jr->totalSellPrice > 600e6)
		color = GREEN;
	else if (jr->totalSellPrice >= 450e6)
		color = YELLOW;
	else
		color = RED;

	printf("  Buy:   %.2f\n", jr->totalBuyPrice);
	printf("  Split: %.2f\n", jr->totalSplitPrice);
	printf("  Sell:  %s%.2f%s\n", color, jr->totalSellPrice, RESET);
	printf("  Link:  https://janice.e-351.com/a/%s\n\n", jr->code);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s moons.txt\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (load_yields("data/yields.json") < 0) {
		fprintf(stderr, "Failed to load yields\n");
		return EXIT_FAILURE;
	}

	if (curl_global_init(CURL_GLOBAL_DEFAULT) != 0) {
		fprintf(stderr, "Failed to init curl\n");
		return EXIT_FAILURE;
	}

	MoonDB *mdb = parse_file(argv[1]);
	if (!mdb) {
		fprintf(stderr, "Failed to parse moons file\n");
		curl_global_cleanup();
		return EXIT_FAILURE;
	}

	printf("%s",
	       "OOTE Metenox Moon Calculator (all yields for 30 days):\n\n");

	for (int i = 0; i < mdb->moon_count; i++) {
		Moon *m = &mdb->moons[i];
		printf("Moon: %s\n", m->name);

		MineralDB result = compute_moon(m, 0.4);

		for (int j = 0; j < result.count; j++) {
			printf("  %lld %s\n", result.minerals[j].quantity,
			       result.minerals[j].name);
		}

		JaniceResult jr;
		if (janice_appraise(&result, &jr) == 0) {
			print_prices(&jr);
		} else {
			printf("  (appraisal failed)\n\n");
		}
	}

	free_moondb(mdb);
	curl_global_cleanup();
	return EXIT_SUCCESS;
}
