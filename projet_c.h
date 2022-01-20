#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>



typedef struct _Voiture
{
	int num_v;
	int ta;
	int tp;
	int da;
	struct _Voiture *suiv;
} Voiture;


typedef struct
{
	Voiture *tete;
} Liste_voitures;


typedef struct
{
	int fv;
	int fo;
	int fr;
} Cycle;





//Calcul du temps d'arrivée
void temps_arrivee(int T, Liste_voitures *l, float lambda);
void ajout_voit_tps_arrivee(Liste_voitures *l,int X,int num_v);
void insertion_fin(Liste_voitures *l, Voiture *v_a_add, float X);
void afficher_liste_voitures(Liste_voitures *l);

//Caldul du temps de passage
void temps_passage(Liste_voitures *l, Cycle feu,float alpha);
int ajout_cycle_voit(Voiture *prem_voit, Voiture *derniere_voit, int cycle);

//Calcul de la durée d'attente
void duree_attente (Liste_voitures *l);

//Ajustements
void correction_num(Liste_voitures *l);

//Ecriture fichier
void ecriture_fichier (Liste_voitures *l);

//Lecture fichier
void lecture_fichier (Liste_voitures *l);

//Tableau de bord
void insertion_fin_simple(Liste_voitures *l, Voiture *v_a_add);
float taille_moyenne_file(Liste_voitures *l);
int taille_max_file(Liste_voitures *l);
float duree_attente_moyenne (Liste_voitures *l);
void tableau_de_bord (Liste_voitures *l);

//Fin
void appli();
int simulation(Cycle feu);
