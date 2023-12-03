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
    int temps_tache;
};



typedef struct EnsembleStations {
    struct Station liste_stations[15]; // Un seul tableau de stations
    int nb_stations; // Le nombre total de stations dans l'ensemble
    float temps_totale_gStation;
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

void BFS(Graphe* graphe, int* sources, int nb_sources, int* nb_stations ,struct Station liste_station[]) {
    int* visite = (int*)malloc(graphe->ordre * sizeof(int));

    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    int* file = (int*)malloc(graphe->ordre * sizeof(int));
    int front = 0;
    int rear = 0;

    int nombre_stations = 0; // Nouveau compteur pour le nombre de stations

    for (int i = 0; i < nb_sources; i++) {
        int source = sources[i];
        visite[source] = 1;
        file[rear++] = source;
    }

    printf("BFS à partir des sommets sources :\n");
    int niveau = 0;
    int k = 0;

    while (front < rear) {
        int niveau_actuel = rear - front;
        printf("Station %d : %d sommets : ", ++niveau, niveau_actuel);
        liste_station[k].nb_sommet = niveau_actuel;

        k = k + 1;
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

        nombre_stations++; // Incrémentation du nombre de stations
        printf("\n");

        *nb_stations = nombre_stations;
    }

    printf("Nombre total de stations : %d\n", nombre_stations);

    free(visite);
    free(file);
}




void GrandeStation(struct Station stations[], EnsembleStations* G_station) {
    G_station->nb_stations = 0;

    // Ajouter chaque station individuelle dans la grande station
    for (int i = 0; i < 10; i++) {
        int sommet_present = 0;

        for (int j = 0; j < stations[i].nb_sommet; j++) {
            if (stations[i].sommets_station[j] > 0 && stations[i].sommets_station[j] <= 37) {
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
/*
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
*/
/*
void CalculerTempsTaches(Graphe* graphe, struct Station* liste_station, FILE* fichier_temps , EnsembleStations G_station) {
    float temps_tache[graphe->ordre]; // Tableau pour stocker les temps de chaque tâche
    float temps_Gstation = 0;

    // Initialiser le tableau des temps à zéro
    for (int i = 0; i < graphe->ordre; i++) {
        temps_tache[i] = 0.0;
    }

    // Lire les temps associés aux tâches depuis le fichier
    int num_tache;
    float temps;
    while (fscanf(fichier_temps, "%d %f", &num_tache, &temps) == 2) {
        temps_tache[num_tache - 1] = temps; // -1 car les tâches sont numérotées à partir de 1
    }

    // Parcourir les stations et calculer le temps total pour chaque tâche
    for (int i = 0; i < 10; i++) {
        float temps_max = 0.0;

        for (int j = 0; j < liste_station[i].nb_sommet; j++) {
            int sommet = liste_station[i].sommets_station[j];
            float temps_sommet = temps_tache[sommet - 1];

            if (temps_sommet > temps_max) {
                temps_max = temps_sommet;
            }
        }

        liste_station[i].temps_tache = temps_max;
        temps_Gstation=temps_Gstation+temps_max;
        printf("Temps total pour la station %d : %.2f\n", i + 1, temps_max);
    }
    G_station.temps_totale_gStation=temps_Gstation;
    printf("\nle temps totale de la grande station : %f",G_station.temps_totale_gStation);


}
*/




void ajouterStation(struct Station **stations, int *nb_station, struct Station nouvelle, int position) {
    // Vérifier si la position est valide
    if (position > *nb_station) {
        printf("Position invalide. L'élément ne sera pas ajouté.\n");
        return;
    }

    // Augmenter la taille du tableau
    (*nb_station)++;

    // Réallouer la mémoire pour le tableau avec une taille plus grande
    *stations = realloc(*stations, (*nb_station) * sizeof(struct Station));

    // Décaler les éléments du tableau pour faire de la place à la nouvelle station
    for (int i = *nb_station - 1; i >= position; --i) {
        (*stations)[i] = (*stations)[i - 1];
    }

    // Ajouter la nouvelle station à la position spécifiée
    (*stations)[position] = nouvelle;
}








int main() {
    Graphe* graphe = CreerGraphe(36);
    struct Station *liste_station = malloc(32 * sizeof(struct Station));
    EnsembleStations station1;

    FILE* fichier = fopen("preced.txt", "r");
    FILE* fichier2 = fopen("temps.txt", "r");

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

    int nb_stations = 0;
    BFS(graphe, sources, nb_sources, &nb_stations, liste_station);
    liste_station = realloc(liste_station, (nb_stations) * sizeof(struct Station));

    GrandeStation(liste_station, &station1);

    //AfficherStations(liste_station,&station1);




    struct Station nouvelle;
    nouvelle.sommets_station[0]=40;
    nouvelle.sommets_station[1]=42;

    nouvelle.nb_sommet=2;



    //rajouter_station(liste_station, nb_stations, 2, nouvelle);


    printf("nb de station : %d\n",nb_stations);
    int taille = nb_stations;
    printf("nb de station : %d\n",taille);

    ajouterStation(&liste_station, &taille, nouvelle, 3);



    printf("Stations après ajout :\n");
    for (int i = 0; i < taille; i++) {
        printf("Station %d : ", i + 1);

        for (int j = 0; j < liste_station[i].nb_sommet; j++) {
            if (liste_station[i].sommets_station[j]!=0){
                printf("%d ", liste_station[i].sommets_station[j]);
            }
        }

        printf("\n");
    }

    //CalculerTempsTaches(graphe, liste_station, fichier2,station1);

    return 0;
}