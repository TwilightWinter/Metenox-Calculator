#ifndef PARSER_H
#define PARSER_H

#define MAX_ORES_PER_MOON 16
#define MAX_MOONS 128

typedef struct {
	char name[64];	 // Ore name, e.g. "Bitumens"
	double fraction; // Fraction, e.g. 0.196226656437
} OreFraction;

typedef struct {
	char name[128]; // Moon header, e.g. "O-0HW8 VII - Moon 9"
	int ore_count;
	OreFraction ores[MAX_ORES_PER_MOON];
} Moon;

typedef struct {
	int moon_count;
	Moon moons[MAX_MOONS];
} MoonDB;

/**
 * Parse a moon input file.
 * Returns a newly allocated MoonDB* (caller must free with free_moondb),
 * or NULL on failure.
 */
MoonDB *parse_file(const char *filename);

/**
 * Free a MoonDB allocated by parse_file.
 */
void free_moondb(MoonDB *db);

#endif
