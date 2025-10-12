#include "calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MONTHLY_VOLUME 21600000.0 // 30 days * 24h * 30,000 m3/h

// helper: add quantity to MineralDB
static void add_mineral(MineralDB *db, const char *name, long long qty) {
	for (int i = 0; i < db->count; i++) {
		if (strcmp(db->minerals[i].name, name) == 0) {
			db->minerals[i].quantity += qty;
			return;
		}
	}
	if (db->count < MAX_MINERALS_TOTAL) {
		strncpy(db->minerals[db->count].name, name,
			sizeof(db->minerals[db->count].name));
		db->minerals[db->count]
		    .name[sizeof(db->minerals[db->count].name) - 1] = '\0';
		db->minerals[db->count].quantity = qty;
		db->count++;
	}
}

MineralDB compute_moon(const Moon *moon, double metenox_factor) {
	MineralDB db;
	db.count = 0;

	for (int i = 0; i < moon->ore_count; i++) {
		OreFraction ore = moon->ores[i];
		const OreYield *oy = find_yield(ore.name);
		if (!oy) {
			fprintf(stderr, "Warning: no yield data for ore %s\n",
				ore.name);
			continue;
		}

		// Step 1: raw volume
		double raw_volume = MONTHLY_VOLUME * ore.fraction;

		// Step 2: apply metenox factor
		double post_metenox = raw_volume * metenox_factor;

		// Step 3: truncate to nearest 1000
		long long units = (long long)(post_metenox / 1000.0);

		if (units <= 0)
			continue; // nothing refinable

		// Step 4: add minerals
		for (int j = 0; j < oy->mineral_count; j++) {
			const MineralYield *my = &oy->minerals[j];
			long long qty = units * my->units_per_1000m3;
			add_mineral(&db, my->name, qty);
		}
	}

	return db;
}
