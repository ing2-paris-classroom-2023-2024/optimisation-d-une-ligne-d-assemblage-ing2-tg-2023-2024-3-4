/*#include <stdio.h>
#include <stdlib.h>

typedef struct tache
{
    float temps;
    int num;
    int couleur;
    int degre;
    int tab_exclu[10];
} taches;

typedef struct
{
    int** matrice_adjacence;
    taches* t;
} Graphe;


int trouver_ordre(FILE* op)
{
    int nb_tache = 0;

    // juste pour faire fonctionner le while (valeur ne sont pas utilisé)
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
    // repartir au debut du fichier operations
    rewind(op);

    return nb_tache;
}

void init(FILE* op, taches* t, int ordre)
{
    for (int i = 0; i < ordre; ++i){
        fscanf(op, "%d %f", &t[i].num, &t[i].temps);
        t[i].couleur = -1;
        t[i].degre = 0;
    }
}


void init_graphe(Graphe* graphe, int ordre, taches* t){
    graphe->matrice_adjacence = (int**)malloc(t[ordre].num * sizeof(int*));
    for (int i = 0; i < t[ordre].num; ++i)
    {
        graphe->matrice_adjacence[i] = (int*)calloc(t[ordre].num, sizeof(int));
    }
}
void init_graphe_bis(Graphe* graphe, int ordre, taches* t){

    graphe->t = (taches*)malloc(ordre * sizeof(taches));

    for (int i = 0; i < ordre; ++i)
    {
        graphe->t[i].num = t[i].num;
        graphe->t[i].couleur = t[i].couleur;
        graphe->t[i].degre = t[i].degre;
        for (int j = 0; j < t[i].degre; ++j)
        {
            graphe->t[i].tab_exclu[j] = t[i].tab_exclu[j];
        }
    }
}

void creer_arete_matrice(Graphe* graphe, int op1, int op2, int ordre)
{
    graphe->matrice_adjacence[op1][op2] = 1;
    graphe->matrice_adjacence[op2][op1] = 1;

    printf("%d et %d\n", op1, op2);


}
void afficher_matrice(Graphe* graphe, int ordre) {
    printf("matrice d'adjacence :\n");

    for (int i = 0; i < graphe->t[ordre].num; ++i) {
        for (int j = 0; j < graphe->t[ordre].num; ++j) {
            for (int k = 1; k < graphe->t[ordre].num; ++k) {
                printf("--> %d et %d", graphe->t[k].num, k);
                if (graphe->t[k].num != k)
                {
                    printf("%d et %d", graphe->t[k].num, k);
                    k++;
                    graphe->matrice_adjacence[i][j] = -1;
                }
            }
            printf("%d ", graphe->matrice_adjacence[i][j]);
        }
        printf("\n");
    }
}

void lire_graphe(FILE* ifs, taches* t, int ordre, Graphe* graphe)
{
    int op1, op2;
    int tab[100];
    int tabj[100];
    int compteur = 0;
    int i = 0;

    while (feof(ifs) == 0)
    {
        fscanf(ifs, "%d%d", &op1, &op2);

        if(feof(ifs) != 0) break;


        // Update to use the modified creer_arete_matrice function
        creer_arete_matrice(graphe, op1, op2, ordre);
        tab[i] = op1;
        tabj[i] = op2;
        i++;
        compteur++;
    }




    for (int k = 0; k < ordre; ++k)
    {
        t[k].degre = 0;
        for (int j = 0; j < compteur; ++j)
        {
            if (t[k].num == tab[j])
            {
                t[k].tab_exclu[t[k].degre] = tabj[j];
                t[k].degre++;
            }
            if (t[k].num == tabj[j])
            {
                t[k].tab_exclu[t[k].degre] = tab[j];
                t[k].degre++;
            }
        }
        for (int n = t[k].degre; n < 10; ++n)
        {
            t[k].tab_exclu[n] = -1;
        }
    }

}



int couleur_dispo(Graphe* graphe, int ordre, int tache, int couleur)
{
    for (int i = 0; i < ordre; ++i)
    {
        if (graphe->matrice_adjacence[tache][i] == 1 && graphe->t[i].couleur == couleur)
        {
            return 0;
        }
    }
    return 1;
}

void exclusion(taches* t, int ordre, Graphe* graphe)
{
    for (int i = 0; i < ordre - 1; ++i)
    {
        for (int j = i + 1; j < ordre; ++j)
        {
            if (graphe->t[i].degre < graphe->t[j].degre)
            {
                taches temp = graphe->t[i];
                graphe->t[i] = graphe->t[j];
                graphe->t[j] = temp;
            }
        }
    }


    int couleur = 0;
    int tache ;

    for (int i = 0; i < ordre; ++i)
    {
        tache = graphe->t[i].num;

        //printf("%d\n", tache);

        while (!couleur_dispo(graphe, ordre, tache, couleur)) // Utiliser l'indice i comme sommet
        {
            couleur++;
        }

        graphe->t[i].couleur = couleur;

        couleur = 0;

    }

    for (int i = 0; i < ordre; ++i)
    {
        printf("Sommet %d (Couleur %d) : Exclusions - ", graphe->t[i].num , graphe->t[i].couleur);
        for (int j = 0; j < ordre; ++j)
        {
            if (graphe->matrice_adjacence[i][j] == 1)
            {
                printf("%d ", graphe->t[j].num);
            }
        }
        printf("\n");
    }
}



int main()
{
    FILE* exclu = fopen("../exclusions.txt", "r");
    if (exclu == NULL)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    FILE* op = fopen("../operations.txt", "r");
    if (!op)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    int ordre = trouver_ordre(op);

    taches* tache;
    tache = (taches*)malloc(ordre * sizeof(taches));

    Graphe* graphe;
    graphe = (Graphe*)malloc(sizeof(Graphe));
    init(op, tache, ordre);


    init_graphe(graphe, ordre, tache);
    lire_graphe(exclu, tache, ordre, graphe);

    init_graphe_bis(graphe, ordre, tache);

    // test: afficher la matrice d'adjacence
    afficher_matrice(graphe, ordre);


    //exclusion(tache, ordre, graphe);




    // Free allocated memory
    free(tache);
    for (int i = 0; i < ordre; ++i)
    {
        free(graphe->matrice_adjacence[i]);
    }
    free(graphe->matrice_adjacence);
    free(graphe);

    fclose(exclu);
    fclose(op);

    return 0;
}*/
#include <stdio.h>
#include <stdlib.h>

typedef struct tache {
    float temps;
    int num;
    int couleur;
    int degre;
    int tab_exclu[10];
} taches;

typedef struct {
    int** matrice_adjacence;
    taches* t;
} Graphe;

int trouver_ordre(FILE* op) {
    int nb_tache = 0;
    int poubelle1;
    float poubelle2;

    while (feof(op) == 0) {
        fscanf(op, "%d%f", &poubelle1, &poubelle2);
        if (feof(op) != 0)
            break;
        nb_tache = poubelle1 > nb_tache ? poubelle1 : nb_tache;
    }

    rewind(op);
    return nb_tache;
}

int trouver_max(FILE* op) {
    int max = 0;
    int poubelle1;
    float poubelle2;

    while (feof(op) == 0) {
        fscanf(op, "%d%f", &poubelle1, &poubelle2);
        if (poubelle1 > max) max = poubelle1;
        else max = max;

        if (feof(op) != 0)
            break;
    }

    //printf("%d\n\n", max);

    rewind(op);
    return max;
}

void init(FILE* op, taches* t, int ordre) {
    for (int i = 0; i < ordre; ++i) {
        fscanf(op, "%d %f", &t[i].num, &t[i].temps);
        t[i].couleur = -1;
        t[i].degre = 0;
    }
}

void init_graphe(Graphe* graphe, int max) {
    graphe->matrice_adjacence = (int**)malloc((max + 1) * sizeof(int*));
    for (int i = 0; i <= max; ++i) {
        graphe->matrice_adjacence[i] = (int*)calloc((max + 1), sizeof(int));
    }
}

void creer_arete_matrice(Graphe* graphe, int op1, int op2) {
    graphe->matrice_adjacence[op1][op2] = 1;
    graphe->matrice_adjacence[op2][op1] = 1;

    //printf("%d et %d\n", op1, op2);
}

void afficher_matrice(Graphe* graphe, int max) {
    printf("matrice d'adjacence :\n");

    for (int i = 0; i <= max; ++i) {
        for (int j = 0; j <= max; ++j) {
            printf("%d ", graphe->matrice_adjacence[i][j]);
        }
        printf("\n");
    }
}

void lire_graphe(FILE* ifs, taches* t, int ordre, Graphe* graphe) {
    int op1, op2;
    int compteur = 0;
    int tab[100];
    int tabj[100];

    while (feof(ifs)==0)
    {
        fscanf(ifs, "%d%d", &op1, &op2);
        if (feof(ifs)!=0) break;

        creer_arete_matrice(graphe, op1, op2);
        tab[compteur] = op1;
        tabj[compteur] = op2;
        compteur++;
    }

    /// a revoir

    // on prend un struct tache
    for (int k = 0; k < ordre; ++k) {
        t[k].degre = 0;
        // on parcours tableau qui contient toutes le op (non ordonnée)
        for (int j = 0; j < compteur; ++j) {
            // qd operation correspond a la tache
            if(t[k].num == tab[j])
            {
                // on rajoute l'exclu de la tache au tab_exclu de celle ci
                t[k].tab_exclu[t[k].degre] = tabj[j];
                t[k].degre++;
            }
            // pareil mais dans l'autre sens
            if (t[k].num == tabj[j])
            {
                t[k].tab_exclu[t[k].degre] = tab[j];
                t[k].degre++;
            }
        }
        // pour eviter des erreur plus tard, detecter qd tache n'a plus d'exclusions
        for (int n = t[k].degre; n < 10 ; ++n) {
            t[k].tab_exclu[n] = -1;
        }
    }


}



void exclusion(taches* t, int ordre, Graphe* graphe) {

    // tri pour welsh and powell
    for (int i = 0; i < ordre - 1; ++i) {
        for (int j = i + 1; j < ordre; ++j) {
            if (graphe->t[i].degre < graphe->t[j].degre) {
                taches temp = graphe->t[i];
                graphe->t[i] = graphe->t[j];
                graphe->t[j] = temp;
            }
        }
    }

    /// verifier si couleur est disponible ect

}



int main() {
    FILE* exclu = fopen("../exclusions.txt", "r");
    if (exclu == NULL) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    FILE* op = fopen("../operations.txt", "r");
    if (!op) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    int ordre = trouver_ordre(op);
    int max = trouver_max(op);

    taches* tache;
    tache = (taches*)malloc((ordre + 1) * sizeof(taches));

    Graphe* graphe;
    graphe = (Graphe*)malloc(sizeof(Graphe));
    init(op, tache, ordre);

    init_graphe(graphe, max);
    lire_graphe(exclu, tache, ordre, graphe);

    afficher_matrice(graphe, max);


    //exclusion(tache, ordre, graphe);

    free(tache);
    for (int i = 0; i <= max; ++i) {
        free(graphe->matrice_adjacence[i]);
    }
    free(graphe->matrice_adjacence);
    free(graphe);

    fclose(exclu);
    fclose(op);

    return 0;
}
