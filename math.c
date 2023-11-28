#include <math.h>

int div(int a, int b) {
	int res;
	asm("mov r0, %1\n\t"
		"mov r1, %2\n\t"
		"swi #0x6\n\t"
		"mov %0, r0"
		: "=r"(res)
		: "r"(a), "r"(b)
		: "r0", "r1");
	return res;
}

int mod(int a, int b) {
	int res;
	asm("mov r0, %1\n\t"
		"mov r1, %2\n\t"
		"swi #0x6\n\t"
		"mov %0, r1"
		: "=r"(res)
		: "r"(a), "r"(b)
		: "r0", "r1");
	return res;
}

void divmod(int a, int b, int *div, int *mod) {
	asm("mov r0, %2\n\t"
		"mov r1, %3\n\t"
		"swi #0x6\n\t"
		"mov %0, r0\n\t"
		"mov %1, r1"
		: "=r"(*div), "=r"(*mod)
		: "r"(a), "r"(b)
		: "r0", "r1");
}

uint udiv(uint a, uint b) {
	//uint dummy, res;
	//udivmod(a, b, &res, &dummy);
	return a / b;
}

uint umod(uint a, uint b) {
	//uint dummy, res;
	//udivmod(a, b, &dummy, &res);
	return a % b;
}

void udivmod(uint a, uint b, uint *div, uint *mod) {
	*div = 0;
	*mod = 0;
	for (int i = 32 - __builtin_clz(a); i >= 0; --i) {
		*mod <<= 1;
		*mod |= a>>i & 1u;
		if (*mod >= b) {
			*mod -= b;
			*div |= 1u<<i;
		}
	}
}
