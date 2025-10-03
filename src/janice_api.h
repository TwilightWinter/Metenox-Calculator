#ifndef JANICE_API_H
#define JANICE_API_H

#include "calculator.h"

typedef struct {
	char code[32];
	double totalBuyPrice;
	double totalSplitPrice;
	double totalSellPrice;
} JaniceResult;

/**
 * Convert a MineralDB into the plain text body Janice expects.
 * Caller must free() the returned string.
 */
char *minerals_to_janice_body(const MineralDB *db);

/**
 * Send appraisal request to Janice API.
 * Returns 0 on success, non-zero on error.
 */
int janice_appraise(const MineralDB *db, JaniceResult *out);

#endif
