#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "projet_c.h"









///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CALCUL DU TEMPS D'ARRIVEE

void temps_arrivee(int T, Liste_voitures *l, float lambda) //T représente le temps total de l'expérience
{
	/*Il faut d'abord créer la liste et initialiser la première voiture*/
	Voiture *voit;
	voit = (Voiture*)malloc(sizeof(Voiture));
	voit->suiv = NULL;
	voit->num_v = 1;
	voit->ta = 0;
	voit->tp = 0;
	voit-> da = 0;

	l->tete = voit;		//La première voiture est mise en début de liste

	int X;		//X contient le temps entre deux voitures 
	float U;
	int compteur = 0;
	int compteur_voiture=2;

	while (compteur<T)
	{
		U = (float)rand()/(float)RAND_MAX; //U e [0;1]
		//printf("U=%f\n",U);
		X = floorf(-logf(1-U)/lambda);
		
		//printf("X=%f\n",X);
		ajout_voit_tps_arrivee(l,X,compteur_voiture);
		compteur += X;
		compteur_voiture += 1;
	}
}









void insertion_fin(Liste_voitures *l, Voiture *v_a_add, float X)	//Ajoute la voiture v_a_add à la fin de la liste tout en initialisant son ta et son tp = ta.
{
	Voiture *voit;
	voit = l->tete;
	
	while (voit->suiv != NULL)
	{
		voit = voit->suiv;
	}

	voit->suiv = v_a_add;
	voit->suiv->ta = voit->ta + X;
	voit->suiv->tp = voit->suiv->ta;			//On initialise le temps de passage au temps d'arrivée
}








void ajout_voit_tps_arrivee(Liste_voitures *l,int X, int num_v)		//Cette fonction ajoute une voiture en appelant la fonction insertion_fin et initialise son numéro
{
	Voiture *voit_a_add;
	voit_a_add = (Voiture*)malloc(sizeof(Voiture));
	voit_a_add->suiv = NULL;
	voit_a_add->num_v = num_v;
	insertion_fin(l,voit_a_add,X);
}










void afficher_liste_voitures(Liste_voitures *l)		//Affiche simplement les infos de chaque voiture à partir d'une liste.
{
	//printf("ON EST DEDAAANS\n");
	Voiture *voit = l->tete;
	while(voit != NULL)
	{
		printf("Voiture n° : %d\n",voit->num_v);
		printf("Temps d'arrivée : %d\n",voit->ta);
		printf("Temps de passage :%d\n",voit->tp);
		printf("Durée d'attente : %d,\n\n",voit->da);
	voit = voit->suiv;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*TEMPS DE PASSAGE */



void temps_passage(Liste_voitures *l, Cycle feu, float alpha)		//Initialise le temps de passage de chaque voiture 
{
    Voiture *voit;
    voit = l->tete;
    int ta;
    int cycle = feu.fv + feu.fr + feu.fo;


    while (voit != NULL)	//On ne s'arrête pas tant que toutes les voitures n'ont pas été traitées
    {
        //printf("Voiture en cours de traitement : n°%d.\n",voit->num_v);

        ta = voit->ta;
        if (ta % cycle <= feu.fv)	//Si elle peut passer au feu vert sans s'arrêter
        {
            
            //voit->tp = ta;		//Inutile car déjà fait dans insertion
            voit->da = 0;
            voit = voit->suiv;
        }


        else 		//Si elle a le feu orange ou rouge, on construit la "liste" des voitures qui s'arrêteront pendant ce temps
        {

            int levier = 0;		//Levier est à 0 si la liste est non vide et passe à 1 quand la liste est vide


            Voiture *prem_voit;	//On "fixe" la première voiture à l'arrêt.
            prem_voit = voit;

            Voiture *derniere_voit;	//Nous servira de pointeur pour parcourir les voitures à "ajouter dans la liste des voitures à l'arrêt"
            derniere_voit = voit;


            Voiture *av_derniere_voit;	//Nous permet de faire en sorte que derniere_voit contiendra la dernière voiture qui sera dans la file et non la première qui ne l'est pas


            int compteur=0;		//Compte le nb de voitures dans la "liste"

            //printf("Dernière voit : n°%d\n", derniere_voit->num_v);
            //printf("Première voit : n°%d\n", prem_voit->num_v);

            while (levier ==0)	//Tant qu'il y a des voitures à l'arrêt
            {
               

                while (derniere_voit !=NULL && derniere_voit->ta - prem_voit->ta <= cycle -(prem_voit->ta % cycle))	//Tant que la voiture pointée par derniere_voit arrive lorsque le feu est rouge
                    //On met dans la "liste" toutes ces voitures
                {
                    compteur +=1;					//On incrémente le compteur de voitures

                    derniere_voit->tp = derniere_voit->tp + (cycle - (derniere_voit->ta%cycle));			//On augmente le temps de passage du temps restant du cycle
                    derniere_voit->num_v = compteur;			//Permet de savoir quelle est la pos de la voiture dans la liste d'arrêt

                    av_derniere_voit = derniere_voit;
                    derniere_voit = derniere_voit -> suiv;			//On passe à la voiture suivante
                }

                derniere_voit = av_derniere_voit;	//On pointe vers la voiture précédente (la derniere qui est dans la file d'arrêt)



                if (compteur*alpha <= feu.fv)				//Si toutes les voitures à l'arrêt ont le temps de passer au prochain feu vert
                {							//On doit incrémenter les tp de alpha*leur pos dans la file

                    Voiture *voit_qui_passe = prem_voit;
                    while (voit_qui_passe != derniere_voit->suiv)
                    {
                        voit_qui_passe->tp = voit_qui_passe->tp + (voit_qui_passe->num_v)*alpha;	//On modifie tp
                        voit_qui_passe = voit_qui_passe->suiv;
                    }

                    levier = 1;			//On sort de toutes ces boucles
                    voit = derniere_voit->suiv;	//On n'oublie pas de compter toutes les voitures que l'on vient de traiter
                }


                else		//Si toutes les voitures ne peuvent pas passer au prochain feu vert
                {

                    while ((prem_voit->num_v)*alpha <= feu.fv)		//On fait passer celles qui peuvent
                    {
                        prem_voit->tp = prem_voit->tp + (prem_voit->num_v)*alpha;	//On modifie tp
                        prem_voit = prem_voit->suiv;
                    }				//A la sortie de la boucle, prem_voit pointe sur la première voiture qui n'a pas pu passer au feu vert

                    compteur = ajout_cycle_voit(prem_voit, derniere_voit,cycle);	//Ajoute au tp de toutes les voitures entre prem_voit et derniere_voit la durée d'un cycle
                }									//et renvoie le nb de voitures dans la file

            }
        }
    }
}


int ajout_cycle_voit(Voiture *prem_voit, Voiture *derniere_voit, int cycle)	//Ajoute la durée d'un cycle au temps de passage de toutes les voitures de la liste
{
	int compteur = 0;		//Contient le nombre de voitures dans la file
	if (prem_voit = derniere_voit)	//S'il n'y a qu'une voiture qui ne peut pas passer
	{
		prem_voit->tp = prem_voit->tp + cycle;
		compteur = 1;
	}
	else	//S'il y a plus d'une voiture qui ne peuvent pas passer
	{

		Voiture *voit = prem_voit;
		while (voit != derniere_voit)
		{
			voit->tp = voit->tp + cycle;
			voit= voit->suiv;
			compteur+=1;
		}
	}
	return(compteur);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DUREE D'ATTENTE

void duree_attente (Liste_voitures *l)
{
	Voiture *voit = l->tete;

	while (voit != NULL)
	{
		voit->da = voit->tp - voit->ta;
		voit=voit->suiv;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//AJUSTEMENTS

void correction_num(Liste_voitures *l)		/*Les numéros des voitures sont modifiés dans "temps_passage" pour permettre d'afficher sa place dans la file d'attente lorsque le feu est rouge, et cela
						  nous permet d'ajouter le bon temps de passage à la voiture en fonction de sa position dans cette file. On les réajuste à la fin de cette fonction pour
						  permettre d'écrire le vrai num des voitures dans le fichier texte*/
{
	int compteur = 1;

	Voiture *voit = l->tete;

	while (voit != NULL)
	{
		voit->num_v = compteur;
		compteur += 1;
		voit = voit->suiv;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ECRITURE FICHIER

void ecriture_fichier (Liste_voitures *l)
{
	FILE *fichier;
	fichier = fopen("liste_des_véhicules.txt","w+");

	fprintf(fichier,"%s\t\t%s\t%s\t%s\n","N° voit","t_arrivée","t_passage","duree_attente");
	Voiture *voit = l->tete;

	while (voit != NULL)
	{
		fprintf(fichier, "%d\t\t%d\t\t%d\t\t%d\n",voit->num_v, voit->ta, voit->tp, voit->da);
		voit= voit->suiv;
	}
	fclose(fichier);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LECTURE FICHIER

void lecture_fichier (Liste_voitures *l) 	//Prend en argument l'adresse à laquelle il doit tout écrire
{
	FILE *fichier;
	fichier = fopen("liste_des_véhicules.txt","r+");

	char p1[100];	
	fgets(p1,100,fichier);		//On élimine la première ligne composée des titres de chaque colonne
	
  

	int num, ta, tp, da;
   


	while (fscanf(fichier, "%d\t\t%d\t\t%d\t\t%d",&num,&ta,&tp,&da) == 4)	//On traite toutes les lignes
	{
		Voiture *v_a_add;
		v_a_add = (Voiture*)malloc(sizeof(Voiture));
		
		/*printf("num : %d\n",num);
		printf("ta : %d\n",ta);
		printf("tp : %d\n",tp);
		printf("da : %d\n\n",da);*/
	
		v_a_add->num_v = num;		//A chaque lecture d'une ligne, on récupère le num de la voiture, son ta, son tp et son da, que l'on ajoute ensuite à la voiture que l'on vient de créer.
		v_a_add->ta = ta;
		v_a_add->tp = tp;
		v_a_add->da = da;
		v_a_add->suiv = NULL;
	
		if (num ==1)	//Si c'est la première voiture à insérer, on l'ajoute directement à la tête de la liste
		{
			l->tete = v_a_add;
		}
		
		else	//Sinon, on l'insère à la fin avec la fonction insertion_fin_simple
		{
			insertion_fin_simple(l, v_a_add);
		}

	}
	fclose(fichier);
}




void insertion_fin_simple(Liste_voitures *l, Voiture *v_a_add)
{
	Voiture *voit;
	voit = l->tete;
	//printf("voit n°%d\n",v_a_add->num_v);
	
	while (voit->suiv != NULL)
	{
		voit = voit->suiv;
	}

	voit->suiv = v_a_add;
	
}







/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TABLEAU DE BORD


float taille_moyenne_file(Liste_voitures *l)		//nb moyen de voitures par file
{
	Voiture *voit;
	voit = l->tete;
	
	float compteur_file =0;	//Compte le nombre de files
	float compteur_voit =0;	//Compte le nombre de voitures qui sont dans des files
	

	while (voit != NULL)
	{
		//printf("da de la voit : %d\n",voit->da);
		if (voit->da != 0)
		{
			
			compteur_file += 1;
			while ( voit != NULL && voit->da !=0)
			{
				compteur_voit +=1;
				voit= voit->suiv;
			}

		}
		else
		{
			voit = voit->suiv;

		}
	}
	//printf("Compteur_voit = %f\n",compteur_voit);
	//printf("compteur_file = %f\n",compteur_file);
	return(compteur_voit / compteur_file);
}




int taille_max_file(Liste_voitures *l)
{
	Voiture *voit;
	voit = l->tete;
	
	int taille_max = 0;
	int compteur_voit = 0;

	while (voit != NULL)
	{
		if (voit->da != 0)
		{
			compteur_voit = 0;
			
			while (voit != NULL && voit->da !=0 )
			{
				//printf("Voiture en cours de traitement : %d\n",voit->num_v);
				compteur_voit += 1;
				voit = voit->suiv;
			}
			
			if (compteur_voit > taille_max)
			{
				taille_max = compteur_voit;
			}
		}
		
		else
		{
			voit = voit->suiv;
		}
	}
	return(taille_max);
}




float duree_attente_moyenne (Liste_voitures *l)
{
	Voiture *voit;
	voit = l->tete;
	
	float somme_da = 0;
	float compteur_voit = 0;
	
	while (voit != NULL)
	{
		somme_da += voit->da;
		compteur_voit += 1;
		
		voit = voit->suiv;
	}
	
	return(somme_da / compteur_voit);
}





void tableau_de_bord (Liste_voitures *l)
{
	FILE *fichier;
	fichier = fopen("tableau_de_bord.txt","w+");
	float t_moyenne_file;
	int t_max_file;
	float d_attente_moyenne; 

	d_attente_moyenne = duree_attente_moyenne(l);

	t_max_file = taille_max_file(l);

	t_moyenne_file = taille_moyenne_file(l);

	
	//printf("taille moeynne : %f\n",t_moyenne_file);	

	//printf("%f, %d, %f\n",d_attente_moyenne, t_max_file, t_moyenne_file);

	fprintf(fichier,"TABLEAU DE BORD\n\n");
	fprintf(fichier,"La taille moyenne des files lorsque le feu est rouge est de : %f voitures.\n",t_moyenne_file);
	fprintf(fichier,"La taille maximale des files lorsque le feu est rouge est de : %d voitures.\n",t_max_file);
	fprintf(fichier,"En moyenne, les voitures attendent %f secondes.\n",d_attente_moyenne);

	fclose(fichier);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FIN

void appli()
{
	float alpha, lambda;
	int Tv,To,Tr;
	int tps;
	
	printf("Saisissez alpha :\t");
	scanf("%f",&alpha);
	printf("\n\n");

	printf("Saisissez lambda :\t");
	scanf("%f",&lambda);
	printf("\n\n");

	printf("Saisissez Tv, To et Tr (séparés d'un espace):\t");
	scanf("%d%d%d",&Tv, &To, &Tr);
	printf("\n\n");
	
	Cycle feu = {Tv, To, Tr};

	tps = simulation(feu);
	


	Liste_voitures l;
	l.tete = NULL;
	

	
	temps_arrivee(tps,&l,lambda);
	
	temps_passage(&l,feu,alpha);
	duree_attente(&l);
	correction_num(&l);
	
	ecriture_fichier(&l);
	//printf("Etape 1 OK\n");
	
	lecture_fichier(&l);
	//printf("Etape 2 OK\n");
	//afficher_liste_voitures(&l);
	tableau_de_bord(&l);
	printf("Simulation finie ! Vous pouvez retrouver la liste des véhicules dans le fichier texte portant le même nom et les résultats de la simulation dans le fichier texte tableau_de_bord.\n\n");
}



int simulation(Cycle feu)	//Permet juste à l'utilisateur de simuler soit un nb de cycle soit un nb de secondes. C'est donc une sorte de menu avec deux choix 
				//et qui renvoie le nb de secondes de la simulation.
{
	int choix;
	int tps_cycle = feu.fv + feu.fo + feu.fr;

	printf("Que souhaitez-vous faire ?\n\n");
	printf("1-Faire la simulation sur un ou plusieurs cycles.\n");
	printf("2-Faire la simulation sur une durée (en secondes).\n");
	printf("Réponse : ");
	scanf("%d",&choix);
	printf("\n\n\n");

	if (choix == 1)
	{
		int nb_cycles;
		printf("Combien de cycles souhaitez vous simuler ? (un cycle = %d secondes)\n", tps_cycle);
		printf("Réponse : ");
		scanf("%d", &nb_cycles);
		printf("\n\n");
		return(nb_cycles*tps_cycle);	//On convertit le nombre de cycles voulus en nb de secondes.
	}
	else if (choix == 2)
	{
		int tps;
		printf("Combien de secondes doit durer la simulation ? \t");
		printf("Réponse : ");
		scanf("%d",&tps);
		printf("\n\n");
		return(tps);
	}
	else
	{
		printf("Erreur, veuillez réessayer.\n\n\n\n\n");
		simulation(feu);
	}
}


