#include <graphics/virtterm.h>
#include <memory.h>
#include <graphics/vmem.h>
#include <graphics/printf.h>
#include <math.h>

extern const tile_t unscii_8_alt[];
extern const palette_t unscii_8_alt_pal;
extern const palette_t test_pal;
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
	load_tileset(unscii_8_alt, 0x80, 0);

	load_palette(unscii_8_alt_pal, 1);
	load_palette(test_pal, 0);

	update_tile(30, 10, set_tilemap_entry_palette(0x41, 1));
	update_tile(30, 42, set_tilemap_entry_palette(0x41, 1));
	update_tile(30, 72, set_tilemap_entry_palette(0x41, 1));
	update_tile(30, 73, set_tilemap_entry_palette(0x41, 1));
	update_tile(30, 74, set_tilemap_entry_palette(0x41, 1));
	update_tile(30, 75, set_tilemap_entry_palette(0x41, 1));
	update_tile(30, 76, set_tilemap_entry_palette(0x41, 1));
	update_tile(30, 106, set_tilemap_entry_palette(0x41, 1));
	update_tile(30, 138, set_tilemap_entry_palette(0x41, 1));

	void *addr = (void*)0x0600000A;
	printf("%010xslash n\n", addr);
	int test = 10;
	printf("%i\n", INT32_MIN);
	int16 scroll = 0;
	while (1) {
		printf("1234");
		(*((int16*)(0x04000014))) = scroll++;
		(*((int16*)(0x04000016))) = -scroll;
		for (int i = 0; i < 120000; ++i) {}
	}
	
	for (;;);

	return 0;
}
