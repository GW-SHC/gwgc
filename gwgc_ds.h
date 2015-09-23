/* Copyright Gabriel Parmer, 2015, GPL v2  */

#ifndef GWGC_DS_H
#define GWGC_DS_H

struct gg_roots {
	int              nroots;
	struct gg_roots *next;
	void            *roots[0];
};

#endif	/* GWGC_DS_H */
