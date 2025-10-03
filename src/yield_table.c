#include "yield_table.h"
#include "../third_party/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORES 128

static OreYield ore_table[MAX_ORES];
static int ore_count = 0;

int load_yields(const char *filename) {
	FILE *f = fopen(filename, "rb");
	if (!f) {
		perror("fopen");
		return -1;
	}

	fseek(f, 0, SEEK_END);
	long len = ftell(f);
	rewind(f);

	char *data = malloc(len + 1);
	if (!data) {
		fclose(f);
		return -1;
	}
	fread(data, 1, len, f);
	data[len] = '\0';
	fclose(f);

	cJSON *root = cJSON_Parse(data);
	free(data);

	if (!root) {
		fprintf(stderr, "Error parsing JSON yields\n");
		return -1;
	}

	ore_count = 0;
	cJSON *ore_item = NULL;
	cJSON_ArrayForEach(ore_item, root) {
		if (ore_count >= MAX_ORES)
			break;

		const char *ore_name = ore_item->string;
		cJSON *tier = cJSON_GetObjectItem(ore_item, "tier");
		cJSON *minerals = cJSON_GetObjectItem(ore_item, "minerals");

		OreYield *oy = &ore_table[ore_count++];
		strncpy(oy->ore, ore_name, sizeof(oy->ore));
		oy->ore[sizeof(oy->ore) - 1] = '\0';

		if (cJSON_IsString(tier)) {
			strncpy(oy->tier, tier->valuestring, sizeof(oy->tier));
		} else {
			strcpy(oy->tier, "?");
		}

		oy->mineral_count = 0;
		cJSON *mineral = NULL;
		cJSON_ArrayForEach(mineral, minerals) {
			if (oy->mineral_count >= MAX_MINERALS)
				break;
			MineralYield *my = &oy->minerals[oy->mineral_count++];
			strncpy(my->name, mineral->string, sizeof(my->name));
			my->units_per_1000m3 = mineral->valueint;
		}
	}

	cJSON_Delete(root);
	return ore_count;
}

const OreYield *find_yield(const char *ore_name) {
	for (int i = 0; i < ore_count; i++) {
		if (strcmp(ore_table[i].ore, ore_name) == 0) {
			return &ore_table[i];
		}
	}
	return NULL;
}
