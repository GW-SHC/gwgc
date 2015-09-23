/* Copyright Gabriel Parmer, 2015, GPL v2  */

#include <gwgc_ds.h>
#include <malloc.h>
#include <assert.h>
#include <stdio.h>

typedef enum {
	GG_UNKNOWN,
	GG_REACHABLE,
} gg_state_t;

/* 
 * We assume that the memory we allocated has all of the pointers to
 * other data within its first "nptrs" words.  This is the only way we
 * can traverse the memory graph.
 */
struct gg_alloc_head {
	gg_state_t            state;
	unsigned int          nptrs;
	struct gg_alloc_head *next; 
	void *allocation[0];
};

/* list of all the GC roots */
struct gg_roots terminal_roots = {.nroots = 0, .next = NULL};
struct gg_roots *roots = &terminal_roots;

/* linked list of all memory allocations */
static struct gg_alloc_head *gg_allocations = NULL; 

struct gg_alloc_head *
gg_alloc2head(void *a)
{ return (struct gg_alloc_head *)((char *)a - sizeof(struct gg_alloc_head)); }

static void
walk(void *a)
{
	struct gg_alloc_head *h;
	unsigned int i;

	if (!a) return;
	h        = gg_alloc2head(a);
	if (h->state == GG_REACHABLE) return;
	h->state = GG_REACHABLE;

	/* iterate through the pointers in this allocation */
	for (i = 0 ; i < h->nptrs ; i++) {
		walk(h->allocation[i]);
	}
}

static void
mark(void)
{
	struct gg_roots *iter;

	/* iterate through the roots */
	for (iter = roots ; iter ; iter = iter->next) {
		int i;

		for (i = 0 ; i < iter->nroots ; i++) {
			walk(iter->roots[i]);
		}
	}
}

static void
sweep(void)
{
	struct gg_alloc_head *iter, *prev;

	for (iter = gg_allocations, prev = NULL ; 
	     iter ; 
	     prev = iter, iter = iter->next) {
		if (iter->state != GG_REACHABLE) {
			if (prev == NULL) gg_allocations = iter->next;
			else              prev->next     = iter->next;

			printf("-");
			free(iter);
		}
		iter->state = GG_UNKNOWN;
	}
}

static void
collect(void)
{
	mark();
	sweep();
}

#define GG_COLLECTION_GRANULARITY 128

/**
 * Allocate a chunk of memory of a given size (sz) that has a number
 * of pointers within the first N (nptrs) words of its memory.
 */
void *
gg_alloc(unsigned int sz, unsigned int nptrs) 
{
	static unsigned int nallocs = 0;
	struct gg_alloc_head *h;
	void *ret;

	/* 
	 * Shall we collect?  This condition could also be based on
	 * amount of allocated memory.
	 */
	if (nallocs >= GG_COLLECTION_GRANULARITY) {
		nallocs = 0;
		collect();
	}

	h        = calloc(1, sz + sizeof(struct gg_alloc_head));
	assert(h); 		/* ...whaaaaaa */
	h->nptrs = nptrs;
	h->state = GG_UNKNOWN;
	h->next  = gg_allocations;
	gg_allocations = h;
	ret      = (void *)h->allocation;

	printf("+");
	nallocs++;

	return ret;
}

