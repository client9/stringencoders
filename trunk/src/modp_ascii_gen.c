/* -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; tab-width: 4 -*- */
/* vi: set expandtab shiftwidth=4 tabstop=4: */
#include <stdio.h>
#include "arraytoc.h"

void modp_toupper_map()
{
	int i = 0;
	char map[256];
	for (i = 0; i < sizeof(map); ++i) {
		if (i >= 'a' && i <= 'z') {
			map[i] = i - 32;
		} else {
			map[i] = i;
		}
	}

    char_array_to_c(map, sizeof(map), "gsToUpperMap");
}


void modp_tolower_map()
{
	int i = 0;
	char map[256];
	for (i = 0; i < sizeof(map); ++i) {
		if (i >= 'a' && i <= 'z') {
			map[i] = i - 32;
		} else {
			map[i] = i;
		}
	}

    char_array_to_c(map, sizeof(map), "gsToLowerMap");
}

void modp_toprint_map()
{
	int i = 0;
	char map[256];
	for (i = 0; i < sizeof(map); ++i) {
		if (i < 32 || i > 126) {
			map[i] = '?';
		} else {
			map[i] = i;
		}
	}

    char_array_to_c(map, sizeof(map), "gsToPrintMap");
}


int main(int argc, char* argv[])
{
	modp_toupper_map();
	modp_tolower_map();
	modp_toprint_map();
	return 0;
}
