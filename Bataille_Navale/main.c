#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define TAILLEGRILLE 10
#define NBBATEAU 5
#define CBATEAU 'O'
#define CATTAQUE 'X'

typedef struct tCase
//Une case peut contenir un bateau et être attaquée.
{
    int nAttaque; //0 Si la case n'est pas attaquée, 1 si elle l'est.
    int nBateau; //Entier identifieur pour le bateau -1 si il n'y en n'a pas, 0,1,2,3,4 pour les différents bateaux.
}tCase;

void Jeu();
void InitGrille(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE]);
void InitBateau(int tnBateau[NBBATEAU]);


void color(int t,int f);
void AfficheGrilleJoueur(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE]);
void AfficheGrilleAdversaire(tCase tcGrille[TAILLEGRILLE][TAILLEGRILLE]);

int main()
{

    Jeu();

    return 0;
}

void Jeu()
{
    tCase tcTableauJ1[TAILLEGRILLE][TAILLEGRILLE]; //Grille du Joueur 1.
    tCase tcTableauJ2[TAILLEGRILLE][TAILLEGRILLE]; //Grille du Joueur 2.
    InitGrille(tcTableauJ1);
    InitGrille(tcTableauJ2);

    int tnBateauJ1[NBBATEAU]; //Bateaux du Joueur 1.
    int tnBateauJ2[NBBATEAU]; //Bateaux du Joueur 2.
    /*Pour les bateaux ils seront indexés de 0 à 4.
    Soit    0 pour le torpilleur Structure 2 cases,
            1 pour le 1er contre-torpilleur Structure 3 cases,
            2 pour le 2ème contre-torpilleur Structure 3 cases,
            3 pour le croiseur Structure 4 cases,
            4 pour le porte-avions Structure 5 cases.*/
    InitBateau(tnBateauJ1);
    InitBateau(tnBateauJ2);

    int nBoolJ1=1; //Etat de la partie si on en est au joueur 1 ou 2.

    printf("Bonjour et Bienvenue dans ce jeu de Bataille Navale !\nNous allons commencer avec le Joueur 1, Joueur 2 veuillez laisser lui laisser la machine.\nAppuyez sur n'importe quelle touche quand c'est bon.");
    getchar();
    fflush(stdin);
    system("cls");


    AfficheGrilleJoueur(tcTableauJ1);
    AfficheGrilleAdversaire(tcTableauJ2);


}

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
        else if(nI==1)
        {
            tnBateau[nI]=nI+2;
        }
    }
}

void CreationFlotte()
{
    printf("Nous allons mettre en place votre Flotte, vous avez cinq bateaux à poser : ");
}

//Affichage

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
                    color(7,0); //Couleur Rouge.
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
                    color(2,0); //Couleur Verte.
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

