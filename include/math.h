#ifndef MATH_H
#define MATH_H

typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned int uint32, uint;
typedef unsigned short uint16;
typedef unsigned char uint8;

#define INT8_MIN (-128)
#define INT16_MIN (-32767-1)
#define INT32_MIN (-2147483647-1)
#define INT8_MAX (127)
#define INT16_MAX (32767)
#define INT32_MAX (2147483647)

#define UINT8_MAX (255)
#define UINT16_MAX (65535)
#define UINT32_MAX (4294967295U)

extern int div(int, int);
extern int mod(int, int);
extern void divmod(int, int, int*, int*);
uint udiv(uint, uint);
uint umod(uint, uint);
void udivmod(uint, uint, uint*, uint*);
extern void obj_affine_set(void*, void*, int, int);
extern void bg_affine_set(void*, void*, int);
uint16 ang(int16 deg);
int16 sin(int16 deg);

#endif
