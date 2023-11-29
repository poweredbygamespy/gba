#include <graphics/tile.h>

#define MEM_VRAM (0x06000000)

#define tileset_memory ((tileset_t*)MEM_VRAM)
#define tileset_8bpp_memory ((tileset_8bpp_t*)MEM_VRAM)

void load_tileset(const tile_t tileset[], unsigned int size, int cbb) {
	asm("mov r0, %0\n\t"
		"mov r1, %1\n\t"
		"mov r2, %2\n\t"
		"swi #0x0c"
		:
		: "r" (tileset),
		  "r" (&tileset_memory[cbb]),
		  "r" (size * sizeof(tile_t) / 4)
		: "r0", "r1", "r2");
}

void load_tileset_8bpp(
		const tile_8bpp_t tileset[], unsigned int size, int cbb) {
	asm("mov r0, %0\n\t"
		"mov r1, %1\n\t"
		"mov r2, %2\n\t"
		"swi #0x0c"
		:
		: "r" (tileset),
		  "r" (&tileset_8bpp_memory[cbb]),
		  "r" (size * sizeof(tile_8bpp_t) / 4)
		: "r0", "r1", "r2");
}
