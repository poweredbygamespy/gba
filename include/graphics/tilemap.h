#ifndef TILEMAP_H
#define TILEMAP_H

typedef struct palette {
	unsigned short int colors[16];
} palette_t, palette_bank_t[16];

typedef struct tile {
	unsigned int data[8];
} tile_t, tileset_t[512];

typedef unsigned short tilemap_entry_t, tilemap_t[1024];

void setup(void);
void load_tileset(const tile_t[], unsigned int, int);
void load_tilemap(const tilemap_t, int);
void update_tile(int, int, tilemap_entry_t);
void shift_up(int, unsigned int);
void load_palette(palette_t, int);

#endif
