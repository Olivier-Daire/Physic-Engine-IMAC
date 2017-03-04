#include "../include/Link.h"

void AlgoRessort(Link* L) {
	double d = g2x_Distance(L->M1->position, L->M2->position);
	double f = - L->k * (1 - (L->d0 / d));

	G2Xvector M1M2 = g2x_Vector(L->M1->position, L->M2->position);

	M1M2.x *= f;
	M1M2.y *= f;

	L->M1->force.x -= M1M2.x;
	L->M1->force.y -= M1M2.y;
	L->M2->force.x += M1M2.x;
	L->M2->force.y += M1M2.y;
}

void AlgoFrein(Link* L) {
	G2Xvector f = L->M1->velocity;

	f.x -= L->M2->velocity.x;
    f.y -= L->M2->velocity.y;

    f.x *= -L->z;
    f.y *= -L->z;

    L->M1->force.x += f.x;
    L->M1->force.y += f.y;

    L->M2->force.x -= f.x;
    L->M2->force.y -= f.y;
}

void Connect(PMat* M1, Link* L, PMat* M2) {
	L->M1 = M1;
	L->M2 = M2;
	L->draw = &DrawLine;
	L->d0 = g2x_Distance(L->M1->position, L->M2->position);
}

static void DrawLine(Link* L) {
	g2x_Line(L->M1->position.x, L->M1->position.y, L->M2->position.x, L->M2->position.y, G2Xr, 2);
}

void AlgoRessortFrein(Link* L) {
	AlgoRessort(L);
	AlgoFrein(L);
}

extern void RessortFrein(Link* L, double k, double z) {
	L->k = k;
	L->z = z;
	L->d0 = 0;

	L->algo = &AlgoRessortFrein;
}

extern void Gravite(Link* L, G2Xvector gravity) {
	L->M1->force.x += gravity.x;
	L->M1->force.y += gravity.y;

	L->M2->force.x += gravity.x;
	L->M2->force.y += gravity.y;
}
