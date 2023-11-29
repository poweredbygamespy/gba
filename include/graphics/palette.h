#ifndef PALETTE_H
#define PALETTE_H

#include <memory.h>

typedef struct palette {
	hword colors[16];
} palette_t, palette_bank_t[16];

void load_obj_palette(palette_t palette, int slot);
void load_bg_palette(palette_t palette, int slot);

#endif
