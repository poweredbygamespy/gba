#include <malloc.h>

const unsigned int a = 0x0403;

int main(void) {
	heap_init();
	*(unsigned int*)0x04000000 = a;

	((unsigned short*)0x06000000)[120+80*240] = 0x001F;
	((unsigned short*)0x06000000)[136+80*240] = 0x03E0;
	((unsigned short*)0x06000000)[120+96*240] = 0x7C00;

	int *b = malloc(10);
	int *c = malloc(256);
	int *d = malloc(sizeof(int));
	free(c);
	free(d);
	int *e = malloc(16);

	for(;;);
	return 0;
}
