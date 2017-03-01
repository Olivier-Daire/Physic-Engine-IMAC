#include "PMat.h"
#include "Link.h"


/* limites de la zone reelle associee a la fenetre */
static double   wxmin=-7.,wymin=-7.,wxmax=+7.,wymax=+7.;


/* des parametres controlables pour les scrollbars */
static double   rA=0.2,rE=0.2,cE=0.5,bkg=1.;
/* une couleur controlable     */
static G2Xcolor col={0.,0.,1.};

/* des flags booleen pour les boutons 'switch' */
static bool _DISKFLAG_=false;
static bool _SQUAREFLAG_=false;
static bool _ANIMFLAG_=false;

/* un fonction associee a une touche clavier */
static void toggle_square(void) { _SQUAREFLAG_=!_SQUAREFLAG_; }

/* des num. de boutons */
static int  SPOTBUT,TRIGBUT;

int nbm = 11;
int nbl = 2;

double h = 0.1;
G2Xvector GRAVITY = {0, -9.1};
PMat* TabM;
Link* TabL;

Link* L;
PMat* M;

/* un fonction associee a un bouton 'popup' : */
/* remise aux positions initiales             */
static void reset(void)
{

}

/* la fonction d'initialisation */
static void init(void)
{
	reset();

	/* les boutons 'switch' */
	g2x_CreateSwitch("D.",&_DISKFLAG_,"affiche/masque le disque de centre A");
	g2x_CreateSwitch("S.",&_SQUAREFLAG_,"affiche/masque le 'quad' <ABDC>   ");

	/* les boutons selecteurs exclusifs */
	SPOTBUT=g2x_CreateButton("spot","affiche/masque le 'spot' de centre E      ");
	TRIGBUT=g2x_CreateButton("tri.","affiche/masque le triangle tricolore <BCD>");
	g2x_CreatePopUp("reset",reset,"reset positions");

	/* les scrollbars : 1 horiz. / 2 vertic. */
	g2x_CreateScrollh_d("rA" ,&rA,0.1,1.0,1.0,"rayon du disque de centre A      ");	
	g2x_CreateScrollv_d("rE" ,&rE,0.1,1.0,1.0,"rayon du 'spot' de centre E      ");
	g2x_CreateScrollv_d("col",&cE,0.0,1.0,1.0,"couleur du spot 0.:bleu->1.:rouge");
	/* un parametre controlable au pave fleche */
	g2x_SetControlParameter_d(&bkg,"bkg",0.01,0.,1.,'+');

	/* une action attachee a une touce clavier */
	g2x_SetKeyAction('s',toggle_square,"affiche/masque le 'quad' <ABDC>");

	/*
	# Frequence : Fe = 1 / h
	# Masse : m = 1
	# raideur : k = 1 * Fe^2
	# viscosite : z = 0.1 * Fe
	*/
	double Fe = 1 / h;
	double k = 0.7 * Fe*Fe;
	double z = 0.1 * Fe;

	TabM = (PMat*)calloc(nbm, sizeof(PMat));
	nbl = (nbm-1);
	TabL = (Link*)calloc(nbl, sizeof(Link));
	M = TabM;
	G2Xvector vel;
	vel.x = 1;
	vel.y = 1;

	MassFixe(M, (G2Xvector){-5., -0.}); M++;
	Mass(M, (G2Xvector) { -4., 0. }, vel, 1); M++;
	Mass(M, (G2Xvector) { -3., 0. }, vel, 1); M++;
	Mass(M, (G2Xvector) { -2., 0. }, vel, 1); M++;
	Mass(M, (G2Xvector) { -1., 0. }, vel, 1); M++;
	Mass(M, (G2Xvector) { 0., 0. }, vel, 1); M++;
	Mass(M, (G2Xvector) { +1., 0. }, vel, 1); M++;
	Mass(M, (G2Xvector) { +2., 0. }, vel, 1); M++;
	Mass(M, (G2Xvector) { +3., 0. }, vel, 1); M++;
	Mass(M, (G2Xvector) { +4., 0. }, vel, 1); M++;
	MassFixe(M, (G2Xvector){+5., 0.}); M++;

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

static void draw()
{ 
	static char msg[32]="";

	/* les boutons selecteurs exclusifs */
	/*switch (g2x_GetButton())
	{
	  case 0 :  col[0]=cE; col[2]=1.-cE; g2x_Spot(E.x,E.y,rE,col); break;
	  case 1 :  g2x_FillTriangle_3col(B.x,B.y,G2Xr_b ,C.x,C.y,G2Xg_b ,D.x,D.y,G2Xb_b); break;
	}*/

  /* les boutons 'switch' */
	/*if (_SQUAREFLAG_) g2x_FillQuad(A.x,A.y,B.x,B.y,D.x,D.y,C.x,C.y,G2Xya_b);*/
	/*if (_DISKFLAG_)   g2x_FillCircle(A.x,A.y,rA,G2Xca_b);*/

	/**********************************
			HERE
	*******************************/
	M = TabM;
	while (M < TabM + nbm) { M->draw(M); ++M; }
	L = TabL;
	while (L < TabL + nbl) { L->draw(L); ++L; }

	g2x_StaticPrint(350,10,(g2x_Running()?G2Xg:G2Xr),'l',"ANIM <space> : %s",(g2x_Running()?"ON":"OFF"));
}

static void anim(void)
{ 
	/* mise a jour des points */
	M = TabM;
	L = TabL;
	while (M < TabM + nbm) { M->algo(M, h); ++M; }
	while (L < TabL + nbl) { L->algo(L); ++L; }
}

static void quit()
{
	free(TabM);
	free(TabL);
  /* rien à faire */
  fprintf(stderr,"\e[31m->nettoyage et sortie\n\e[0m");
}

int main(int argc, char **argv)
{

	/* creation de la fenetre - titre et tailles (pixels) */
	g2x_InitWindow(*argv,512,512);
	/* zone graphique reelle associee a la fenetre */	
	g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);
	g2x_SetInitFunction(init); /* fonction d'initialisation */
	g2x_SetDrawFunction(draw); /* fonction de dessin        */
	g2x_SetAnimFunction(anim); /* fonction d'animation      */
	g2x_SetExitFunction(quit); /* fonction de sorite        */

	return g2x_MainStart();
}

