#include <malloc.h>

__attribute__((section(".ewram")))
unsigned int a = 0x0403;

int main(void) {
	*(unsigned int*)0x04000000 = a;

	((unsigned short*)0x06000000)[120+80*240] = 0x001F;
	((unsigned short*)0x06000000)[136+80*240] = 0x03E0;
	((unsigned short*)0x06000000)[120+96*240] = 0x7C00;

	unsigned int *b[0x100];
	for (int i = 0; i < 0x100; ++i) {
		b[i] = malloc(3 * sizeof(int));
		*b[i] = 0x12345678;
		*(b[i] + 1) = 0x9abcdef0;
		*(b[i] + 2) = 0xffffffff;
	}

	for (int i = 0; i < 0x40; ++i) {
		int j = 4 * i;
		free(b[j]);
		free(b[j + 1]);
	}

	for(;;);
	return 0;
}
