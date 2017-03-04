#include "../include/PMat.h"

extern void Mass(PMat* M, G2Xpoint pos, G2Xvector vel, double m) {
	M->position = pos;
	M->velocity = vel;

	M->force.x = 0;
	M->force.y = 0;

	M->masse = m;

	M->draw = &DrawCircle;
	M->algo = &AlgoLeapFrog;
}

extern void MassFixe(PMat* M, G2Xpoint pos) {
	M->position = pos;

	M->velocity.x = 0;
	M->velocity.y = 0;

	M->force.x = 0;
	M->force.y = 0;

	M->masse = 1;

	M->draw = &DrawCircle;
	M->algo = &AlgoFixe;
}

static void DrawCircle(PMat* M) {
	g2x_FillCircle(M->position.x, M->position.y, 0.2, G2Xca_b);
}

extern void AlgoLeapFrog(PMat* M, double h) {
	M->velocity.x += h / M->masse * M->force.x; /* Vn+1 = Vn + h/m Fn */
	M->velocity.y += h / M->masse * M->force.y;

	M->position.x +=  h * M->velocity.x; /* Xn+1 = Xn + hVn+1 */
	M->position.y +=  h * M->velocity.y; /* Yn+1 = Yn + hVn+1 */

	M->force.x = 0;
	M->force.y = 0;
}

extern void AlgoFixe(PMat* M, double h) {
	M->velocity.x = 0;
	M->velocity.y = 0;
}
