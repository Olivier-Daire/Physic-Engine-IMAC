#ifndef PMAT
#define PMAT
#include <g3x.h>
#include "math3D.h"

typedef struct PMat{
	Vector3 position;
	Vector3 velocity;
	Vector3 force;
	double masse;
	float radius;

	void (*algo)(struct PMat *, double h);
	void (*draw)(struct PMat *, G3Xcolor);

	int type;
}PMat;

extern void Mass(PMat* M, Vector3 pos, Vector3 vel, double m);
extern void MassFixe(PMat* M, Vector3 pos);

static void DrawCircle(PMat* M, G3Xcolor color);

extern void AlgoLeapFrog(PMat* M, double h);
extern void AlgoFixe(PMat* M, double h);

extern bool sphereCollision(PMat* M, PMat* Sphere);

#endif
