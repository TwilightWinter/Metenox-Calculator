#ifndef YIELD_TABLE_H
#define YIELD_TABLE_H

#include <stddef.h>

#define MAX_MINERALS 8

typedef struct {
	char name[64];
	int units_per_1000m3;
} MineralYield;

typedef struct {
	char ore[64];
	char tier[8];
	int mineral_count;
	MineralYield minerals[MAX_MINERALS];
} OreYield;

/**
 * Load yields from JSON file into memory.
 * Returns the number of ore types loaded, or -1 on error.
 */
int load_yields(const char *filename);

/**
 * Lookup an ore yield entry by name.
 * Returns NULL if not found.
 */
const OreYield *find_yield(const char *ore_name);

#endif
