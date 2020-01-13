#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define TAILLEGRILLE 10
#define NBBATEAU 5
#define CBATEAU 'O'
#define CATTAQUE 'X'
#define TAILLEPSEUDOJOUEUR 30
#define TAILLELECTURESCORE 50

typedef struct tCase
//Une case peut contenir un bateau et être attaquée.
{
    int nAttaque; //0 Si la case n'est pas attaquée, 1 si elle l'est.
    int nBateau; //Entier identifieur pour le bateau -1 si il n'y en n'a pas, 0,1,2,3,4 pour les différents bateaux.
}tCase;

void Jeu();
void InitGrille(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE]);
void InitBateau(int tnBateau[NBBATEAU]);


int isdigit(int c);
int toupper(int c);
void CreationFlotte(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE]);
void DecoupeEntreeJoueur(char cSeparateur,char cChainePrincipale[4],char cChaineFille1[2],char cChaineFille2[2]);
int AlphaVersEntier(char cAlpha);
int TabCarVersEntier(char cTab[2]);
void CharVersXY (char cHoriz, char cVert[2],int* nX,int* nY);
void PoserBateauEst(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2],int nTailleBateau,int nNumBateau);
void PoserBateauSud(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2],int nTailleBateau,int nNumBateau);
void AttaqueBateau(char cHoriz, char cVert[2],tCase tcTableauEnnemi[TAILLEGRILLE][TAILLEGRILLE],int tnBateauEnnemi[NBBATEAU]);
int FinDePartie(int nBoolJ1,int tnBateauAdversaire[NBBATEAU]);
void TourDuJoueur(tCase tcTableauJoueur[TAILLEGRILLE][TAILLEGRILLE],tCase tcTableauEnnemi[TAILLEGRILLE][TAILLEGRILLE],int tnBateauEnnemi[NBBATEAU]);
void EcritureFichierScore (int nNbTour);
void LectureFichierScore();

int VerifBateauEst(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2],int nTailleBateau,int nNumBateau);
int VerifBateauSud(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2],int nTailleBateau,int nNumBateau);
int VerifDebordementVertical(int nTailleBateau,char cPosVert[2]);
int VerifDebordementHorizontal(int nTailleBateau,char cPosHoriz);
int VerifHorizontal(char cVerif);
int VerifVertical(char cVerif[2]);
int VerifAttaqueValide(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2]);

void color(int t,int f);
void AfficheGrilleJoueur(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE]);
void AfficheGrilleAdversaire(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE]);
void AfficheMenu();

int main()
{
    int nFonctionnement=1;
    char cEntreeMenu[10]={'\0'};

    while (nFonctionnement==1)
    {
        system("cls");
        AfficheMenu();
        fgets(cEntreeMenu,sizeof cEntreeMenu,stdin);
        fflush(stdin);

        if(cEntreeMenu[0]=='j' || cEntreeMenu[0]=='j')
        {
            Jeu();
            getchar();
            fflush(stdin);
        }
        else if(cEntreeMenu[0]=='s' || cEntreeMenu[0]=='S')
        {
            LectureFichierScore();
            getchar();
            fflush(stdin);
        }
        else if(cEntreeMenu[0]=='q' || cEntreeMenu[0]=='Q')
        {
            nFonctionnement=0;
        }
    }
    printf("Fin du jeu, au revoir.\n");
    return 0;
}

void Jeu()
{
    system("cls");
    tCase tcTableauJ1[TAILLEGRILLE][TAILLEGRILLE]; //Grille du Joueur 1.
    tCase tcTableauJ2[TAILLEGRILLE][TAILLEGRILLE]; //Grille du Joueur 2.
    InitGrille(tcTableauJ1);
    InitGrille(tcTableauJ2);

    int tnBateauJ1[NBBATEAU]={0}; //Bateaux du Joueur 1.
    int tnBateauJ2[NBBATEAU]={0}; //Bateaux du Joueur 2.
    /*Pour les bateaux ils seront indexés de 0 à 4.
    Soit    0 pour le torpilleur Structure 2 cases,
            1 pour le 1er contre-torpilleur Structure 3 cases,
            2 pour le 2ème contre-torpilleur Structure 3 cases,
            3 pour le croiseur Structure 4 cases,
            4 pour le porte-avions Structure 5 cases.*/
    InitBateau(tnBateauJ1);
    InitBateau(tnBateauJ2);

    int nBoolJ1=1; //Etat de la partie si on en est au joueur 1 ou 2.
    int nFinDePartie=0;
    int nNombredeTour=1;

    printf("Bonjour et Bienvenue dans ce jeu de Bataille Navale !\nNous allons commencer avec le Joueur 1, Joueur 2 veuillez laisser lui laisser la machine.\nAppuyez sur n'importe quelle touche quand c'est bon.");
    getchar();
    fflush(stdin);
    system("cls");
    //AfficheGrilleJoueur(tcTableauJ1);
    //AfficheGrilleAdversaire(tcTableauJ2);
    CreationFlotte(tcTableauJ1);
    system("cls");
    printf("Nous allons passer au tour du Joueur 2 pour creer sa flotte, Joueur 1 veuillez laisser lui laisser la machine.\nAppuyez sur n'importe quelle touche quand c'est bon.");
    getchar();
    fflush(stdin);
    system("cls");
    CreationFlotte(tcTableauJ2);
    system("cls");

    printf("Le jeu va pouvoir commencer !\n");

    while (nFinDePartie==0)
    {
        if (nBoolJ1==1)
        {
            system("cls");
            printf("Nous allons passer au tour du joueur 1, veuillez prendre la main.\nAppuyez sur une touche quand c'est bon.\n");
            getchar();
            fflush(stdin);
            system("cls");
            TourDuJoueur(tcTableauJ1,tcTableauJ2,tnBateauJ2);
            nFinDePartie=FinDePartie(nBoolJ1,tnBateauJ2);
            nBoolJ1=0;
            printf("Appuyez sur Entree pour passer a la suite.\n");
            getchar();
            fflush(stdin);
        }
        else
        {
            system("cls");
            printf("Nous allons passer au tour du joueur 2, veuillez prendre la main.\nAppuyez sur une touche quand c'est bon.\n");
            getchar();
            fflush(stdin);
            system("cls");
            TourDuJoueur(tcTableauJ2,tcTableauJ1,tnBateauJ1);
            nFinDePartie=FinDePartie(nBoolJ1,tnBateauJ1);
            nBoolJ1=1;
            nNombredeTour+=1;
            printf("Appuyez sur Entree pour passer a la suite.\n");
            getchar();
            fflush(stdin);
        }
    }
    printf("Vous avez gagne en %d tours !\n",nNombredeTour);
    EcritureFichierScore(nNombredeTour);
}

void TourDuJoueur(tCase tcTableauJoueur[TAILLEGRILLE][TAILLEGRILLE],tCase tcTableauEnnemi[TAILLEGRILLE][TAILLEGRILLE],int tnBateauEnnemi[NBBATEAU])
{
    int nBoolEntreeValide=0;
	char cEntreeJoueur[5]={'\0'};
	char cHorizontal[2]={'\0'};
	char cVertical[2]={'\0'};

	while (nBoolEntreeValide==0)
    {
        do
        {
            AfficheGrilleJoueur(tcTableauJoueur);
            AfficheGrilleAdversaire(tcTableauEnnemi);
            printf("Veuillez rentrer les coordonnes de votre attaque sous la forme A/1.\n");
            fgets(cEntreeJoueur, sizeof cEntreeJoueur, stdin);
            fflush(stdin);
            DecoupeEntreeJoueur('/',cEntreeJoueur,cHorizontal,cVertical);
        }
        while (!(VerifHorizontal(cHorizontal[0])==1 && VerifVertical(cVertical)));

        if (VerifAttaqueValide(tcTableauEnnemi,cHorizontal[0],cVertical)==1)
        {
            AttaqueBateau(cHorizontal[0],cVertical,tcTableauEnnemi,tnBateauEnnemi);
            nBoolEntreeValide=1;
        }
        else
        {
            printf("Attaque invalide, vous avez deja attaque cette case.\n");
        }
    }
}

void AttaqueBateau(char cHoriz, char cVert[2],tCase tcTableauEnnemi[TAILLEGRILLE][TAILLEGRILLE],int tnBateauEnnemi[NBBATEAU])
{
    int nX,nY;
    CharVersXY(cHoriz,cVert,&nX,&nY);
    nX+=-1;//Réglage d'un décalage dû au tableau commençant par 0 et non par 1.
    nY+=-1;//Réglage d'un décalage dû au tableau commençant par 0 et non par 1.
    if (tcTableauEnnemi[nX][nY].nBateau>-1)
    {
        tnBateauEnnemi[tcTableauEnnemi[nX][nY].nBateau]-=1;
        printf("Touche !\n");
        if (tnBateauEnnemi[tcTableauEnnemi[nX][nY].nBateau]<1)
        {
            printf("Coule !\n");
        }
    }
    tcTableauEnnemi[nX][nY].nAttaque=1;
}

//Initialisations

void InitGrille(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE])
{
    int nI,nJ;
    for (nI=0; nI<TAILLEGRILLE; nI++)
    {
        for (nJ=0; nJ<TAILLEGRILLE; nJ++)
        {
            tcGrille[nI][nJ].nAttaque=0;
            tcGrille[nI][nJ].nBateau=-1;
        }
    }
}

void InitBateau(int tnBateau[NBBATEAU])
{
    int nI;
    for (nI=0; nI<NBBATEAU; nI++)
    {
        if(nI>=2)
        {
            tnBateau[nI]=nI+1;
        }
        else if(nI<2)
        {
            tnBateau[nI]=nI+2;
        }
    }
}

//Créations

void CreationFlotte(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE])
{
	int nBoolEntreeValide=0;
	char cEntreeJoueur[5]={'\0'};
	char cHorizontal[2]={'\0'};
	char cVertical[2]={'\0'};
	char cDirection=' ';
	int nTailleBateau=5;
	int nNumBateau=4;
    printf("Nous allons mettre en place votre Flotte, vous avez cinq bateaux a poser :\n-Un porte-avions de cinq case\n-Un croiseur de 4 cases\n-Deux contre-torpilleur de 3 cases\n-Un torpilleur de 2 cases.\n\nNous allons commencer par le porte-avions.\n");
    while (nBoolEntreeValide==0)
    {
        while (!(VerifHorizontal(cHorizontal[0])==1 && VerifVertical(cVertical)))
        {
            printf("Veuillez rentrer un point de depart sous la forme A/1.\n");
            fgets(cEntreeJoueur, sizeof cEntreeJoueur, stdin);
            fflush(stdin);
            DecoupeEntreeJoueur('/',cEntreeJoueur,cHorizontal,cVertical);
        }
        while (cDirection!='s' && cDirection!='S' && cDirection!='e' && cDirection!='E')
        {
            printf("Veuillez a present entrer la direction vers laquelle votre porte-avion va s'etendre, a savoir S pour Sud ou E pour Est.\n");
            //cDirection=fgetc(stdin);
            cDirection=getc(stdin);
            fflush(stdin);
        }
        if (cDirection=='s' || cDirection=='S')
        {
            if (VerifDebordementVertical(nTailleBateau,cVertical)==1)
            {
                nBoolEntreeValide=1;
                PoserBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
        if (cDirection=='e' || cDirection=='E')
        {
            if(VerifDebordementHorizontal(nTailleBateau,cHorizontal[0])==1)
            {
                nBoolEntreeValide=1;
                PoserBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
    }
    AfficheGrilleJoueur(tcTableau);

    nBoolEntreeValide=0;
    nTailleBateau=4;
	nNumBateau=3;
    while (nBoolEntreeValide==0)
    {
        do
        {
            printf("Nous allons a present passer au croiseur de 4 cases.\nVeuillez rentrer un point de depart sous la forme A/1.\n");
            fgets(cEntreeJoueur, sizeof cEntreeJoueur, stdin);
            fflush(stdin);
            DecoupeEntreeJoueur('/',cEntreeJoueur,cHorizontal,cVertical);
        }
        while (!(VerifHorizontal(cHorizontal[0])==1 && VerifVertical(cVertical)));

        do
        {
            printf("Veuillez a present entrer la direction vers laquelle votre croiseur va s'etendre, a savoir S pour Sud ou E pour Est.\n");
            //cDirection=fgetc(stdin);
            cDirection=getc(stdin);
            fflush(stdin);
        }
        while (cDirection!='s' && cDirection!='S' && cDirection!='e' && cDirection!='E');

        if (cDirection=='s' || cDirection=='S')
        {
            if (VerifDebordementVertical(nTailleBateau,cVertical)==1)
            {
                if(VerifBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau)==1)
                {
                    nBoolEntreeValide=1;
                    PoserBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
                }
                else
                {
                    printf("Entree incorrect, il y a deja un bateau sur le chemin.\n");
                    AfficheGrilleJoueur(tcTableau);
                }
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
        if (cDirection=='e' || cDirection=='E')
        {
            if(VerifDebordementHorizontal(nTailleBateau,cHorizontal[0])==1)
            {
                if(VerifBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau)==1)
                {
                    nBoolEntreeValide=1;
                    PoserBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
                }
                else
                {
                    printf("Entree incorrect, il y a deja un bateau sur le chemin.\n");
                    AfficheGrilleJoueur(tcTableau);
                }
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
    }
    AfficheGrilleJoueur(tcTableau);

    nBoolEntreeValide=0;
    nTailleBateau=3;
	nNumBateau=2;
	while (nBoolEntreeValide==0)
    {
        do
        {
            printf("Nous allons a present passer au premier contre-torpilleur de 3 cases.\nVeuillez rentrer un point de depart sous la forme A/1.\n");
            fgets(cEntreeJoueur, sizeof cEntreeJoueur, stdin);
            fflush(stdin);
            DecoupeEntreeJoueur('/',cEntreeJoueur,cHorizontal,cVertical);
        }
        while (!(VerifHorizontal(cHorizontal[0])==1 && VerifVertical(cVertical)));

        do
        {
            printf("Veuillez a present entrer la direction vers laquelle votre contre-torpilleur va s'etendre, a savoir S pour Sud ou E pour Est.\n");
            //cDirection=fgetc(stdin);
            cDirection=getc(stdin);
            fflush(stdin);
        }
        while (cDirection!='s' && cDirection!='S' && cDirection!='e' && cDirection!='E');

        if (cDirection=='s' || cDirection=='S')
        {
            if (VerifDebordementVertical(nTailleBateau,cVertical)==1)
            {
                if(VerifBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau)==1)
                {
                    nBoolEntreeValide=1;
                    PoserBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
                }
                else
                {
                    printf("Entree incorrect, il y a deja un bateau sur le chemin.\n");
                    AfficheGrilleJoueur(tcTableau);
                }
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
        if (cDirection=='e' || cDirection=='E')
        {
            if(VerifDebordementHorizontal(nTailleBateau,cHorizontal[0])==1)
            {
                if(VerifBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau)==1)
                {
                    nBoolEntreeValide=1;
                    PoserBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
                }
                else
                {
                    printf("Entree incorrect, il y a deja un bateau sur le chemin.\n");
                    AfficheGrilleJoueur(tcTableau);
                }
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
    }
    AfficheGrilleJoueur(tcTableau);

    nBoolEntreeValide=0;
    nTailleBateau=3;
	nNumBateau=1;
    while (nBoolEntreeValide==0)
    {
        do
        {
            printf("Nous allons a present passer au deuxieme contre-torpilleur de 3 cases.\nVeuillez rentrer un point de depart sous la forme A/1.\n");
            fgets(cEntreeJoueur, sizeof cEntreeJoueur, stdin);
            fflush(stdin);
            DecoupeEntreeJoueur('/',cEntreeJoueur,cHorizontal,cVertical);
        }
        while (!(VerifHorizontal(cHorizontal[0])==1 && VerifVertical(cVertical)));

        do
        {
            printf("Veuillez a present entrer la direction vers laquelle votre contre-torpilleur va s'etendre, a savoir S pour Sud ou E pour Est.\n");
            //cDirection=fgetc(stdin);
            cDirection=getc(stdin);
            fflush(stdin);
        }
        while (cDirection!='s' && cDirection!='S' && cDirection!='e' && cDirection!='E');

        if (cDirection=='s' || cDirection=='S')
        {
            if (VerifDebordementVertical(nTailleBateau,cVertical)==1)
            {
                if(VerifBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau)==1)
                {
                    nBoolEntreeValide=1;
                    PoserBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
                }
                else
                {
                    printf("Entree incorrect, il y a deja un bateau sur le chemin.\n");
                    AfficheGrilleJoueur(tcTableau);
                }
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
        if (cDirection=='e' || cDirection=='E')
        {
            if(VerifDebordementHorizontal(nTailleBateau,cHorizontal[0])==1)
            {
                if(VerifBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau)==1)
                {
                    nBoolEntreeValide=1;
                    PoserBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
                }
                else
                {
                    printf("Entree incorrect, il y a deja un bateau sur le chemin.\n");
                    AfficheGrilleJoueur(tcTableau);
                }
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
    }
    AfficheGrilleJoueur(tcTableau);

    nBoolEntreeValide=0;
    nTailleBateau=2;
	nNumBateau=0;
	while (nBoolEntreeValide==0)
    {
        do
        {
            printf("Nous allons terminer avec le torpilleur de 2 cases.\nVeuillez rentrer un point de depart sous la forme A/1.\n");
            fgets(cEntreeJoueur, sizeof cEntreeJoueur, stdin);
            fflush(stdin);
            DecoupeEntreeJoueur('/',cEntreeJoueur,cHorizontal,cVertical);
        }
        while (!(VerifHorizontal(cHorizontal[0])==1 && VerifVertical(cVertical)));

        do
        {
            printf("Veuillez a present entrer la direction vers laquelle votre torpilleur va s'etendre, a savoir S pour Sud ou E pour Est.\n");
            //cDirection=fgetc(stdin);
            cDirection=getc(stdin);
            fflush(stdin);
        }
        while (cDirection!='s' && cDirection!='S' && cDirection!='e' && cDirection!='E');

        if (cDirection=='s' || cDirection=='S')
        {
            if (VerifDebordementVertical(nTailleBateau,cVertical)==1)
            {
                if(VerifBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau)==1)
                {
                    nBoolEntreeValide=1;
                    PoserBateauSud(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
                }
                else
                {
                    printf("Entree incorrect, il y a deja un bateau sur le chemin.\n");
                    AfficheGrilleJoueur(tcTableau);
                }
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
        if (cDirection=='e' || cDirection=='E')
        {
            if(VerifDebordementHorizontal(nTailleBateau,cHorizontal[0])==1)
            {
                 if(VerifBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau)==1)
                {
                    nBoolEntreeValide=1;
                    PoserBateauEst(tcTableau,cHorizontal[0],cVertical,nTailleBateau,nNumBateau);
                }
                else
                {
                    printf("Entree incorrect, il y a deja un bateau sur le chemin.\n");
                    AfficheGrilleJoueur(tcTableau);
                }
            }
            else
            {
                printf("Le bateau sort de l'ecran, veuillez le replacer.\n");
            }
        }
    }
    AfficheGrilleJoueur(tcTableau);

    printf("Votre flotte est creee !\nAppuyez sur une touche pour passer a la suite.\n");
    getchar();
    fflush(stdin);
}

void PoserBateauEst(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2],int nTailleBateau,int nNumBateau)
{
    int nX,nY,nI;
    CharVersXY(cHoriz,cVert,&nX,&nY);
    nX+=-1;
    nY+=-1;
    for (nI=0;nI<nTailleBateau;nI++)
    {
        tcTableau[nX][nY+nI].nBateau=nNumBateau;
    }
}

void PoserBateauSud(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2],int nTailleBateau,int nNumBateau)
{
    int nX,nY,nI;
    CharVersXY(cHoriz,cVert,&nX,&nY);
    nX+=-1;
    nY+=-1;
    for (nI=0;nI<nTailleBateau;nI++)
    {
        tcTableau[nX+nI][nY].nBateau=nNumBateau;
    }
}

void CharVersXY (char cHoriz, char cVert[2],int* nX,int* nY)
{
    *nX=AlphaVersEntier(cHoriz);
    *nY=TabCarVersEntier(cVert);
}

void EcritureFichierScore (int nNbTour)
{
    printf("Veuillez rentrer un nom pour le tableau des Scores.\n");
    char cPseudoGagnant[TAILLEPSEUDOJOUEUR]={'\0'};
    fgets(cPseudoGagnant,sizeof cPseudoGagnant,stdin);
    fflush(stdin);
    strtok(cPseudoGagnant,"\n"); //Pour gérer la pression de la touche entrée à la fin.

    FILE* fScore = NULL;
    fScore = fopen("Score.bin","ab");
    if (fScore != NULL)
    {
        fprintf(fScore,"%s gagne en %d tours."+'\0',cPseudoGagnant,nNbTour);
    }
    else
    {
        printf("Impossible d'ouvrir le fichier Score.bin.\n");
    }
    fclose(fScore);
}

void LectureFichierScore()
{
    char cLectureFichier[TAILLELECTURESCORE]="";
    FILE* fScore = NULL;
    fScore = fopen("Score.bin","rb");
    if (fScore != NULL)
    {
        printf("TABLEAU DES SCORES :\n\n\n");
        while (fgets(cLectureFichier, TAILLELECTURESCORE, fScore) != NULL)
        {
            printf("%s\n",cLectureFichier);
        }
    }
    else
    {
        printf("Impossible d'ouvrir le fichier Score.bin, ou alors il n'y a encore aucun score enregistre.\n");
    }
    fclose(fScore);
}

//Vérifications

void DecoupeEntreeJoueur(char cSeparateur,char cChainePrincipale[4],char cChaineFille1[2],char cChaineFille2[2])
{
    if (cChainePrincipale[0]!=cSeparateur && cChainePrincipale[0]!=' ' && cChainePrincipale[0]!='\0')
    {
        cChaineFille1[0]=cChainePrincipale[0];
    }
    if (cChainePrincipale[1]!=cSeparateur && cChainePrincipale[1]!=' ' && cChainePrincipale[1]!='\0')
    {
        cChaineFille1[1]=cChainePrincipale[1];
    }
    if (cChainePrincipale[2]!=cSeparateur && cChainePrincipale[2]!=' ' && cChainePrincipale[2]!='\0' &&  isdigit(cChainePrincipale[2]))
    {
        cChaineFille2[0]=cChainePrincipale[2];
    }
    if (cChainePrincipale[3]!=cSeparateur && cChainePrincipale[3]!=' ' && cChainePrincipale[3]!='\0' && isdigit(cChainePrincipale[3]))
    {
        cChaineFille2[1]=cChainePrincipale[3];
    }
}

int VerifHorizontal(char cVerif)
{
    int nVerif=0;
    if (cVerif=='a' || cVerif=='A' || cVerif=='b' || cVerif=='B' || cVerif=='c' || cVerif=='C' || cVerif=='d' || cVerif=='D' || cVerif=='e' || cVerif=='E' || cVerif=='f' || cVerif=='F' || cVerif=='g' || cVerif=='G' || cVerif=='h' || cVerif=='H' || cVerif=='i' || cVerif=='I' || cVerif=='j' || cVerif=='J')
    {
        nVerif=1;
    }
    return nVerif;
}

int VerifVertical(char cVerif[2])
{
    int nVerif=0;
    if(isdigit(cVerif[0]))
    {
        if(cVerif[0]!=1 && cVerif[0]!=0)
        {
            nVerif=1;
        }
        if (cVerif[0]==1)
        {
            if (cVerif[1]==0)
            {
                nVerif=1;
            }
        }
    }
    return nVerif;
}

int VerifDebordementVertical(int nTailleBateau,char cPosVert[2])
{
    int nVerif=0;
    if (cPosVert[0]!='1')
    {
        if (nTailleBateau+strtol(&cPosVert[0],NULL,1)<11)
        {
            nVerif=1;
        }
    }
    else if (cPosVert[1]!='0')
    {
        nVerif=1;
    }
    return nVerif;
}

int VerifDebordementHorizontal(int nTailleBateau,char cPosHoriz)
{
    int nHoriz,nVerif=0;
    nHoriz = AlphaVersEntier(cPosHoriz);
    if ((nHoriz+nVerif)<11)
    {
        nVerif=1;
    }
    return nVerif;
}

int AlphaVersEntier(char cAlpha)
{
    int nEntier;
    switch(toupper(cAlpha))
    {
        case 'A' :
            nEntier=1;
            break;
        case 'B' :
            nEntier=2;
            break;
        case 'C' :
            nEntier=3;
            break;
        case 'D' :
            nEntier=4;
            break;
        case 'E' :
            nEntier=5;
            break;
        case 'F' :
            nEntier=6;
            break;
        case 'G' :
            nEntier=7;
            break;
        case 'H' :
            nEntier=8;
            break;
        case 'I' :
            nEntier=9;
            break;
        default :
            nEntier=10;
            break;
    }
    return nEntier;
}

int TabCarVersEntier(char cTab[2])
{
    int nEntier;
    if (cTab[0]!='1')
    {
        switch (cTab[0])
        {
            case '2' :
                nEntier=2;
                break;
            case '3' :
                nEntier=3;
                break;
            case '4' :
                nEntier=4;
                break;
            case '5' :
                nEntier=5;
                break;
            case '6' :
                nEntier=6;
                break;
            case '7' :
                nEntier=7;
                break;
            case '8' :
                nEntier=8;
                break;
            default :
                nEntier=9;
                break;
        }
    }
    else if (cTab[1]=='0')
    {
        nEntier=10;
    }
    else
    {
        nEntier=1;
    }
    return nEntier;
}

int VerifBateauEst(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2],int nTailleBateau,int nNumBateau)
{
    int nBool=1;
    int nX,nY,nI;
    CharVersXY(cHoriz,cVert,&nX,&nY);
    nX+=-1;
    nY+=-1;
    for (nI=0;nI<nTailleBateau;nI++)
    {
        if(tcTableau[nX][nY+nI].nBateau>-1)
        {
            nBool=0;
        }
    }
    return nBool;
}

int VerifBateauSud(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2],int nTailleBateau,int nNumBateau)
{
    int nBool=1;
    int nX,nY,nI;
    CharVersXY(cHoriz,cVert,&nX,&nY);
    nX+=-1;
    nY+=-1;
    for (nI=0;nI<nTailleBateau;nI++)
    {
        if(tcTableau[nX+nI][nY].nBateau>-1)
        {
            nBool=0;
        }
    }
    return nBool;
}

int VerifAttaqueValide(tCase tcTableau[TAILLEGRILLE][TAILLEGRILLE],char cHoriz, char cVert[2])
{
    int nBool=1;
    int nX,nY;
    CharVersXY(cHoriz,cVert,&nX,&nY);
    nX+=-1;
    nY+=-1;
    if (tcTableau[nX][nY].nAttaque==1)
    {
        nBool=0;
    }
    return nBool;
}

int FinDePartie(int nBoolJ1,int tnBateauAdversaire[NBBATEAU])
{
    int nI, nBoolFin=1;
    for (nI=0;nI<NBBATEAU;nI++)
    {
        if (tnBateauAdversaire[nI]>0)
        {
            nBoolFin=0;
            return nBoolFin;
        }
    }
    if (nBoolFin==1)
    {
        if (nBoolJ1==1)
        {
            printf("Le Joueur 1 a gagne !\n");
        }
        else
        {
            printf("Le Joueur 2 a gagne !\n");
        }
    }
    return nBoolFin;
}

//Affichages

void color(int t,int f)
//Les couleurs qu'on utilise, 0 pour noir, 7 pour gris clair, 2 pour du vert, 14 pour du jaune, 12 pour du rouge, 11 pour du cyan.
{
	HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H,f*16+t);
}

void AfficheGrilleJoueur(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE])
{
    printf("\n Votre grille.\n");
    int nI,nJ;
    for (nI=0;nI<TAILLEGRILLE;nI++)
    {
        printf("\n_______________________________\n|");
        for (nJ=0;nJ<TAILLEGRILLE;nJ++)
        {
            if(tcGrille[nI][nJ].nBateau>-1)
            {
                if(tcGrille[nI][nJ].nAttaque==1)
                {
                    color(12,0); //Couleur Rouge.
                }
                else
                {
                    color(11,0); //Couleur Cyan
                }
                printf("%2c",CBATEAU);
                color(7,0);//On remet gris sur fond noir.
                printf("|");
            }
            else if (tcGrille[nI][nJ].nAttaque==1)
            {
                color(12,0); //Couleur Rouge.
                printf("%2c",CATTAQUE);
                color(7,0);//On remet gris sur fond noir.
                printf("|");
            }
            else
            {
                printf("%2c|",' ');
            }
        }
    }
    printf("\n_______________________________\n");
}


void AfficheGrilleAdversaire(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE])
{
    printf("\n La grille de votre adversaire.\n");
    int nI,nJ;
    for (nI=0;nI<TAILLEGRILLE;nI++)
    {
        printf("\n_______________________________\n|");
        for (nJ=0;nJ<TAILLEGRILLE;nJ++)
        {
            if(tcGrille[nI][nJ].nAttaque==1)
            {
                if(tcGrille[nI][nJ].nBateau>-1)
                {
                    color(12,0); //Couleur Verte.
                    printf("%2c",CBATEAU);
                }
                else
                {
                    color(12,0); //Couleur Rouge.
                    printf("%2c",CATTAQUE);
                }
                color(7,0);//On remet gris sur fond noir.
                printf("|");
            }
            else
            {
                printf("%2c|",' ');
            }
        }
    }
    printf("\n_______________________________\n");
}

void AfficheMenu()
{
    color(11,0); //Couleur Cyan
    printf("BATAILLE NAVALE EN C !\n");
    color(2,0); //Couleur Verte.
    printf("\n[J]");
    color(7,0);//On remet gris sur fond noir.
    printf("ouer\n");
    color(14,0); //Couleur Jaune.
    printf("\n[S]");
    color(7,0);//On remet gris sur fond noir.
    printf("core\n");
    color(12,0); //Couleur Rouge.
    printf("\n[Q]");
    color(7,0);//On remet gris sur fond noir.
    printf("uitter\n");
}
