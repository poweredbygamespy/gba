#ifndef TILEMAP_H
#define TILEMAP_H

#include <memory.h>
//#include <graphics/tile.h>

typedef hword tilemap_entry_t, tilemap_t[1024];
typedef byte affine_tilemap_entry_t, affine_tilemap_t[2048];

void setup(void);
void load_tilemap(const tilemap_entry_t tilemap[], unsigned int size, int sbb);
void update_tile(int, int, tilemap_entry_t);
void update_tile_affine(int sbb, int offset, affine_tilemap_entry_t te);
tilemap_entry_t flip_tilemap_entry_h(tilemap_entry_t);
tilemap_entry_t flip_tilemap_entry_v(tilemap_entry_t);
tilemap_entry_t set_tilemap_entry_palette(tilemap_entry_t, byte);
void shift_up(int, unsigned int);

#endif
