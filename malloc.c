#include <memory.h>

extern unsigned int __eheap_start;
extern unsigned int __eheap_end;

#define MALLOC_MIN_CHUNK_SIZE (4 * sizeof(int))
typedef struct chunk {
	unsigned int prev_size;
	unsigned int size;
	struct chunk *data_or_fwd;
	struct chunk *bck;
} chunk_t;

__attribute__((section(".ewram")))
static chunk_t *free_list;

static inline unsigned int align(unsigned int n) { return (n + 3) & ~3u; }

static inline unsigned int trunc(unsigned int n) { return n & ~3u; }

static inline chunk_t *get_next(chunk_t *this) {
	chunk_t *next = (chunk_t*)((unsigned int)this + (this->size & ~3u));
	if (next >= (chunk_t*)((unsigned int)&__eheap_end - sizeof(int)))
		return 0;
	return (chunk_t*)((unsigned int)this + (this->size & ~3u));
}

static inline chunk_t *get_prev_free(chunk_t *this) {
	chunk_t *prev = 0;
	chunk_t *curr = free_list;
	while (curr != 0 && curr < this) {
		prev = curr;
		curr = curr->data_or_fwd;
	}
	return prev;
}

static inline void write_prev_size(chunk_t *empty_c) {
	// i resign
	*((unsigned int*)((unsigned int)empty_c + trunc(empty_c->size)))
		= trunc(empty_c->size);
}

void heap_init(void) {
	free_list = (chunk_t*)((unsigned int)&__eheap_start - 4);
	free_list->size = (unsigned int)&__eheap_end - (unsigned int)&__eheap_start;
	write_prev_size(free_list);
}

void *malloc(unsigned int size) {
	size = size >= MALLOC_MIN_CHUNK_SIZE ?
		align(size + sizeof(int)) : MALLOC_MIN_CHUNK_SIZE;
	
	chunk_t *fit = free_list;
	while (fit != 0 && trunc(fit->size) < size) {
		fit = fit->data_or_fwd;
	}

	if (fit == 0)
		return 0;

	chunk_t *next;
	chunk_t *prev_free;
	chunk_t *next_free;
	// not enough space for new empty chunk, fill up all the space
	if (trunc(fit->size) - size < MALLOC_MIN_CHUNK_SIZE) {
		// previous chunk is in use or it'd have been merged with this one
		fit->size = trunc(fit->size) + 1;

		next = get_next(fit);

		prev_free = fit->bck;
		next_free = fit->data_or_fwd;
	}
	// split off new empty chunk
	else {
		unsigned int old_size = trunc(fit->size);
		fit->size = size + 1;

		next = get_next(fit);
		if (next) {
			next->data_or_fwd = fit->data_or_fwd;
			next->size = old_size - size;
			write_prev_size(next);
		}

		prev_free = fit->bck;
		next_free = next;
	}

	if (prev_free == 0)
		free_list = next_free;
	else
		prev_free->data_or_fwd = next_free;
	if (next_free)
		next_free->bck = prev_free;

	if (next)
		next->size |= 1;

	return &fit->data_or_fwd;
}

void free(void *ptr) {
	chunk_t *chunk = (chunk_t*)((unsigned int)ptr - 2 * sizeof(int));
	chunk_t *prev_free;
	chunk_t *next_free;
	// coalesce with previous chunk
	if (!(chunk->size & 1)) {
		chunk_t *prev = (chunk_t*)((unsigned int)chunk - chunk->prev_size);
		prev->size += chunk->size;
		chunk = prev;
		prev_free = chunk->bck;
		next_free = chunk->data_or_fwd;
	}
	else {
		prev_free = get_prev_free(chunk);
		if (prev_free == 0) {
			next_free = free_list;
			free_list = chunk;
			chunk->bck = 0;
		}
		else {
			next_free = prev_free->data_or_fwd;
			prev_free->data_or_fwd = chunk;
			chunk->bck = prev_free;
		}
	}

	chunk_t *next = get_next(chunk);
	// coalesce with next chunk
	if (next && next_free == next) {
		chunk->size += trunc(next->size);
		chunk->data_or_fwd = next->data_or_fwd;
		if (chunk->data_or_fwd)
			chunk->data_or_fwd->bck = chunk;
		next_free = next->data_or_fwd;
		next = get_next(chunk);
	}

	chunk->data_or_fwd = next_free;
	if (next_free)
		next_free->bck = chunk;

	if (next)
		next->size &= ~1u;
	write_prev_size(chunk);
}
