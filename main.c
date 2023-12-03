#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/// precedance

struct Arc {
    int sommet;
    struct Arc* arc_suivant;
};

typedef struct Arc* PointeurArc;

struct Station {
    int sommets_station[10];
    int nb_sommet;
    int nb_stations;
    float temps_tache;
};

typedef struct EnsembleStations {
    struct Station liste_stations[15]; // Un seul tableau de stations
    int nb_stations; // Le nombre total de stations dans l'ensemble
    float temps_totale_gStation;
} EnsembleStations;

struct Sommet {
    PointeurArc arc;
    int nb_predecesseurs;
};

typedef struct Sommet* PointeurSommet;

typedef struct Graphe {
    int ordre;
    PointeurSommet* pointeurs_sommets;
} Graphe;



/// exclusions

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
    int* indices; // tableau d'indices (pour tache)

} Graphe_exclu;


/// exclusion

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

    return nb_tache+1;
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
        t[i].couleur = 0;
        t[i].degre = 0;
    }
}

void init_graphe(Graphe_exclu * graphe, int max) {
    graphe->matrice_adjacence = (int**)malloc((max + 1) * sizeof(int*));
    for (int i = 0; i <= max; ++i) {
        graphe->matrice_adjacence[i] = (int*)calloc((max + 1), sizeof(int));
    }
}

void creer_arete_matrice(Graphe_exclu* graphe, int op1, int op2) {
    graphe->matrice_adjacence[op1][op2] = 1;
    graphe->matrice_adjacence[op2][op1] = 1;

    //printf("%d et %d\n", op1, op2);
}

void lire_graphe(FILE* ifs, taches* t, int ordre, Graphe_exclu* graphe) {
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

void init_graphe_bis(Graphe_exclu* graphe, int ordre, taches* t){
    graphe->t = (taches*)malloc(ordre * sizeof(taches));
    graphe->indices = (int*)malloc(ordre * sizeof(int));

    for (int i = 0; i < ordre; ++i)
    {
        graphe->t[i].num = t[i].num;
        graphe->t[i].couleur = t[i].couleur;
        graphe->t[i].degre = t[i].degre;
        for (int j = 0; j < t[i].degre; ++j)
        {
            graphe->t[i].tab_exclu[j] = t[i].tab_exclu[j];
        }
        graphe->indices[i] = i; // Initialisation des indices
    }
}

void exclusion(taches* t, int ordre, Graphe_exclu* graphe) {

    init_graphe_bis(graphe, ordre, t);

    // tri pour welsh and powell -> dans ordre decroissant
    for (int i = 0; i < ordre - 1; ++i) {
        for (int j = i + 1; j < ordre; ++j) {
            if (graphe->t[i].degre < graphe->t[j].degre) {
                taches temp = graphe->t[i];
                graphe->t[i] = graphe->t[j];
                graphe->t[j] = temp;

                // Mise à jour des indices
                int temp_index = graphe->indices[i];
                graphe->indices[i] = graphe->indices[j];
                graphe->indices[j] = temp_index;
            }
        }
    }

    /// on remarque juste que les sommets n'apparaissant pas dans file exclusions sont oublié, or a mettre qd meme dans stations, attention

    int couleur = 0;
    graphe->t[0].couleur = 0;


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
        // si aucune couleur précédemment utilisee est disponible
        if (couleur_disponible) {
            // attribuer une nouvelle couleur
            graphe->t[i].couleur = couleur;
            couleur = (couleur + 1) % ordre; // Utilisation de toutes les couleurs disponibles
        }
    }

    for (int i = 0; i < ordre; ++i) {
        t[graphe->indices[i]].couleur = graphe->t[i].couleur;
    }



    /* test bon fonctionnement
    for (int i = 0; i < ordre; ++i) {
        printf("degre %d du sommet %d et de couleur %d\n", graphe->t[i].degre, graphe->t[i].num, graphe->t[i].couleur);
    }*/

}

int tri_par_couleur(Graphe_exclu* graphe, int ordre, taches* tache) {

    // nombre de couleurs utilisées
    int couleur_max = 0;
    for (int i = 0; i < ordre; ++i) {
        if (graphe->t[i].couleur > couleur_max) {
            couleur_max = graphe->t[i].couleur;
        }
    }


    /* affichage
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
    printf("\n");*/

    return couleur_max;
}


/// precedance

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
        // verification que le sommet existe bien
        if (graphe->pointeurs_sommets[i]->nb_predecesseurs == 0 && sommets_existants[i]) {
            sources[(*nb_sources)++] = i;
        }
    }
}

void BFS(Graphe* graphe, int* sources, int nb_sources, int* nb_stations, struct Station liste_stations[]) {
    int* visite = (int*)malloc(graphe->ordre * sizeof(int));

    for (int i = 0; i < graphe->ordre; i++) {
        visite[i] = 0;
    }

    int* file = (int*)malloc(graphe->ordre * sizeof(int));
    int avant = 0;
    int arriere = 0;

    int nombre_stations = 0; // Nouveau compteur pour le nombre de stations

    for (int i = 0; i < nb_sources; i++) {
        int source = sources[i];
        visite[source] = 1;
        file[arriere++] = source;
    }

    printf("BFS a partir des sommets sources :\n");
    int niveau = 0;
    int k = 0;

    while (avant < arriere) {
        int niveau_actuel = arriere - avant;
        printf("Station %d : %d sommets : ", ++niveau, niveau_actuel);
        liste_stations[k].nb_sommet = niveau_actuel;

        k = k + 1;
        for (int i = 0; i < niveau_actuel; i++) {
            int sommet_actuel = file[avant++];
            printf("%d ", sommet_actuel);

            // Ajout de l'incrémentation de nb_sommet
            liste_stations[niveau - 1].sommets_station[i] = sommet_actuel;

            PointeurArc arc_actuel = graphe->pointeurs_sommets[sommet_actuel]->arc;

            while (arc_actuel != NULL) {
                int sommet_adjacent = arc_actuel->sommet;

                if (!visite[sommet_adjacent] && --graphe->pointeurs_sommets[sommet_adjacent]->nb_predecesseurs == 0) {
                    visite[sommet_adjacent] = 1;
                    file[arriere++] = sommet_adjacent;
                }

                arc_actuel = arc_actuel->arc_suivant;
            }
        }

        nombre_stations++; // Incrémentation du nombre de stations
        printf("\n");

        *nb_stations = nombre_stations;
    }

    printf("Nombre total de stations : %d\n", nombre_stations);
    liste_stations[0].nb_stations = nombre_stations;

    free(visite);
    free(file);
}



/// mix

void ajouterStation(struct Station **stations, int *nb_station, int position) {
    // Vérifier si la position est valide
    position=position-1;
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
    (*stations)[position].nb_sommet = 0;

}

void ajout_sommet_station(struct Station **stations, int sommet, int position) {
    position=position-1;

    (*stations)[position].sommets_station[(*stations)[position].nb_sommet] = sommet;
    (*stations)[position].nb_sommet = (*stations)[position].nb_sommet + 1;

}

void sup_sommet_station(struct Station **stations, int position, int indice){
    position=position-1;
    (*stations)[position].sommets_station[indice]=-1;
}

int indice_exclu(struct Station liste_station[], taches* t, int ordre)
{
    // max = nb de mini station
    int max = liste_station[0].nb_stations;
    //printf("%d", max);
    int sommet;

    // test pour toutes les mini stations donc tant que != 0
    for (int s = 0; s < max; ++s)
    {
        /// remplir dans [] le nb max de sommet dans la station
        for (int i = 0; i < liste_station[s].nb_sommet; ++i)
        {

            for (int a = 0; a < ordre; ++a) {
                if (t[a].num == liste_station[s].sommets_station[i]) sommet = a;
            }

            for (int k = 0; k < t[sommet].degre; ++k)
            {
                for (int j = 0; j < liste_station[s].nb_sommet; ++j) {
                    if(t[sommet].tab_exclu[k] == liste_station[s].sommets_station[j])
                    {
                        return s+2;
                    }
                }
            }

        }
    }
    return 0;
}

int tab_exclu(struct Station liste_station[], taches* t, int ordre) {

    // voir si possible de faire tableau dynamique

    int p = 0;
    int sommet;

    int max = liste_station[0].nb_stations;

    for (int s = 0; s < max; ++s)
    {
        /// remplir dans [] le nb max de sommet dans la station
        for (int i = 0; i < liste_station[s].nb_sommet; ++i)
        {

            for (int a = 0; a < ordre; ++a) {
                if (t[a].num == liste_station[s].sommets_station[i]) sommet = a;
            }

            for (int k = 0; k < t[sommet].degre; ++k)
            {
                for (int j = 0; j < liste_station[s].nb_sommet; ++j) {
                    if(t[sommet].tab_exclu[k] == liste_station[s].sommets_station[j])
                    {
                        p = t[sommet].tab_exclu[k];
                        return p;
                    }
                }
            }
        }
    }
}

int pos_exclu(struct Station liste_station[], taches* t, int ordre) {

    // voir si possible de faire tableau dynamique

    int sommet;

    int max = liste_station[0].nb_stations;

    for (int s = 0; s < max; ++s)
    {
        /// remplir dans [] le nb max de sommet dans la station
        for (int i = 0; i < liste_station[s].nb_sommet; ++i)
        {

            for (int a = 0; a < ordre; ++a) {
                if (t[a].num == liste_station[s].sommets_station[i]) sommet = a;
            }

            for (int k = 0; k < t[sommet].degre; ++k)
            {
                for (int j = 0; j < liste_station[s].nb_sommet; ++j) {
                    if(t[sommet].tab_exclu[k] == liste_station[s].sommets_station[j])
                    {
                        int pos;
                        pos = j;

                        return pos;
                    }
                }

            }
        }
    }
}

int test_exclu(struct Station liste_station[], taches* t, int ordre)
{
    // max = nb de mini station
    int max = liste_station[0].nb_stations;
    //printf("%d", max);
    int sommet;

    // test pour toutes les mini stations donc tant que != 0
    for (int s = 0; s < max; ++s)
    {
        /// remplir dans [] le nb max de sommet dans la station
        for (int i = 0; i < liste_station[s].nb_sommet; ++i)
        {

            //printf("nb sommet = %d de station %d", liste_station[s].nb_sommet, s+1);

            for (int a = 0; a < ordre; ++a) {
                if (t[a].num == liste_station[s].sommets_station[i]) sommet = a;
            }

            //printf("%d\n", liste_station[s].sommets_station[i]);
            //printf("%d\n", t[sommet].degre);

            for (int k = 0; k < t[sommet].degre; ++k)
            {
                for (int j = 0; j < liste_station[s].nb_sommet; ++j) {
                    //printf("%d et %d \n",t[sommet].tab_exclu[k], liste_station[s].sommets_station[j]);
                    if(t[sommet].tab_exclu[k] == liste_station[s].sommets_station[j])
                    {
                        return 1;
                    }
                }

            }
            //printf("\n");
        }
    }
    return 0;
}

void appliquer_exclusion(struct Station liste_station[], taches* t, int ordre,int *nb_station)
{
    while(test_exclu(liste_station, t, ordre)==1)
    {

        printf("\nindice %d\n", indice_exclu(liste_station, t, ordre));

        // creer nouvelle mini station a un indice
        ajouterStation(&liste_station, nb_station, indice_exclu(liste_station, t, ordre));


        int excluu = tab_exclu(liste_station, t, ordre);
        printf("%d sommet exclu\n", excluu);

        ajout_sommet_station (&liste_station, excluu, indice_exclu(liste_station, t, ordre));


        printf("\n");
        printf("Stations apres ajout :\n");
        printf("\n");
        for (int i = 0; i < liste_station[0].nb_stations; i++) {
            printf("Station %d : ", i + 1);

            for (int j = 0; j < liste_station[i].nb_sommet; j++) {
                if (liste_station[i].sommets_station[j]!=0){
                    printf("%d ", liste_station[i].sommets_station[j]);
                }
            }
            printf("\n");
        }
    }
}

void temps(FILE* temps, struct Station liste_station[],struct grandeStation tab[], taches* t, int ordre,int* nb_station)
{
    float temps_cycle;
    fscanf(temps, "%f", &temps_cycle);

    int sommet;
    float temps_tot = 0;
    for (int i = 0; i < *nb_station; i++) {

        float max = 0;
        for (int j = 0; j < liste_station[i].nb_sommet; j++) {
            if (liste_station[i].sommets_station[j]!=-1){

                // pour comparer avec tache
                for (int a = 0; a < ordre; ++a) {
                    if (t[a].num == liste_station[i].sommets_station[j]) sommet = a;
                }

                // temps de chaque station
                if (t[sommet].temps > max) max = t[sommet].temps;
                else max = max;
                liste_station[i].temps_tache= max;
                //printf("%f", liste_station[i].temps_tache);

            }
        }
        // addition
        temps_tot = temps_tot + max;
    }

    printf("\ntemps total : %f\n", temps_tot);

}


void G_station (FILE* temps, struct Station **stations,struct grandeStation **Gstation, int nb_station){
    float temps_cycle;
    fscanf(temps, "%f", &temps_cycle);

    float tempsT=0;
    int nb_Gstation=1;
    for (int i = 0; i < nb_station; ++i) {

        if (tempsT>=0 && tempsT<=temps_cycle){
            tempsT=tempsT+(*stations)[i].temps_tache;
            (*Gstation)[1].temps=(*Gstation)[1].temps+tempsT;

            (*Gstation)[1].station[i]=(*stations)[i].sommets_station[i];
            nb_Gstation=1;
        }

        if (tempsT>temps_cycle && tempsT<=2*temps_cycle){
            tempsT=tempsT+(*stations)[i].temps_tache;

            (*Gstation)[2].temps=(*Gstation)[2].temps+(tempsT-temps_cycle);
            (*Gstation)[2].station[i]=(*stations)[i].sommets_station[i];
            nb_Gstation=2;
        }

        if (tempsT>2*temps_cycle && tempsT<=3*temps_cycle){
            tempsT=tempsT+(*stations)[i].temps_tache;

            (*Gstation)[3].temps=(*Gstation)[3].temps+(tempsT-2*temps_cycle);
            (*Gstation)[3].station[i]=(*stations)[i].sommets_station[i];
            nb_Gstation=3;
        }

        if (tempsT>3*temps_cycle && tempsT<=4*temps_cycle){
            tempsT=tempsT+(*stations)[i].temps_tache;

            (*Gstation)[4].temps=(*Gstation)[4].temps+(tempsT-3*temps_cycle);
            (*Gstation)[4].station[i]=(*stations)[i].sommets_station[i];
            nb_Gstation=4;
        }

    }
    printf("\nil y a %d grande station",nb_Gstation);
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

    FILE* fichier = fopen("../precedances.txt", "r");
    if (fichier == NULL) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    FILE* tps = fopen("../temps_cycle.txt", "r");
    if (exclu == NULL) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }




    int ordre = trouver_ordre(op);
    int max = trouver_max(op);

    /// exclusions

    taches* tache;
    tache = (taches*)malloc((ordre) * sizeof(taches));

    Graphe_exclu* graphe_exclu;
    graphe_exclu = (Graphe_exclu *)malloc(sizeof(Graphe_exclu));

    init(op, tache, ordre);

    init_graphe(graphe_exclu, max);
    lire_graphe(exclu, tache, ordre, graphe_exclu);

    exclusion(tache, ordre, graphe_exclu);

    int couleur_max = tri_par_couleur(graphe_exclu, ordre, tache);



    /// precedances

    // Déclaration du tableau pour suivre les sommets existants
    bool sommets_existants[50] = {false};

    Graphe* graphe = CreerGraphe(max+1);
    struct Station liste_station[30];
    EnsembleStations station1;


    int s1, s2;

    while (fscanf(fichier, "%d %d", &s1, &s2) == 2) {
        AjouterArete(graphe, s1, s2);
    }


    int sources[ordre];
    int nb_sources;

    TrouverSommetsSources(graphe, sources, &nb_sources);

    if (nb_sources == 0) {
        printf("Aucun sommet source trouvé\n");
        return 0;
    }

    printf("Sommets sources : ");
    for (int i = 0; i < nb_sources; i++) {
        printf("%d ", sources[i]);
    }
    printf("\n");

    BFS(graphe, sources, nb_sources, liste_station);

    GrandeStation(liste_station, &station1);

    //AfficherStations(liste_station,&station1);
    //CalculerTempsTaches(graphe, liste_station, fichier2,station1);


    fclose(fichier);

    return 0;
}