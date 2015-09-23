#include <gwgc.h>
#include <stdlib.h>
#include <stdio.h>

/* binary tree node */
struct ll_node {
	struct ll_node *next;
	int data;
};

struct gg_roots globals;
struct ll_node *head;
int len = 0;

#define ITER 1024
#define NALLOC 32

void
test(void)
{
	GG_PTRS(4);
	struct ll_node *first = NULL, *n = NULL, *p = NULL, *iter = NULL;
	int i;

	for (i = 0 ; i < ITER ; i++) {
		int j, nkeep;

		first = p = gg_alloc(sizeof(struct ll_node), 1);
		for (j = 0 ; j < NALLOC-1 ; j++) {
			n       = gg_alloc(sizeof(struct ll_node), 1);
			p->next = n;
			p       = n;
		}
		if (len == 0) {
			head  = first;
			nkeep = 0;
		} else {
			nkeep  = rand() % len;
			for (j = 0, iter = head ; j < nkeep ; j++, iter = iter->next) ;
			iter->next = first;
		}
		len = nkeep + NALLOC;
	}
	GG_RETURN();
}

int
main(void)
{
	head = NULL;
	gg_add(&globals, 1);
	test();

	return 0;
}
