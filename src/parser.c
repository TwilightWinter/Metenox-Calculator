#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// trim leading/trailing whitespace
static void trim(char *s) {
	// trim leading
	while (isspace((unsigned char)*s)) {
		memmove(s, s + 1, strlen(s));
	}
	// trim trailing
	char *end = s + strlen(s) - 1;
	while (end >= s && isspace((unsigned char)*end)) {
		*end = '\0';
		end--;
	}
}

MoonDB *parse_file(const char *filename) {
	FILE *f = fopen(filename, "r");
	if (!f) {
		perror("fopen");
		return NULL;
	}

	MoonDB *db = calloc(1, sizeof(MoonDB));
	if (!db) {
		fclose(f);
		return NULL;
	}

	char line[512];
	Moon *current = NULL;

	while (fgets(line, sizeof(line), f)) {
		// strip trailing newline
		line[strcspn(line, "\r\n")] = '\0';

		// skip empty
		if (line[0] == '\0')
			continue;

		// check for leading space → ore line
		if (isspace((unsigned char)line[0])) {
			if (!current)
				continue; // no moon yet

			// tokenize: ore name and fraction
			char *saveptr;
			char *tok = strtok_r(line, " \t", &saveptr);
			if (!tok)
				continue;

			OreFraction ore;
			strncpy(ore.name, tok, sizeof(ore.name));
			ore.name[sizeof(ore.name) - 1] = '\0';

			tok = strtok_r(NULL, " \t", &saveptr);
			if (!tok)
				continue;

			ore.fraction = atof(tok);

			if (current->ore_count < MAX_ORES_PER_MOON) {
				current->ores[current->ore_count++] = ore;
			}
		} else {
			// new moon header
			if (db->moon_count >= MAX_MOONS)
				break;

			current = &db->moons[db->moon_count++];
			memset(current, 0, sizeof(Moon));

			strncpy(current->name, line, sizeof(current->name));
			current->name[sizeof(current->name) - 1] = '\0';
			trim(current->name);
		}
	}

	fclose(f);
	return db;
}

void free_moondb(MoonDB *db) { free(db); }
