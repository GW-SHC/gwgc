/* Copyright Gabriel Parmer, 2015, GPL v2  */

#ifndef GWGC_H
#define GWGC_H

#include <gwgc_ds.h>

extern struct gg_roots *roots;

static inline void
gg_add(struct gg_roots *rs, int nroots)
{
	rs->nroots = nroots;
	rs->next   = roots;
	roots      = rs;
}

static inline void
gg_ret(struct gg_roots *rs)
{ roots = rs->next; }

void *gg_alloc(unsigned int sz, unsigned int nptrs);

/* How many pointers follow the use of this macro? */
#define GG_PTRS(nptrs)	      \
	struct gg_roots ggrs; \
	gg_add(&ggrs, nptrs)

/* "return" replacement */
#define GG_RETURN(retval) \
	gg_ret(&ggrs);	  \
	return retval

#endif	/* GWGC_H */
