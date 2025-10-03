#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "parser.h"
#include "yield_table.h"

#define MAX_MINERALS_TOTAL 128

typedef struct {
	char name[64];	    // mineral name, e.g. "Hydrocarbons"
	long long quantity; // total units
} MineralTally;

typedef struct {
	int count;
	MineralTally minerals[MAX_MINERALS_TOTAL];
} MineralDB;

/**
 * Compute minerals for one moon.
 * - monthly volume = 21,600,000 m3
 * - multiply by ore fraction
 * - apply metenox factor (0.4)
 * - truncate to nearest 1000 m3
 * - convert to minerals via yield table
 *
 * Returns a MineralDB containing totals.
 */
MineralDB compute_moon(const Moon *moon, double metenox_factor);

#endif
