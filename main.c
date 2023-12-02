#include <stdio.h>


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

    ReorganiserStations(G_station);
}
int main() {
    CalculerTempsTaches(Graphe* graphe, struct Station* liste_station, FILE* fichier_temps , EnsembleStations G_station)
}
