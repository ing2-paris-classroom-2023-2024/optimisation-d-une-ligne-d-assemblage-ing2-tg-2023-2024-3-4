#include <stdio.h>
#include <stdlib.h>

typedef struct tache
{
    float temps;
    int num;
    int tab_exclu[10]; /// a changer dynamiquement
}taches;



/// test station liste chainée

typedef struct station
{
    int sommet_station[20]; /// rendre dynamique
    struct station* suivant;
}stations;


//
stations* creer_maillon(int valeur, int i)
{
    stations *nouveau_maillon;
    nouveau_maillon = (stations*) malloc(sizeof(stations));
    nouveau_maillon->sommet_station[i] = valeur; /// a changer
    nouveau_maillon->suivant = NULL;
    return nouveau_maillon;
}

void afficher_liste_chainee(stations *ancre, int i) {
    stations *actuel = ancre;
    do {
        printf("%d ", actuel->sommet_station[i]); /// a changer
        actuel = actuel->suivant;
    } while (actuel != NULL);
    printf("\n\n");
}

void ajouter_maillon_en_fin_de_chaine(stations *ancre, stations *nouveau_maillon) {
    stations *actuel=ancre;
    while(actuel->suivant!=NULL)
    {
        actuel=actuel->suivant;
    }
    actuel->suivant = nouveau_maillon;
}


int trouver_ordre(FILE* op)
{
    int nb_tache = 0;

    int poubelle1;
    float poubelle2;

    // s'arrete quand le fichier est vide
    while (feof(op)==0)
    {
        fscanf(op, "%d%f", &poubelle1, &poubelle2);
        if (feof(op)!=0) break;

        // compte taille du fichier
        nb_tache++;
    }
    rewind(op);

    return nb_tache;
}

void init(FILE* op, taches* t, int ordre)
{
    for (int i = 0; i < ordre; ++i) {
        fscanf(op, "%d %f", &t[i].num, &t[i].temps);
    }
}

void lire_graphe(FILE* ifs, taches*  t, int ordre) {

    int op1, op2;

    /// faire des tableau dynamique
    int tab[100];
    int tabj[100];

    int compteur = 0;
    int i = 0;

    while (feof(ifs) == 0)
    {
        fscanf(ifs, "%d%d", &op1, &op2);
        if(feof(ifs) != 0) break;

        tab[i] = op1;
        tabj[i] = op2;
        i++;

        // compte taille du fichier (nombre d'exclusions)
        compteur++;
    }

    int n;
    // on prend un struct tache
    for (int k = 0; k < ordre; ++k) {
        n = 0;
        // on parcours tableau qui contient toutes le op
        for (int j = 0; j < compteur; ++j) {
            // qd op correspond a la tache
            if(t[k].num == tab[j])
            {
                // on rajoute exclu de l'op au tab_exclu de la tache
                t[k].tab_exclu[n] = tabj[j];
                n++;
            }
            if (t[k].num == tabj[j])
            {
                t[k].tab_exclu[n] = tab[j];
                n++;
            }
        }
        for (n = n; n < 10 ; ++n) {
            t[k].tab_exclu[n] = -1;
        }
    }


    //test
    for (int j = 0; j < ordre; ++j) {
        int k=0;
        while(t[j].tab_exclu[k] != -1)
        {
            printf("%d exclu de %d\n", t[j].num, t[j].tab_exclu[k]);
            k++;
        }
    }

}


void exclusion(taches* t, int ordre){

    stations *ancre = NULL;
    int tab_op_stations[20];

    int i = 0;

    for (int j = 0; j < ordre; ++j) {
        int k=0;
        while(t[j].tab_exclu[k] != -1)
        {

            k++;
        }
    }

    // faire welsh powell, si nouvelle couleur alors nouvelle station

}

int main() {

    FILE *exclu = fopen("../exclusions.txt", "r");
    if (exclu == NULL) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    FILE *op = fopen("../operations.txt", "r");
    if (!op) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }



    int ordre = trouver_ordre(op);
    //printf("%d\n", ordre);


    taches* tache;
    tache = (taches*)malloc(ordre*sizeof(taches));







    init(op, tache, ordre);
    /*for (int i = 0; i < ordre; ++i) {
        printf("%d de %f\n", tache[i].num, tache[i].temps);
    }*/


    lire_graphe(exclu, tache, ordre);


    return 0;
}


/// SOURCES

// lien pour detection fin fichier : https://zestedesavoir.com/tutoriels/755/le-langage-c-1/1043_aggregats-memoire-et-fichiers/les-fichiers-2/#:~:text=lecture%20et%20%C3%A9criture-,D%C3%A9tection%20d'erreurs%20et%20fin%20de%20fichier,la%20survenance%20d'une%20erreur
//

