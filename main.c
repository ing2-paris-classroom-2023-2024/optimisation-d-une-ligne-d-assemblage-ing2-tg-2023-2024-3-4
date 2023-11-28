#include <stdio.h>
#include <stdlib.h>

struct Arc {
    int sommet;
    struct Arc* arc_suivant;
};

struct station {
    int sommets_station[10];
};

typedef struct Arc* pArc;

struct Sommet {
    pArc arc;
    char couleur;
    int nb_predecesseurs;
};

typedef struct Sommet* pSommet;

typedef struct Graphe {
    int ordre;
    pSommet* pSommet;
} Graphe;

Graphe* CreerGraphe(int ordre) {
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));
    graphe->ordre = ordre;
    graphe->pSommet = (pSommet*)malloc(ordre * sizeof(pSommet));
    for (int i = 0; i < ordre; i++) {
        graphe->pSommet[i] = (pSommet)malloc(sizeof(struct Sommet));
        graphe->pSommet[i]->arc = NULL;
        graphe->pSommet[i]->nb_predecesseurs = 0;
    }
    return graphe;
}

void AjouterArete(Graphe* graphe, int s1, int s2) {
    pArc nouvel_arc = (pArc)malloc(sizeof(struct Arc));
    nouvel_arc->sommet = s2;
    nouvel_arc->arc_suivant = graphe->pSommet[s1]->arc;
    graphe->pSommet[s1]->arc = nouvel_arc;
    graphe->pSommet[s2]->nb_predecesseurs++;
}

void BFS(Graphe* graphe, int* sources, int nb_sources, struct station liste_station[]) {
    int* visite = (int*)malloc(graphe->ordre * sizeof(int));
    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    int* file = (int*)malloc(graphe->ordre * sizeof(int));
    int front = 0, rear = 0;

    for (int i = 0; i < nb_sources; i++) {
        int source = sources[i];
        visite[source] = 1;
        file[rear++] = source;
    }

    printf("BFS Ã  partir des sommets sources :\n");
    int niveau = 0;

    while (front < rear) {
        int niveau_actuel = rear - front;
        printf("station %d : ", ++niveau);

        for (int i = 0; i < niveau_actuel; i++) {
            int sommet_actuel = file[front++];
            printf("%d ", sommet_actuel);

            liste_station[niveau - 1].sommets_station[i] = sommet_actuel;

            pArc arc_actuel = graphe->pSommet[sommet_actuel]->arc;
            while (arc_actuel != NULL) {
                int sommet_adjacent = arc_actuel->sommet;

                if (!visite[sommet_adjacent] && --graphe->pSommet[sommet_adjacent]->nb_predecesseurs == 0) {
                    visite[sommet_adjacent] = 1;
                    file[rear++] = sommet_adjacent;
                }

                arc_actuel = arc_actuel->arc_suivant;
            }
        }

        printf("\n");
    }

    free(visite);
    free(file);
}

void AfficherGraphe(Graphe* graphe) {
    printf("Graphe :\n");
    for (int i = 0; i < graphe->ordre; i++) {
        printf("Sommet %d : ", i);
        pArc arc_actuel = graphe->pSommet[i]->arc;
        while (arc_actuel != NULL) {
            printf("%d ", arc_actuel->sommet);
            arc_actuel = arc_actuel->arc_suivant;
        }
        printf("\n");
    }
}

int main() {
    Graphe* graphe = CreerGraphe(36);
    struct station liste_station[10];

    FILE* fichier = fopen("preced.txt", "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int s1, s2;
    while (fscanf(fichier, "%d %d", &s1, &s2) == 2) {
        AjouterArete(graphe, s1, s2);
    }

    fclose(fichier);

    AfficherGraphe(graphe);

    int sources[] = {1, 5}; // Les sommets sources
    int nb_sources = sizeof(sources) / sizeof(sources[0]);

    BFS(graphe, sources, nb_sources, liste_station);

    return 0;
}