#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_PLATEAU 3
#define TAILLE_MAX_NOM 20
#define SYMBOL_1 'o'
#define SYMBOL_2 'x'

/**
 * @brief L'interface du jeu
 * ligne de commande ou interface 
 * graphique.
 */
typedef enum {
    CLI = 0,
    GUI = 1
} MODE_I;

/**
 * @brief Le mode de jeu
 * Humain vs Humain
 * Humain vs Ordi
 */
typedef enum {
    HH = 0,
    HO = 1
} MODE_J;

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

int lire_nom(char *dest, int taille) {
    char r = 'a';
    int readcount = 0;

    for (int i = 0; i < TAILLE_MAX_NOM; i++) {
        r = fgetc(stdin);
        if ((r >= 'a' && r <= 'z') || (r >= 'A' && r <= 'z') || (r >= '0' && r <= '9')) {
            dest[i] = r;
        } else {
            return readcount;
        }
    }
    return readcount;
}

int main(int argc, char *argv[]) {

    Plateau *plateau = NULL;
    // par défaut en ASCII
    MODE_I interface = CLI;
    // par défaut humain vs ordi
    MODE_J mode_de_jeu = HO;
    Joueur *joueur_1 = NULL;
    Joueur *joueur_2 = NULL;
    char nom[TAILLE_MAX_NOM];

    /*           TRAITEMENT OPTIONS            */
    // index de l'argument
    int i = 1;
    // pointeur sur la chaine de l'argument
    char *str_option;
    // iteration sur les arguments
    for (; i < argc; i++) {
        str_option = argv[i];
        // si est une option
        if (*str_option == '-') {
            str_option++;
            // iteration sur la chaine de l'argument
            for (; *str_option; str_option++) {
                // enregistre choix option
                switch (*str_option) {
                case 'a':
                    interface = CLI;
                    break;
                case 'g':
                    interface = GUI;
                    break;
                case 'h':
                    mode_de_jeu = HH;
                    break;
                case 'o':
                    mode_de_jeu = HO;
                    break;
                default:
                    continue;
                }
            }
        }
    }

    /*               JEU                */
    switch (mode_de_jeu) {
    case HH:
        // La consigne du #define TAILLE_MAX_NOM 20 m'embête
        //
        do {
            printf("Quel est le nom du premier joueur (symbol %c) : ", SYMBOL_1);
            lire_nom(nom, TAILLE_MAX_NOM);
            printf("\n");
        } while ((joueur_1 = make_joueur(nom, SYMBOL_1, 0)) == NULL);

        do {
            printf("Quel est le nom du second joueur (symbol %c) : ", SYMBOL_2);
            lire_nom(nom, TAILLE_MAX_NOM);
            printf("\n");
        } while ((joueur_2 = make_joueur(nom, SYMBOL_1, 0)) == NULL);

        if ((plateau = creer_plateau(joueur_1, joueur_2)) == NULL)
            return 1;
        affiche_plateau_ascii(*plateau);
        printf("\n");

        break;
    case HO:
        break;
    default:
        return 1;
    }

    return 0;
}
// clang -Wall -Wfatal-errors -std=c17 main.c -o main
