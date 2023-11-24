#ifndef MALLOC_H
#define MALLOC_H

extern void memcpy(unsigned int *src, unsigned int* dst, unsigned int len);

void heap_init();

void *malloc(unsigned int size);
void free(void *ptr);

#endif
