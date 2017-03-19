#ifndef LINK
#define LINK
#include "PMat.h"
#include <g3x.h>
#include "math3D.h"

typedef struct _link {
	PMat *M1, *M2;
	double k,z;
	double d0;
	G3Xvector f;

	void (*algo)(struct _link *);
	void (*draw)(struct _link *);
} Link;

void Connect(PMat* M1, Link* L, PMat* M2);
void ConnectSphere(PMat* Sphere, Link* L, PMat* M2);
void AlgoRessort(Link* L);
void AlgoRessortFreinSphere(Link* L);
void AlgoFrein(Link* L);

extern void Gravite(Link* L, Vector3 gravity);
extern void RessortFrein(Link* L, double k, double z);
extern void RessortFreinSphere(Link* L, double k, double z);

static void DrawLine(Link* L);
static void NoDraw(Link* L);

static void Gravity(Link* L);
extern void GravityLinkInit(Link* L);
#endif
