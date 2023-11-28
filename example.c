#include <graphics/virtterm.h>
#include <memory.h>
#include <graphics/tilemap.h>
#include <graphics/printf.h>
#include <math.h>

extern const tile_t unscii_8_alt[];
extern const palette_t unscii_8_alt_pal;
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

	load_palette(unscii_8_alt_pal, 0);

	void *addr = (void*)0x0600000A;
	printf("%010xslash n\n", addr);
	int test = 10;
	printf("%i\n", INT32_MIN);
	while (1) {
		printf("1234");
		for (int i = 0; i < 120000; ++i) {}
	}
	
	for (;;);

	return 0;
}
