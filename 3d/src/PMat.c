#include "../include/PMat.h"

extern void Mass(PMat* M, Vector3 pos, Vector3 vel, double m) {
	M->position = pos;
	M->velocity = vel;

	M->force.x = 0;
	M->force.y = 0;
	M->force.z = 0;

	M->masse = m;
	M->radius = 0.1;

	M->draw = &DrawCircle;
	M->algo = &AlgoLeapFrog;
}

extern void MassFixe(PMat* M, Vector3 pos) {
	M->position = pos;

	M->velocity.x = 0;
	M->velocity.y = 0;
	M->velocity.z = 0;

	M->force.x = 0;
	M->force.y = 0;
	M->force.z = 0;

	M->masse = 1;
	M->radius = 0.1;

	M->draw = &DrawCircle;
	M->algo = &AlgoFixe;
}

static void DrawCircle(PMat* M, G3Xcolor color) {
	glPushMatrix();
		glTranslatef(M->position.x, M->position.y, M->position.z);
		g3x_Material(color,1,1,1,1,1.);
		glutSolidSphere(M->radius,40,40); // First radius
	glPopMatrix();
}

extern void AlgoLeapFrog(PMat* M, double h) {
	M->velocity.x += h / M->masse * M->force.x; /* Vn+1 = Vn + h/m Fn */
	M->velocity.y += h / M->masse * M->force.y;
	M->velocity.z += h / M->masse * M->force.z;

	M->position.x +=  h * M->velocity.x; /* Xn+1 = Xn + hVn+1 */
	M->position.y +=  h * M->velocity.y; /* Yn+1 = Yn + hVn+1 */
	M->position.z +=  h * M->velocity.z;

	M->force.x = 0;
	M->force.y = 0;
	M->force.z = 0;
}

extern void AlgoFixe(PMat* M, double h) {
	M->velocity.x = 0;
	M->velocity.y = 0;
	M->velocity.z = 0;
}
