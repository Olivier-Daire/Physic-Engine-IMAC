#ifndef LINK
#define LINK
#include "PMat.h"
#include <g2x.h>

typedef struct _link {
	PMat *M1, *M2;
	double k,z,l,s;
	G2Xvector f;
	double d0;

	void (*algo)(struct _link *);
	void (*draw)(struct _link *);
} Link;

void Connect(PMat* M1, Link* L, PMat* M2);
void AlgoRessort(Link* L);
void AlgoFrein(Link* L);

extern void Gravite(Link* L, G2Xvector gravity);
extern void RessortFrein(Link* L, double k, double z);

static void DrawLine(Link* L);
#endif
