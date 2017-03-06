/*=================================================================*/
/*= E.Incerti - incerti@upem.fr                                   =*/
/*= Université Paris-Est-Marne-la-Vallée                          =*/
/*= Exemple de fonctionalités de lib. graphique <g3x>             =*/
/*=================================================================*/

#include <g3x.h>
#include "PMat.h"
#include "Link.h"

/* des couleurs prédéfinies */  
static G3Xcolor rouge  ={1.0,0.0,0.0};
static G3Xcolor jaune  ={1.0,1.0,0.0};
static G3Xcolor vert   ={0.0,1.0,0.0};
static G3Xcolor cyan   ={0.0,1.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0};
static G3Xcolor magenta={1.0,0.0,1.0};
/* paramètres géométriques */
static double angle= 0.00;
static double rayon= 0.66;
/* paramètres de lumière/matière */
static double alpha= 0.5;
static double ambi = 0.2;
static double diff = 0.3;
static double spec = 0.4;
static double shin = 0.5;

#define MAXCOL 25
static G3Xcolor colmap[MAXCOL];
double h = 0.1;
int nbm = 11;
int nbl = 2;
Vector3 GRAVITY = {0, -9.1, 0};

PMat* TabM;
Link* TabL;

Link* L;
PMat* M;


static void Init(void)
{
  double Fe = 1 / h;
  double k = 0.7 * Fe*Fe;
  double z = 0.1 * Fe;

  TabM = (PMat*)calloc(nbm, sizeof(PMat));
  nbl = (nbm-1);
  TabL = (Link*)calloc(nbl, sizeof(Link));
  M = TabM;
  Vector3 vel;
  vel.x = 1;
  vel.y = 1;
  vel.z = 1;

  MassFixe(M, (Vector3){-5., -0., 0}); M++;
  Mass(M, (Vector3) { -4., 0., 0 }, vel, 1); M++;
  Mass(M, (Vector3) { -3., 0., 0 }, vel, 1); M++;
  Mass(M, (Vector3) { -2., 0., 0 }, vel, 1); M++;
  Mass(M, (Vector3) { -1., 0., 0 }, vel, 1); M++;
  Mass(M, (Vector3) { 0., 0., 0 }, vel, 1); M++;
  Mass(M, (Vector3) { +1., 0., 0 }, vel, 1); M++;
  Mass(M, (Vector3) { +2., 0., 0 }, vel, 1); M++;
  Mass(M, (Vector3) { +3., 0., 0 }, vel, 1); M++;
  Mass(M, (Vector3) { +4., 0., 0 }, vel, 1); M++;
  MassFixe(M, (Vector3){+5., 0., 0}); M++;

  M = TabM;
  L = TabL;

  while(L < TabL+nbl) {
    RessortFrein(L, k, z);
    Connect(M, L, M+1);
    Gravite(L, GRAVITY);
    M++;
    L++;
  }
}

void Anim(void)
{
  M = TabM;
  L = TabL;
  while (M < TabM + nbm) { M->algo(M, h); ++M; }
  while (L < TabL + nbl) { L->algo(L); ++L; }
}

/*= FONCTION DE DESSIN PRINCIPALE =*/
static void Dessin(void)
{
	M = TabM;
  while (M < TabM + nbm) { M->draw(M); ++M; }
  L = TabL;
  while (L < TabL + nbl) { L->draw(L); ++L; }
}

/*=    ACTION A EXECUTER EN SORTIE   =*/
/*= libération de mémoire, nettoyage =*/
/*= -> utilise la pile de <atexit()> =*/
static void Exit(void)
{
  /* rien à faire ici puisqu'il n'y a pas d'allocation dynamique */
	fprintf(stdout,"\nbye !\n");
}

/*= action : variation de couleur =*/
static void action1(void)
{
  rouge[0]-=0.01;  rouge[1]+=0.01;
  vert[1] -=0.01;  vert[2] +=0.01;  
  bleu[2] -=0.01;  bleu[0] +=0.01;
}
/*= action : variation de couleur =*/
static void action2(void)
{
  rouge[0]+=0.01;  rouge[1]-=0.01;
  vert[1] +=0.01;  vert[2] -=0.01;  
  bleu[2] +=0.01;  bleu[0] -=0.01;
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
  g3x_SetPerspective(40.,100.,1.);
  /* position, orientation de la caméra */
  g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.,(G3Xpoint){0.,0.,0.});

  /* fixe les param. colorimétriques du spot lumineux */
	/* lumiere blanche (c'est les valeurs par defaut)   */	
  g3x_SetLightAmbient (1.,1.,1.);
  g3x_SetLightDiffuse (1.,1.,1.);
  g3x_SetLightSpecular(1.,1.,1.);

  /* fixe la position et la direction du spot lumineux */
	/* (c'est les valeurs par defaut)                    */	
  g3x_SetLightPosition (10.,10.,10.);
  g3x_SetLightDirection( 0., 0., 0.);  

	g3x_SetScrollWidth(4);
	g3x_CreateScrollh_d("shin",&shin,0.0,1.0,1.0,"intensite lumiere brillance ");
	g3x_CreateScrollh_d("spec",&spec,0.0,1.0,1.0,"intensite lumiere speculaire");
	g3x_CreateScrollh_d("diff",&diff,0.0,1.0,1.0,"intensite lumiere diffuse   ");
	g3x_CreateScrollh_d("ambi",&ambi,0.0,1.0,1.0,"intensite lumiere ambiante  ");
	
  /* action attachées à des touches */
  g3x_SetKeyAction('g',action1,"variation de couleur");
  g3x_SetKeyAction('G',action2,"variation de couleur");
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
