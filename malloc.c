#include <malloc.h>

extern unsigned int __eheap_start;
extern unsigned int __eheap_size;
extern unsigned int __eheap_end;

typedef struct chunk {
	unsigned int prev_size;
	unsigned int size;
	struct chunk *data;
} chunk_t;

__attribute__((section(".ewram")))
chunk_t *free_list = (chunk_t*)(&__eheap_start - 1);

void heap_init() {
	__eheap_start = (unsigned long int)&__eheap_size;
}

static inline unsigned int align(unsigned int n) { return (n + 3u) & ~3u; }
static inline chunk_t *get_next(chunk_t *this) {
	return (chunk_t*)((unsigned long int)this + (this->size & ~3u));
}
static inline chunk_t *get_prev_free(chunk_t *this) {
	chunk_t *prev = 0;
	chunk_t *curr = free_list;
	while (curr != 0 && curr < this) {
		prev = curr;
		curr = curr->data;
	}
	return prev;
}

void *malloc(unsigned int size) {
	size = size >= 2 * sizeof(int) ?
		align(size + sizeof(int)) : 2 * sizeof(int);
	
	chunk_t *smallest_fit = 0;
	chunk_t *curr = free_list;
	chunk_t *prev_free = 0;
	while (curr != 0) {
		if ((curr->size & ~3u) >= size) {
			if (smallest_fit == 0) {
				smallest_fit = curr;
			}
			else if ((smallest_fit->size & ~3u) >= (curr->size & ~3u)) {
				smallest_fit = curr;
				break;
			}
		}
		prev_free = curr;
		curr = curr->data;
	}

	if (smallest_fit == 0) {
		return 0;
	}
	if (smallest_fit == free_list) {
		prev_free = 0;
	}

	chunk_t *next;
	if ((smallest_fit->size & ~3u) - size < 2 * sizeof(int)) { // no space for
															   // empty chunk
															   // after this one
		size = smallest_fit->size & ~3u;
		smallest_fit->size = size + 1; // previous chunk is in use
									   // or it'd have been coalesced

		next = get_next(smallest_fit);
		if (prev_free != 0)
			prev_free->data = smallest_fit->data;
		if (smallest_fit == free_list)
			free_list = smallest_fit->data;
	}
	else { // split off new empty chunk
		unsigned int old_size = smallest_fit->size & ~3u;
		smallest_fit->size = size + 1;
		next = get_next(smallest_fit);
		if (prev_free != 0)
			prev_free->data = next;
		next->data = smallest_fit->data;
		next->size = old_size - size;
		if (smallest_fit == free_list)
			free_list = next;
	}
	next->size |= 1;

	return &smallest_fit->data;
}

void free(void *ptr) {
	chunk_t *chunk = (chunk_t*)((unsigned long int)ptr - 2 * sizeof(int));
	int coalesced = 0;
	if (!(chunk->size & 1)) { // coalesce with previous chunk
		coalesced = 1;
		chunk_t *prev = (chunk_t*)((unsigned long int)chunk - chunk->prev_size);
		prev->size += chunk->size & ~3u;
		chunk = prev;
	}

	chunk_t *next = get_next(chunk);
	chunk_t *prev_free = get_prev_free(chunk);
	if (free_list == next || prev_free->data == next || (coalesced && chunk->data == next)) { // coalesce with next chunk
		chunk->size += next->size & ~3u;
		chunk->data = next->data;
		next = get_next(chunk);
	}

	if (prev_free != 0) {
		chunk->data = prev_free->data;
		prev_free->data = chunk;
	}
	else {
		if (free_list > chunk) {
			chunk->data = free_list;
		}
		else if (free_list == chunk) {
			chunk->data = free_list->data;
		}
		free_list = chunk;
	}

	if (next != (chunk_t*)((unsigned long int)&__eheap_end - sizeof(int)))
		next->size &= ~1u;
	next->prev_size = chunk->size & ~3u;
}
