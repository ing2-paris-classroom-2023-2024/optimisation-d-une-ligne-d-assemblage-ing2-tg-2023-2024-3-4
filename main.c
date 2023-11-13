#include <stdio.h>
#include <stdlib.h>

void lire_graphe(char * nomFichier) {

    // ouverture fichier manuel
    FILE *ifs = fopen(nomFichier, "r");
    int op1, op2;

    if (!ifs) {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }


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

    FILE * fp;
    fp= fopen("../exclusions.txt", "r");
    if (fp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    int* tab;

    char nom_fichier[50];
    printf("entrer le nom du fichier du labyrinthe (format:../nom_du_fichier):\n");
    gets(nom_fichier);

    lire_graphe(nom_fichier);


    return 0;
}


/// SOURCES

// lien pour detection fin fichier : https://zestedesavoir.com/tutoriels/755/le-langage-c-1/1043_aggregats-memoire-et-fichiers/les-fichiers-2/#:~:text=lecture%20et%20%C3%A9criture-,D%C3%A9tection%20d'erreurs%20et%20fin%20de%20fichier,la%20survenance%20d'une%20erreur
//

