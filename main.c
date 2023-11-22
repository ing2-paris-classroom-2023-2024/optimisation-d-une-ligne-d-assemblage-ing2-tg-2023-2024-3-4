#include <stdio.h>
#include <stdlib.h>

typedef struct tache
{
    float temps;
    int num;
    int* tab_exclu;
}taches;


// pb comment trouver ordre sans lire données
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

void lire_graphe(FILE* ifs) {

    int op1, op2;

    // faire des tableau dynamique
    int tab[100];
    int tabj[100];

    int compteur = 0;
    int i = 0;
    int j = 0;


    while (feof(ifs) == 0)
    {
        fscanf(ifs, "%d%d", &op1, &op2);
        if(feof(ifs) != 0) break;

        tab[i] = op1;
        tabj[j] = op2;

        i++;
        j++;


        // compte taille du fichier (nombre d'exclusions)
        compteur++;

    }
        for (int k = 0; k < compteur; ++k) {
        printf("%d %d\n", tab[k], tabj[k]);
    }

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
    printf("%d\n", ordre);


    taches* tache;
    tache = (taches*)malloc(ordre*sizeof(taches));


    init(op, tache, ordre);
    for (int i = 0; i < ordre; ++i) {
        printf("%d de %f\n", tache[i].num, tache[i].temps);
    }


    lire_graphe(exclu);


    return 0;
}


/// SOURCES

// lien pour detection fin fichier : https://zestedesavoir.com/tutoriels/755/le-langage-c-1/1043_aggregats-memoire-et-fichiers/les-fichiers-2/#:~:text=lecture%20et%20%C3%A9criture-,D%C3%A9tection%20d'erreurs%20et%20fin%20de%20fichier,la%20survenance%20d'une%20erreur
//

