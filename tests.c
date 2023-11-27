#include <tests.h>
#include <malloc.h>

void test_malloc() {
	unsigned int *b[0x100];
	for (int i = 0; i < 0x100; ++i) {
		b[i] = malloc(3 * sizeof(int));
		*b[i] = 0x99999999;
		*(b[i] + 1) = 0xcccccccc;
		*(b[i] + 2) = 0xffffffff;
	}

	for (int i = 0; i < 0x40; ++i) {
		int j = 4 * i;
		free(b[j]);
		free(b[j + 1]);
	}

	for (int i = 0; i < 0x40; ++i) {
		int j = 4 * i + 2;
		free(b[j]);
		free(b[j + 1]);
	}

	//unsigned int *c = malloc((65535 - 1) * sizeof(int));
	unsigned int *d = malloc(1);
}
