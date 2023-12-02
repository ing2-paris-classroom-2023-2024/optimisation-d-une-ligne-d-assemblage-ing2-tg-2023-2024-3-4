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

void exclusion(taches* t, int ordre, Graphe* graphe) {

    init_graphe_bis(graphe, ordre, t);

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

    /// on remarque juste que les sommets n'apparaissant pas dans file exclusions sont oublié, or a mettre qd meme dans stations, attention

    int couleur = 0;
    graphe->t[0].couleur = 0;
    int couleur_max = 0;

    for (int i = 1; i < ordre; ++i) {
        int couleur_disponible = 1;

        // parcour des couleurs précédemment utilisées
        for (int c = 0; c < i; ++c) {
            // test si couleur précédemment utilisée crée un conflit avec ses voisins
            int conflit = 0;
            for (int k = 0; k < i; ++k) {
                if (graphe->matrice_adjacence[graphe->t[i].num][graphe->t[k].num] == 1 &&
                    graphe->t[k].couleur == c) {
                    conflit = 1;
                    break;
                }
            }
            // si aucune violation
            if (!conflit) {
                // utiliser la couleur précédemment utilisée
                graphe->t[i].couleur = c;
                couleur_disponible = 0;
                break;
            }
        }
        // si aucune couleur précédemment utilisée est disponible
        if (couleur_disponible) {
            // attribuer une nouvelle couleur
            graphe->t[i].couleur = couleur;
            couleur = (couleur + 1) % ordre; // Utilisation de toutes les couleurs disponibles
        }
    }

    /// test bon fonctionnement
    for (int i = 0; i < ordre; ++i) {
        printf("degre %d du sommet %d et de couleur %d\n", graphe->t[i].degre, graphe->t[i].num, graphe->t[i].couleur);
    }

}

void tri_par_couleur(Graphe* graphe, int ordre) {

    // nombre de couleurs utilisées
    int couleur_max = 0;
    for (int i = 0; i < ordre; ++i) {
        if (graphe->t[i].couleur > couleur_max) {
            couleur_max = graphe->t[i].couleur;
        }
    }

    // affichage
    printf("\ntaches par couleur :\n");
    for (int i = 0; i <= couleur_max; ++i) {
        printf("Couleur %d: ", i);
        for (int j = 0; j < ordre; ++j) {
            if (graphe->t[j].couleur == i) {
                printf("%d ", graphe->t[j].num);
            }
        }
        printf("\n");
    }
}

/*
void exclusion(taches* t, int ordre, Graphe* graphe) {
    init_graphe_bis(graphe, ordre, t);

    // Sorting for Welsh and Powell
    for (int i = 0; i < ordre - 1; ++i) {
        for (int j = i + 1; j < ordre; ++j) {
            if (graphe->t[i].degre < graphe->t[j].degre) {
                taches temp = graphe->t[i];
                graphe->t[i] = graphe->t[j];
                graphe->t[j] = temp;
            }
        }
    }

    int couleur_max = 0;

    // Coloring vertices
    for (int i = 0; i < ordre; ++i) {
        int couleur_disponible = 1;

        // Check available colors
        for (int c = 0; c < i; ++c) {
            int conflit = 0;

            // Check for conflicts with neighbors
            for (int k = 0; k < i; ++k) {
                if (graphe->matrice_adjacence[graphe->t[i].num][graphe->t[k].num] == 1 &&
                    graphe->t[k].couleur == c) {
                    conflit = 1;
                    break;
                }
            }

            // Assign color if no conflict
            if (!conflit) {
                graphe->t[i].couleur = c;
                couleur_disponible = 0;
                break;
            }
        }

        // Assign a new color if no available colors
        if (couleur_disponible) {
            graphe->t[i].couleur = couleur_max;
            couleur_max++;
        }
    }

    printf("Nombre maximum de couleurs : %d\n", couleur_max);

    // Arrays to store vertices of the same color
    int** vertices_par_couleur = (int**)malloc(couleur_max * sizeof(int*));
    for (int i = 0; i < couleur_max; ++i) {
        vertices_par_couleur[i] = (int*)malloc(ordre * sizeof(int));
    }

    // Initialize arrays
    for (int i = 0; i < couleur_max; ++i) {
        for (int j = 0; j < ordre; ++j) {
            vertices_par_couleur[i][j] = -1;
        }
    }

    // Populate arrays with vertices of the same color
    for (int i = 0; i < ordre; ++i) {
        int couleur = graphe->t[i].couleur;
        int j = 0;
        while (vertices_par_couleur[couleur][j] != -1) {
            j++;
        }
        vertices_par_couleur[couleur][j] = graphe->t[i].num;
    }

    // Print vertices for each color
    for (int i = 0; i < couleur_max; ++i) {
        printf("Vertices with color %d: ", i);
        int j = 0;
        while (vertices_par_couleur[i][j] != -1) {
            printf("%d ", vertices_par_couleur[i][j]);
            j++;
        }
        printf("\n");
    }

    // Free allocated memory
    for (int i = 0; i < couleur_max; ++i) {
        free(vertices_par_couleur[i]);
    }
    free(vertices_par_couleur);
}
*/

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


    exclusion(tache, ordre, graphe);
    tri_par_couleur(graphe, ordre);



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
