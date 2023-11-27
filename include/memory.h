#ifndef MEMORY_H
#define MEMORY_H

#define NULL 0

void memcpy(void *dst, const void *src, unsigned int len);
void memset(void *dst, const int byte, unsigned int len);
void memclr(void *dst, unsigned int len);

void heap_init(void);

void *malloc(unsigned int size);
void free(void *ptr);

#endif
