#ifndef PMAT
#define PMAT
#include <g2x.h>

typedef struct PMat{
	G2Xpoint position;
	G2Xvector velocity;
	G2Xvector force;
	double masse;
	G2Xcolor color;

	void (*algo)(struct PMat *, double h);
	void (*draw)(struct PMat *);
	int type;
}PMat;

extern void Mass(PMat* M, G2Xpoint pos, G2Xvector vel, double m);
extern void MassFixe(PMat* M, G2Xpoint pos);

static void DrawCircle(PMat* M);

extern void AlgoLeapFrog(PMat* M, double h);
extern void AlgoFixe(PMat* M, double h);

#endif
