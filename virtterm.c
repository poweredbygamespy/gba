#include <virtterm.h>
#include <tilemap.h>

static int tilemap = 31;

static int screen_width = 30;
static int screen_height = 20;
static int tilemap_width = 32;
//static int tilemap_height = 32;
static int write_offset = 0;

int putchar(int c) {
	switch (c) {
		case '\r':
			break;
		case '\t':
			write_offset = (write_offset + 2) & ~1;
			break;
		case '\n':
			write_offset = (write_offset + tilemap_width - 1)
				& ~(tilemap_width - 1);
			break;
		default:
			update_tile(31, write_offset, (unsigned char)c);
			write_offset++;
	}

	if (write_offset % tilemap_width >= screen_width) {
		write_offset += tilemap_width - screen_width;
	}
	if (write_offset >= screen_height * tilemap_width) {
		shift_up(tilemap, 1);
		write_offset = tilemap_width * (screen_height - 1);
	}
	return 1;
}
