#include "../include/Link.h"

void AlgoRessort(Link* L) {
	double d = Distance3(L->M1->position, L->M2->position);
	double f = - L->k * (1 - (L->d0 / d));


	Vector3 M1M2 = Vec3SubVec3(L->M2->position, L->M1->position);

	ProdVec3(&M1M2, f);

	L->M1->force = Vec3SubVec3(L->M1->force, M1M2);
	L->M2->force = Vec3AddVec3(L->M2->force, M1M2);
}

void AlgoFrein(Link* L) {
	Vector3 f = Vec3SubVec3(L->M1->velocity, L->M2->velocity);
	ProdVec3(&f, -L->z);

	L->M1->force = Vec3AddVec3(L->M1->force, f);
	L->M2->force = Vec3SubVec3(L->M2->force, f);
}

void Connect(PMat* M1, Link* L, PMat* M2) {
	L->M1 = M1;
	L->M2 = M2;
	L->draw = &DrawLine;
	L->d0 = Distance3(L->M1->position, L->M2->position);
}

void AlgoRessortFrein(Link* L) {
	AlgoRessort(L);
	AlgoFrein(L);
}

void ConnectSphere(PMat* Sphere, Link* L, PMat* M2){
	L->M1 = M2;
	L->M2 = Sphere;
	L->draw = &NoDraw;
	L->d0 = Sphere->radius;
}

static void DrawLine(Link* L) {
	glLineWidth(2.5);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex3f(L->M1->position.x, L->M1->position.y, L->M1->position.z);
		glVertex3f(L->M2->position.x,  L->M2->position.y,  L->M2->position.z);
	glEnd();
}

static void NoDraw(Link* L) {}

void AlgoRessortFreinSphere(Link* L) {
	double d = Distance3(L->M1->position, L->M2->position);

	if (d <= L->d0)
	{
		AlgoRessort(L);
		AlgoFrein(L);
	}
}

extern void RessortFrein(Link* L, double k, double z) {
	L->k = k;
	L->z = z;
	L->d0 = 0;

	L->algo = &AlgoRessortFrein;
}

extern void RessortFreinSphere(Link* L, double k, double z) {
	L->k = k;
	L->z = z;

	L->algo = &AlgoRessortFreinSphere;
}

extern void GravityLinkInit(Link* L) {
    L->k = 0;
    L->z = 0;
    L->d0 = 0;

    L->algo = &Gravity;
    L->draw = &DrawLine;
}

static void Gravity(Link* L) {
    float factor = 50;
    Vector3 gravity = {0.0f, 9.81f * factor, 0.0f};

	// Somethin is wrong with my x and z axes but i don't what
	L->M1->force.x += -gravity.x;
	L->M1->force.y += gravity.y;
	L->M1->force.z += -gravity.z;

	L->M2->force.x += -gravity.x;
	L->M2->force.y += gravity.y;
	L->M2->force.z += -gravity.z;

    L->M1->force = Vec3AddVec3(L->M1->force, gravity);
    L->M2->force = Vec3AddVec3(L->M2->force, gravity);
}
