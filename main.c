/**
 * @file main.c
 * @author Antoine Bastos 
 * @author Ramzi Djadja
 * @brief Mini-jeu Attaxx
 * @version 0.1
 * @date 2021-11-22
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_PLATEAU 3
#define TAILLE_MAX_NOM 20
#define SYMBOL_1 'o'
#define SYMBOL_2 'x'
#define VIDE '.'
#define BORD '*'

/**
 * @brief L'interface du jeu.
 * ligne de commande ou interface
 * graphique.
 */
typedef enum {
    CLI = 0,
    GUI = 1
} MODE_I;

/**
 * @brief Le mode de jeu.
 * Humain vs Humain,
 * Humain vs Ordi.
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
 * @param joueur_1 
 * @param joueur_2 
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
                plateau->plateau[i][j] = VIDE;
            } else {
                plateau->plateau[i][j] = BORD;
            }
        }
    }
    plateau->joueurs[0] = joueur_1;
    plateau->joueurs[1] = joueur_2;
    plateau->plateau[1][1] = joueur_2->symbol;
    plateau->plateau[TAILLE_PLATEAU][TAILLE_PLATEAU] = joueur_2->symbol;
    plateau->plateau[1][TAILLE_PLATEAU] = joueur_1->symbol;
    plateau->plateau[TAILLE_PLATEAU][1] = joueur_1->symbol;

    return plateau;
}

/**
 * @brief Affiche le plateau 
 * dans la sortie standard.
 * @param plateau 
 */
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

/**
 * @brief Ajouter un pion au plateau
 * 
 * @param plateau le plateau sur lequel placer le pion
 * @param pi coordonée abscisse du pion
 * @param pj coordonée ordonée du pion
 * @param symbol le symbole à placer
 * @return int retourne le nombre de nouveaux pions de symbole @param symbol
 */
int ajouter_pion(Plateau *plateau, int pi, int pj, char symbol) {

    if (pi < 0 || pj < 0 || pi > TAILLE_PLATEAU || pj > TAILLE_PLATEAU || plateau->plateau[pi][pj] != VIDE)
        return 0;

    int adverses = 0;
    int i, j;
    // vérifications des pions adjacents
    for (i = pi - 1; i <= pi + 1; i++) {
        for (j = pj - 1; j <= pj + 1; j++) {
            if (
                i != pi && j != pj && plateau->plateau[i][j] != VIDE && plateau->plateau[i][j] != symbol && plateau->plateau[i][j] != BORD) {
                adverses += 1;
            }
        }
    }
    if (adverses == 0)
        return 0;
    // On change de symbole les pions adjacents
    for (i = pi - 1; i <= pi + 1; i++) {
        for (j = pj - 1; j <= pj + 1; j++) {
            if (i != pi && j != pj && plateau->plateau[i][j] != VIDE && plateau->plateau[i][j] != BORD) {
                plateau->plateau[i][j] = symbol;
            }
        }
    }
    return adverses + 1;
}

int verifier_gagnant(Plateau *plateau) {
    ;
    return 0;
}

int jouer(Plateau *plateau) {
    int numj = 0;
    int i = -1, j = -1;
    Joueur *joueur = NULL;
    int pions_retournes = 0;

    while ((1)) {
        // vérifier etat du jeu.
        i = -1, j = -1;
        joueur = plateau->joueurs[numj];
        printf("%s (%c), veuillez saisir les coordonnées où jouer (entre 1 et %d) : ", joueur->nom, joueur->symbol, TAILLE_PLATEAU);
        scanf(" %d %d", &i, &j);
        // si le joueur parviens à placer un pion
        if ((pions_retournes = ajouter_pion(plateau, i, j, joueur->symbol)) != 0) {
            // reaffichage du plateau
            affiche_plateau_ascii(*plateau);
            // mise à jour des points
            joueur->score += pions_retournes;
            numj = (numj + 1) % 2;
            // mise à jour des points adverses
            plateau->joueurs[numj]->score -= (pions_retournes - 1);
            // affichage des scores
            printf(
                "Score actuel : %s(%c) %d", joueur->nom, joueur->symbol, joueur->score);
            printf(
                " - %s(%c) %d\n", plateau->joueurs[numj]->nom, plateau->joueurs[numj]->symbol, plateau->joueurs[numj]->score);
        }
    }
    return 0;
}

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

/**
 * @brief Y'a un bleme avec cette fonction
 *  si jamais on laisse des espace /tabulations etc ça fait de la merde
 * faudrait trouver le moyene de clear stdin ou une autre manière
 * 
 * @param dest 
 * @param taille 
 * @return int 
 */
int lire_nom(char *dest, int taille) {

    // char r = 'a';
    // int readcount = 0;

    // for (int i = 0; i < TAILLE_MAX_NOM; i++) {
    //     r = fgetc(stdin);
    //     if (
    //         (r >= 'a' && r <= 'z') ||
    //         (r >= 'A' && r <= 'z') ||
    //         (r >= '0' && r <= '9')) {
    //         dest[i] = r;
    //     } else {
    //         return readcount;
    //     }
    // }

    //return readcount;

    return scanf(" %20s", dest);
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
        } while ((joueur_2 = make_joueur(nom, SYMBOL_2, 0)) == NULL);

        if ((plateau = creer_plateau(joueur_1, joueur_2)) == NULL)
            return 1;
        affiche_plateau_ascii(*plateau);
        printf("\n");
        jouer(plateau);

        break;
    case HO:
        break;
    default:
        return 1;
    }

    return 0;
}
// clang -Wall -Wfatal-errors -std=c17 main.c -o main
