#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Arc {
    int sommet;
    struct Arc* arc_suivant;
};

struct Station {
    int sommets_station[10];
    int nb_sommet;
};

typedef struct EnsembleStations {
    struct Station liste_stations[15]; // Un seul tableau de stations
    int nb_stations; // Le nombre total de stations dans l'ensemble
} EnsembleStations;

typedef struct Arc* PointeurArc;

struct Sommet {
    PointeurArc arc;
    char couleur;
    int nb_predecesseurs;
};

typedef struct Sommet* PointeurSommet;

typedef struct Graphe {
    int ordre;
    PointeurSommet* pointeurs_sommets;
} Graphe;

// Déclaration du tableau pour suivre les sommets existants
bool sommets_existants[50] = {false};

Graphe* CreerGraphe(int ordre) {
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe));
    graphe->ordre = ordre;
    graphe->pointeurs_sommets = (PointeurSommet*)malloc(ordre * sizeof(PointeurSommet));

    for (int i = 0; i < ordre; i++) {
        graphe->pointeurs_sommets[i] = (PointeurSommet)malloc(sizeof(struct Sommet));
        graphe->pointeurs_sommets[i]->arc = NULL;
        graphe->pointeurs_sommets[i]->nb_predecesseurs = 0;
    }

    return graphe;
}

void AjouterArete(Graphe* graphe, int s1, int s2) {
    // Marquer les sommets existants
    sommets_existants[s1] = true;
    sommets_existants[s2] = true;

    PointeurArc nouvel_arc = (PointeurArc)malloc(sizeof(struct Arc));
    nouvel_arc->sommet = s2;
    nouvel_arc->arc_suivant = graphe->pointeurs_sommets[s1]->arc;
    graphe->pointeurs_sommets[s1]->arc = nouvel_arc;
    graphe->pointeurs_sommets[s2]->nb_predecesseurs++;
}

void TrouverSommetsSources(Graphe* graphe, int* sources, int* nb_sources) {
    *nb_sources = 0;

    for (int i = 0; i < graphe->ordre; i++) {
        // Ajouter une vérification pour s'assurer que le sommet existe
        if (graphe->pointeurs_sommets[i]->nb_predecesseurs == 0 && sommets_existants[i]) {
            sources[(*nb_sources)++] = i;
        }
    }
}

void BFS(Graphe* graphe, int* sources, int nb_sources, struct Station liste_station[]) {
    int* visite = (int*)malloc(graphe->ordre * sizeof(int));

    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    int* file = (int*)malloc(graphe->ordre * sizeof(int));
    int front = 0;
    int rear = 0;

    for (int i = 0; i < nb_sources; i++) {
        int source = sources[i];
        visite[source] = 1;
        file[rear++] = source;
    }

    printf("BFS à partir des sommets sources :\n");
    int niveau = 0;
    int k = 0 ;

    while (front < rear) {
        int niveau_actuel = rear - front;
        printf("Station %d : %d sommets : ", ++niveau, niveau_actuel);
        liste_station[k].nb_sommet=niveau_actuel;


        k= k+1;
        for (int i = 0; i < niveau_actuel; i++) {
            int sommet_actuel = file[front++];
            printf("%d ", sommet_actuel);

            // Ajout de l'incrémentation de nb_sommet


            liste_station[niveau - 1].sommets_station[i] = sommet_actuel;

            PointeurArc arc_actuel = graphe->pointeurs_sommets[sommet_actuel]->arc;

            while (arc_actuel != NULL) {
                int sommet_adjacent = arc_actuel->sommet;

                if (!visite[sommet_adjacent] && --graphe->pointeurs_sommets[sommet_adjacent]->nb_predecesseurs == 0) {
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



void GrandeStation(struct Station stations[], EnsembleStations* G_station) {
    G_station->nb_stations = 0;

    // Ajouter chaque station individuelle dans la grande station
    for (int i = 0; i < 10; i++) {
        int sommet_present = 0;

        for (int j = 0; j < stations[i].nb_sommet; j++) {
            if (stations[i].sommets_station[j] > 0 && stations[i].sommets_station[j] <= 36) {
                G_station->liste_stations[G_station->nb_stations].sommets_station[j] = stations[i].sommets_station[j];
                sommet_present = 1;
            }
        }

        // Incrémenter le nombre de stations uniquement si au moins un sommet est ajouté
        if (sommet_present) {
            G_station->nb_stations++;
        }
    }
}

void AfficherGraphe(Graphe* graphe) {
    printf("Graphe :\n");

    for (int i = 0; i < graphe->ordre; i++) {
        printf("Sommet %d : ", i);
        PointeurArc arc_actuel = graphe->pointeurs_sommets[i]->arc;

        while (arc_actuel != NULL) {
            printf("%d ", arc_actuel->sommet);
            arc_actuel = arc_actuel->arc_suivant;
        }

        printf("\n");
    }
}

void AfficherStations(struct Station stations[],EnsembleStations* G_station) {
    printf("Toutes les stations dans la grande station :\n");

    for (int i = 0; i < 10; i++) {
        printf("Station %d : ", i + 1);

        for (int j = 0; j < stations[i].nb_sommet; j++) {
            if (G_station->liste_stations[i].sommets_station[j] > 0 ) {
                printf("%d ", G_station->liste_stations[i].sommets_station[j]);
            }
        }

        printf("\n");
    }
}



int main() {
    Graphe* graphe = CreerGraphe(36);
    struct Station liste_station[10];
    EnsembleStations station1;

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

    int sources[graphe->ordre];
    int nb_sources;

    TrouverSommetsSources(graphe, sources, &nb_sources);

    if (nb_sources == 0) {
        printf("Aucun sommet source trouvé.\n");
        return 0;
    }

    printf("Sommets sources : ");
    for (int i = 0; i < nb_sources; i++) {
        printf("%d ", sources[i]);
    }
    printf("\n");

    BFS(graphe, sources, nb_sources, liste_station);

    GrandeStation(liste_station, &station1);

    AfficherStations(liste_station,&station1);

    for (int i = 0; i < 10; ++i) {
        printf("\n%d",liste_station[i].nb_sommet);
    }

    return 0;
}
