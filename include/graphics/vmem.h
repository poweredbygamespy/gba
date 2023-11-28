#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory.h>

typedef struct palette {
	hword colors[16];
} palette_t, palette_bank_t[16];

typedef struct tile {
	word data[8];
} tile_t, tileset_t[512];

typedef struct dtile {
	word data[16];
} dtile_t, dtileset_t[256];

typedef hword tilemap_entry_t, tilemap_t[1024];
typedef byte affine_tilemap_entry_t, affine_tilemap_t[2048];

void setup(void);
void load_tileset(const tile_t[], unsigned int, int);
void load_tilemap(const tilemap_t, int);
void update_tile(int, int, tilemap_entry_t);
void update_tile_affine(int sbb, int offset, affine_tilemap_entry_t te);
tilemap_entry_t flip_tilemap_entry_h(tilemap_entry_t);
tilemap_entry_t flip_tilemap_entry_v(tilemap_entry_t);
tilemap_entry_t set_tilemap_entry_palette(tilemap_entry_t, byte);
void shift_up(int, unsigned int);
void load_palette(palette_t, int);

#endif
