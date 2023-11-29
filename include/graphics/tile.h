#ifndef TILE_H
#define TILE_H

#include <memory.h>

typedef struct tile {
	word data[8];
} tile_t, tileset_t[512];

typedef struct tile_8bpp {
	word data[16];
} tile_8bpp_t, tileset_8bpp_t[256];

void load_tileset(const tile_t tileset[], unsigned int size, int cbb);
void load_tileset_8bpp(const tile_8bpp_t tileset[], unsigned int size, int cbb);

#endif
