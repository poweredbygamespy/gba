#include <graphics/virtterm.h>
#include <memory.h>
#include <graphics/tiledbg.h>
#include <graphics/printf.h>
#include <graphics/palette.h>
#include <graphics/tile.h>
#include <math.h>

extern const tile_t unscii_8_alt[];
extern const tile_t walltiles[];
extern const palette_t wallpalette;
extern const tilemap_entry_t wallmap[];
extern const palette_t unscii_8_alt_pal;
extern const palette_t test_pal;
extern const void test_affine_bg;
extern const char ones[];

extern void memcpyAligned(void *dst, const void *src, unsigned int len);

int main(void) {
	setup();

	/*void *addr = (void*)0x06000000;
	for (int j = 0; j < 5000; ++j) {
		//load_tileset(unscii_8_alt, 0x80, 0);
		memcpy(addr, &unscii_8_alt, 0x80 * sizeof(tile_t));
		//memcpyAligned(addr, &unscii_8_alt, 0x80 * sizeof(tile_t));
	}*/
	//load_tileset(unscii_8_alt, 0x80, 0);
	load_tileset(walltiles, 16, 2);
	load_tilemap(wallmap, sizeof(tilemap_t) / sizeof(tilemap_entry_t), 31);
	//load_tileset(&test_affine_bg, 0x4, 1);
	//load_tileset_8bpp(&test_affine_bg, 0x3, 1);

	load_bg_palette(unscii_8_alt_pal, 0);
	load_bg_palette(wallpalette, 3);
	//load_bg_palette(unscii_8_alt_pal, 1);
	//load_bg_palette(test_pal, 0);

	/*
	update_tile_affine(30, 2, 2);
	update_tile_affine(30, 18, 2);
	update_tile_affine(30, 32, 2);
	update_tile_affine(30, 33, 2);
	update_tile_affine(30, 34, 2);
	update_tile_affine(30, 35, 2);
	update_tile_affine(30, 36, 2);
	update_tile_affine(30, 50, 2);
	update_tile_affine(30, 66, 2);
	*/

	/*
	void *addr = (void*)0x0600000A;
	int frame_counter = 0;
	printf("%010p slash n\n", addr);
	printf("%i\n", INT32_MIN);
	int16 angle = 0;
	struct __attribute__((packed, aligned(4))) {
		int32 cx; int32 cy;
		int16 dx; int16 dy;
		int16 sx; int16 sy; uint16 rot;
	} a = { 20*256, 20*256, 20*2, 20*2, (1<<8)/2, (1<<8)/2, ang(angle) };
	
	while (1) {
		while(*((hword*)0x04000006) < 160);
		angle -= 1;
		a.rot = ang(angle);
		a.dx += 1;
		a.dx = a.dx > 500 ? -50 : a.dx;
		bg_affine_set(&a, (void*)0x04000020, 1);
		frame_counter++;
		if (frame_counter % 10 == 0)
			printf("1234");
		while(*((hword*)0x04000006) >= 160);
	}
	*/
	
	for (;;);

	return 0;
}
