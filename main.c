#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_PLATEAU 3
#define TAILLE_MAX_NOM 20
#define SYMBOL_1 'o'
#define SYMBOL_2 'x'

/**
 * @brief Le mode graphique du jeu
 * ligne de commande ou interface 
 * graphique.
 */
enum G_MODE {
    CLI = 0,
    GUI = 1
};

/**
 * @brief 
 * 
 */
enum J_MODE {
    HH = 0,
    HJ = 1
};

// clang -Wall -Wfatal-errors -std=c17 main.c -o main

typedef struct {
    char nom[TAILLE_MAX_NOM];
    // identifiant de pion sur le plateau
    char symbol;
    // nombre de pions
    int score;
} Joueur;

typedef struct {
    char plateau[TAILLE_PLATEAU + 2][TAILLE_PLATEAU + 2];
    Joueur *joueurs[2];
} Plateau;

/**
 * @brief 
 * 
 * @return Plateau* 
 */
Plateau *creer_plateau(Joueur *joueur_1, Joueur *joueur_2) {
    Plateau *plateau;
    int i, j;
    if ((plateau = (Plateau *)malloc(sizeof(Plateau))) == NULL) {

        return NULL;
    }

    for (i = 0; i < TAILLE_PLATEAU + 2; i++) {
        for (j = 0; j < TAILLE_PLATEAU + 2; j++) {
            if (i < TAILLE_PLATEAU + 1 && i > 0 && j < TAILLE_PLATEAU + 1 && j > 0) {
                plateau->plateau[i][j] = '.';
            } else {
                plateau->plateau[i][j] = '*';
            }
        }
    }
    return plateau;
}

void affiche_plateau_ascii(Plateau plateau) {
    int i, j;

    for (i = 0; i < TAILLE_PLATEAU + 2; i++) {
        for (j = 0; j < TAILLE_PLATEAU + 2; j++) {
            printf("%c", plateau.plateau[i][j]);
            printf(" ");
        }
        //printf(" "); // espace invisible fin de ligne !
        printf("\n");
    }
}

// ajoute_pion()
// gagne()

/**
 * @brief 
 * 
 * @param nom 
 * @param symbol 
 * @param score 
 * @return Joueur* 
 */
Joueur *make_joueur(char nom[TAILLE_MAX_NOM], char symbol, int score) {
    if (strlen(nom) == 0)
        return NULL;
    Joueur *joueur = NULL;
    if ((joueur = (Joueur *)malloc(sizeof(Joueur)))) {
        strcpy(joueur->nom, nom);
        joueur->symbol = symbol;
        return joueur;
    }
    return NULL;
}

int main(int argc, char *argv[]) {

    Plateau *plateau = NULL;
    G_MODE
    Joueur *joueur_1 = NULL;
    Joueur *joueur_2 = NULL;
    char nom[TAILLE_MAX_NOM];

    /*           TRAITEMENT OPTIONS            */
    //options disponibles
    char etat_options[128] = {0};
    // index de l'argument
    int i = 1;
    // pointeur sur la chaine de l'argument
    char *str_option;

    // options par défaut
    etat_options[(int)'g'] = 0;

    for (; i < argc; i++) {
        str_option = argv[i];
        if (*str_option == '-') {
            str_option++;

            for (; *str_option; str_option++) {

                // enregistre choix option
                if (*str_option == 'a') {
                    etat_options[(int)(*str_option)] = 1;
                    etat_options[(int)'g'] = 0;
                }
                //
                if (*str_option == 'g') {
                    etat_options[(int)(*str_option)] = 1;
                    etat_options[(int)'a'] = 0;
                }
                if (*str_option == 'h') {
                    etat_options[(int)(*str_option)] = 1;
                    etat_options[(int)'o'] = 0;
                }
                if (*str_option == 'o') {
                    etat_options[(int)(*str_option)] = 1;
                    etat_options[(int)'h'] = 0;
                }

                etat_options[(int)(*str_option)] = 1;
            }
        }
    }

    /*               JEU                */

    if ((etat_options[(int)'a'])) {

        printf("Affichage ASCII.\n");

    } else if ((etat_options[(int)'g'])) {
        printf("Affichage graphique.\n");
    } else {
        printf("Erreur option manquante.\n");
        return 1;
    }

    if ((etat_options[(int)'h'])) {

        printf("Deux joueurs humains.\n");

        do {
            printf("Quel est le nom du premier joueur (symbol %c) : ", SYMBOL_1);
            scanf("%20s", nom);
            printf("\n");
        } while ((joueur_1 = make_joueur(nom, SYMBOL_1, 0)) == NULL);

        do {
            printf("Quel est le nom du premier joueur (symbol %c) : ", SYMBOL_2);
            scanf("%20s", nom);
            printf("\n");
        } while ((joueur_2 = make_joueur(nom, SYMBOL_1, 0)) == NULL);

        if ((plateau = creer_plateau()) == NULL)
            return 1;
        affiche_plateau_ascii(*plateau);

    } else if ((etat_options[(int)'o'])) {
        printf("Humain contre ordinateur.\n");

    } else {
        printf("Erreur option manquante.\n");

        return 1;
    }

    return 0;
}
