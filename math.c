#include <math.h>

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

int16 sin(int16 deg) {
	struct __attribute__((packed, aligned(4))) {
		int16 sx; int16 sy; uint16 rot;
	} a = { 10000, 10000, ang(deg) };
	struct __attribute__((packed, aligned(4))) {
		int16 a11; int16 a12; int16 a21; int16 a22;
	} b = { 0, 0, 0, 0 };
	obj_affine_set(&a, &b, 1, 2);
	return b.a21;
}

uint16 ang(int16 deg) {
	return (uint16)(0x10000 * deg / 360);
}
