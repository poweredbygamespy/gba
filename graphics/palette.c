#include <graphics/palette.h>

#define MEM_PAL (0x05000000)

#define palette_memory ((palette_bank_t*)MEM_PAL)

void load_obj_palette(palette_t palette, int slot) {
	asm("mov r0, %0\n\t"
		"mov r1, %1\n\t"
		"mov r2, %2\n\t"
		"swi #0x0c"
		:
		: "r" (&palette),
		  "r" (&palette_memory[1][slot]),
		  "r" (sizeof(palette_t) / 4)
		: "r0", "r1", "r2");
}

void load_bg_palette(palette_t palette, int slot) {
	asm("mov r0, %0\n\t"
		"mov r1, %1\n\t"
		"mov r2, %2\n\t"
		"swi #0x0c"
		:
		: "r" (&palette),
		  "r" (&palette_memory[0][slot]),
		  "r" (sizeof(palette_t) / 4)
		: "r0", "r1", "r2");
}
