#include <graphics/tilemap.h>
#include <memory.h>

#define MEM_IO (0x04000000)
#define MEM_PAL (0x05000000)
#define MEM_VRAM (0x06000000)

#define palette_memory ((palette_bank_t*)MEM_PAL)
#define tileset_memory ((tileset_t*)MEM_VRAM)
#define tilemap_memory ((tilemap_t*)MEM_VRAM)

#define REG_DISPCNT (*(volatile unsigned int*)MEM_IO)
#define DCNT_MODE0 (0)
#define DCNT_MODE1 (1)
#define DCNT_MODE2 (2)
#define DCNT_MODE3 (3)
#define DCNT_MODE4 (4)
#define DCNT_MODE5 (5)
#define DCNT_OBJ_2D (0<<6)
#define DCNT_OBJ_1D (1<<6)
#define DCNT_BG0 (0x0100)
#define DCNT_BG1 (0x0200)
#define DCNT_BG2 (0x0400)
#define DCNT_BG3 (0x0800)
#define DCNT_OBJ (0x1000)

#define REG_VCOUNT (*(volatile unsigned int*)(MEM_IO + 0x6))

#define REG_BG0CNT (*(volatile unsigned int*)(MEM_IO + 0x8))
#define BG_PRIO0 (0)
#define BG_PRIO1 (1)
#define BG_PRIO2 (2)
#define BG_PRIO3 (3)
#define BG_CBB0 (0<<2)
#define BG_CBB1 (1<<2)
#define BG_CBB2 (2<<2)
#define BG_CBB3 (3<<2)
#define BG_MOSAIC (1<<6)
#define BG_CM_4BPP (0<<7)
#define BG_CM_8BPP (1<<7)
#define BG_SBB0 (0<<8)
#define BG_SBB1 (1<<8)
#define BG_SBB2 (2<<8)
#define BG_SBB3 (3<<8)
#define BG_SBB4 (4<<8)
#define BG_SBB5 (5<<8)
#define BG_SBB6 (6<<8)
#define BG_SBB7 (7<<8)
#define BG_SBB8 (8<<8)
#define BG_SBB9 (9<<8)
#define BG_SBB10 (10<<8)
#define BG_SBB11 (11<<8)
#define BG_SBB12 (12<<8)
#define BG_SBB13 (13<<8)
#define BG_SBB14 (14<<8)
#define BG_SBB15 (15<<8)
#define BG_SBB16 (16<<8)
#define BG_SBB17 (17<<8)
#define BG_SBB18 (18<<8)
#define BG_SBB19 (19<<8)
#define BG_SBB20 (20<<8)
#define BG_SBB21 (21<<8)
#define BG_SBB22 (22<<8)
#define BG_SBB23 (23<<8)
#define BG_SBB24 (24<<8)
#define BG_SBB25 (25<<8)
#define BG_SBB26 (26<<8)
#define BG_SBB27 (27<<8)
#define BG_SBB28 (28<<8)
#define BG_SBB29 (29<<8)
#define BG_SBB30 (30<<8)
#define BG_SBB31 (31<<8)
#define BG_REG_32x32 (0<<14)
#define BG_REG_64x32 (1<<14)
#define BG_REG_32x64 (2<<14)
#define BG_REG_64x64 (3<<14)

void setup(void) {
	REG_DISPCNT = DCNT_BG0 | DCNT_MODE0;
	REG_BG0CNT = BG_REG_32x32 | BG_CM_4BPP | BG_SBB31 | BG_CBB0;
}

void load_tileset(const tile_t tileset[], unsigned int size, int cbb) {
	asm("mov r0, %0\n\t"
		"mov r1, %1\n\t"
		"mov r2, %2\n\t"
		"swi #0x0c"
		:
		: "r" (tileset), "r" (&tileset_memory[cbb]), "r" (size * sizeof(tile_t) / 4)
		: "r0", "r1", "r2");
}

void load_tilemap(const tilemap_t tilemap, int sbb) {
	asm("mov r0, %0\n\t"
		"mov r1, %1\n\t"
		"mov r2, %2\n\t"
		"swi #0x0c"
		:
		: "r" (&tilemap), "r" (&tilemap_memory[sbb]), "r" (sizeof(tilemap_t) / 4)
		: "r0", "r1", "r2");
}

void update_tile(int sbb, int offset, tilemap_entry_t te) {
	tilemap_memory[sbb][offset] = te;
}

void shift_up(int sbb, unsigned int lines) {
	memcpy(&tilemap_memory[sbb], &tilemap_memory[sbb][32 * lines], (32 - lines) * 32 * sizeof(tilemap_entry_t));
	memclr(&tilemap_memory[sbb][32 * (32 - lines)], lines * 32 * (int)sizeof(tilemap_entry_t));
}

void load_palette(palette_t palette, int paletteid) {
	asm("mov r0, %0\n\t"
		"mov r1, %1\n\t"
		"mov r2, %2\n\t"
		"swi #0x0c"
		:
		: "r" (&palette), "r" (&palette_memory[0][paletteid]), "r" (sizeof(palette_t) / 4)
		: "r0", "r1", "r2");
}

