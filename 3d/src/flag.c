#include <g3x.h>
#include "PMat.h"
#include "Link.h"


#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];

#define HEIGHT 24
#define WIDTH 24

int nbm = HEIGHT * WIDTH;
int nbl = HEIGHT * WIDTH * 5;
Vector3 GRAVITY = {0, 9.8, 0};
double h;

PMat* TabM;
Link* TabL;
Link* TabLSphere;

Link* L;
Link* LSphere;
PMat* M;

PMat* Sphere;
Link* Gravit;

Link* Gravit2;

static void Init(void)
{
  double Fe = 20; // Frequence
  h = 1 / Fe; 
  double k = 0.03 * Fe * Fe; // Raideur
  double z = 0.013 * Fe; // Viscosite

  TabM = (PMat*)calloc(nbm, sizeof(PMat));
  TabL = (Link*)calloc(nbl, sizeof(Link));

  M = TabM;

  Vector3 vel;
  vel.x = 0;
  vel.y = 0;
  vel.z = 0;

  for (int i = 0; i < WIDTH ; i++)
  {
    for (int j = 0; j < HEIGHT; j++)
    {
      float mass = 0.3;
      if (i == 0 || j == 0 || j == HEIGHT - 1  || i == WIDTH - 1) {
        mass = 0.2;
      }
      Mass(M, (Vector3) { 1 * i , 0, 1 * j}, vel, mass); M++;
    }
  }

  M = TabM;
  L = TabL;

  for (int i = 0; i < nbm - 1; i++) {
    //liens verticaux
    if (((i + 1) % HEIGHT) != 0) { 
        RessortFrein(L, k, z);
        Connect(M, L, M+1);
        L++;
    }

    // liens horizontaux
    if (i < (nbm - HEIGHT)) {
      RessortFrein(L, k, z);
      Connect(M, L, M+HEIGHT);
      L++;
    }

    // diagonale
   if (i < (nbm - HEIGHT) && ((i + 1) % HEIGHT) != 0) {
      RessortFrein(L, k, z);
      Connect(M, L, M+HEIGHT+1);
      L++;
    }

    // diagonale
    if (i < (nbm - HEIGHT) && ((i) % HEIGHT) != 0) {
      RessortFrein(L, k, z);
      Connect(M, L, M+HEIGHT-1);
      L++;
    }

    M++;
  }

  Gravit = malloc(sizeof(Link));
  GravityLinkInit(Gravit);
  M = TabM;
  for (int i = 0; i < nbm - 1; i++) {
    if(i == nbm - 1 ){
      break;
    }
    M++;
  }
  Connect(TabM, Gravit, M);

  Gravit2 = malloc(sizeof(Link));
  GravityLinkInit(Gravit2);
  M = TabM;
  for (int i = 0; i < nbm; i++) {
    if(i == 23){
      break;
    }
    M++;
  }
  Connect(TabM + 24*23 , Gravit2, M);

  // Sphere
  Sphere = malloc(sizeof(PMat));
  MassFixe(Sphere, (Vector3){10, 10, 8});
  Sphere->radius = 5;
  TabLSphere = (Link*)calloc(nbm, sizeof(Link));
  LSphere = TabLSphere;
  M = TabM;
  for (int i = 0; i < nbm - 1; i++) {
     // lien avec sphere
    RessortFreinSphere(LSphere, k * 50, z);
    ConnectSphere(Sphere, LSphere, M);
    LSphere++;
    M++;
  }
}

void Anim(void)
{
  M = TabM;
  L = TabL;
  for(int i = 0; i < nbm ; i++) {
      M->algo(M, h); ++M;
  }
  for(int i = 0; i < (nbm*4)-142 ; i++) { // FIXME Whyyy ?
    L->algo(L);
    ++L;
  }
  LSphere = TabLSphere;
  for(int i = 0; i < nbm -1 ; i++) {
    LSphere->algo(LSphere);
    ++LSphere;
  }

  Gravit->algo(Gravit);
  Gravit2->algo(Gravit2);
}

/*= FONCTION DE DESSIN PRINCIPALE =*/
static void Dessin(void)
{
  bool drawLines = false;
  if (drawLines)
  {
    M = TabM;
    L = TabL;
    for(int i = 0; i < nbm ; i++) {
      M->draw(M, G3Xrb); ++M;
    }
    for(int i = 0; i < (nbm*4)-142 ; i++) { // FIXME Whyyy ?
      L->draw(L); ++L;
    }
      LSphere = TabLSphere;
    for(int i = 0; i < nbm -1 ; i++) {
      LSphere->draw(LSphere); // FIXME
      ++LSphere;
    }
  } else {
    drawFlag();
  }
  Sphere->draw(Sphere, G3Xbb);
  //Gravit->draw(Gravit);
  //Gravit2->draw(Gravit2);
}

void drawFlag() {
    M = TabM;
    glBegin(GL_TRIANGLES); //Begin triangle coordinates
    for(int i = 0; i < WIDTH; ++i) {
        for(int j = 0; j < HEIGHT; ++j) {

            if (i < WIDTH-1 && j < HEIGHT - 1) {
                  drawVertexTriangle(M, M+1 + HEIGHT, M + HEIGHT);
                    drawVertexTriangle(M, M+1, M + 1 + HEIGHT);
            }
            M++;
        }
    }
    glEnd();
}

void drawVertexTriangle(PMat* M1, PMat* M2, PMat* M3) {
    glVertex3f(M1->position.x, M1->position.y, M1->position.z);
    glVertex3f(M2->position.x, M2->position.y, M2->position.z);
    glVertex3f(M3->position.x, M3->position.y, M3->position.z);
}

/*=    ACTION A EXECUTER EN SORTIE   =*/
/*= libération de mémoire, nettoyage =*/
/*= -> utilise la pile de <atexit()> =*/
static void Exit(void)
{
  /* rien à faire ici puisqu'il n'y a pas d'allocation dynamique */
	fprintf(stdout,"\nbye !\n");
}

/*= action : affiche les parametre camera dans le terminal =*/
static void camera_info(void)
{
	G3Xcamera* cam=g3x_GetCamera();
	fprintf(stderr,"position (x:%lf,y:%lf,z:%lf)\n",(*cam->pos)[0],(*cam->pos)[1],(*cam->pos)[2]);
	fprintf(stderr,"cible    (x:%lf,y:%lf,z:%lf)\n",(*cam->tar)[0],(*cam->tar)[1],(*cam->tar)[2]);
	fprintf(stderr,"coord. spheriques (d:%lf,theta:%lf,phi:%lf)\n",cam->dist,cam->theta,cam->phi);
}

int main(int argc, char** argv)
{ 
  
  /* initialisation de la fenêtre graphique et paramétrage Gl */
  g3x_InitWindow(*argv,1280,720);

	/* paramètres caméra */
  /* param. géométrique de la caméra. cf. gluLookAt(...) */
  g3x_SetPerspective(25.,100.,1.);
  /* position, orientation de la caméra */
  //g3x_SetCameraSpheric(0,0,30,(G3Xpoint){20.,0.,0.});
  //,theta:2.578274,phi:1.685796
  g3x_SetCameraSpheric(0.85*PI,0.535*PI,100.,(G3Xpoint){10.,10.,10.});
  //g3x_SetCameraCartesian((G3Xpoint){-1,1, 100}, (G3Xpoint){0.,0.,1.});

  /* fixe les param. colorimétriques du spot lumineux */
	/* lumiere blanche (c'est les valeurs par defaut)   */	
  g3x_SetLightAmbient (1.,1.,1.);
  g3x_SetLightDiffuse (1.,1.,1.);
  g3x_SetLightSpecular(1.,1.,1.);

  /* fixe la position et la direction du spot lumineux */
	/* (c'est les valeurs par defaut)                    */	
  g3x_SetLightPosition (10.,10.,10.);
  g3x_SetLightDirection( 0., 0., 0.);  

  /* action attachées à des touches */
  g3x_SetKeyAction('c',camera_info,"pos./dir. de la camera => terminal");

	/* initialisation d'une carte de couleurs */
	g3x_FillColorMap(colmap,MAXCOL);

  /* définition des fonctions */
  g3x_SetInitFunction(Init);
  g3x_SetExitFunction(Exit  );     /* la fonction de sortie */
  g3x_SetDrawFunction(Dessin);     /* la fonction de Dessin */
	g3x_SetAnimFunction(Anim);
	
  
	/* JUSTE POUR ILLUSTRATION DU TRACEUR D'ALLOC EN COMPIL DEGUG */
	void* ptr=malloc(1);
	
	/* boucle d'exécution principale */
  return g3x_MainStart();
  /* rien après ça */
}
