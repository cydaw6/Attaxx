/**
 * @file main.c
 * @author Antoine Bastos
 * @author Ramzi Djadja
 * @brief Mini-jeu Attaxx
 * @version 0.1
 * @date 2021-11-26
 *
 */
#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAILLE_PLATEAU 7
#define TAILLE_MAX_NOM 20
#define SYMBOL_1 'o'
#define SYMBOL_2 'x'
#define VIDE '.'
#define BORD '*'
#define NOM_ORDI "Ordi"
#define BACKGROUND_COLOR MLV_COLOR_WHITE
#define PADDING_CASE 1
#define MAX_OPTS 10

/**
 * @brief Pourcentage du côté le plus petit
 * de l'écran.
 */
#define TAILLE_RELATIVE 50

/**
 * @brief Représentation du type d'interface du jeu.
 * ligne de commande ou interface
 * graphique.
 */
typedef enum { CLI = 0, GUI = 1 } TYPE_I;

/**
 * @brief Représentation du mode de jeu.
 * Humain vs Humain,
 * Humain vs Ordi.
 */
typedef enum { HH = 0, HO = 1 } MODE_J;

/**
 * @brief options du mode de jeu.
 * I : probabilité d'infection de
 */
typedef enum { I = 0 } OPTS;

/**
 * @brief Représentation d'un joueur.
 *
 */
typedef struct {
    char nom[TAILLE_MAX_NOM];
    // identifiant de pion sur le plateau
    char symbol;
    // nombre de pions
    int score;
} Joueur;

/**
 * @brief Représentation d'un Plateau attax.
 *
 */
typedef struct {
    char plateau[TAILLE_PLATEAU + 2][TAILLE_PLATEAU + 2];
    Joueur *joueurs[2];
} Plateau;

/**
 * @brief Alloue une structure plateau de jeu et l'intialise avec les premiers
 * pions.
 * Le score des joueurs est définie à 2.
 * @param joueur_1 le premier joueur.
 * @param joueur_2 le deuxième joueur.
 * @return Plateau*
 */
Plateau *creer_plateau(Joueur *joueur_1, Joueur *joueur_2);

/**
 * @brief Affiche le plateau
 * dans la sortie standard.
 * @param plateau Le plateau à afficher.
 */
void affiche_plateau_cli(Plateau plateau);

/**
 * @brief Affiche le Plateau dans une fêtre.
 * @param plateau Le plateau à afficher.
 * @param j_courant Le joueur courant.
 */
void affiche_plateau_gui(Plateau plateau, Joueur j_courant);

/**
 * @brief Fait afficher un plateau donné en fonction
 * du type d'interface choisie en option.
 *
 * @param plateau Le plateau à afficher.
 * @param j_courant Le joueur effectuant le tour.
 * @param interface Le type d'interface.
 */
void affiche_plateau(Plateau plateau, Joueur j_courant, TYPE_I interface);

/**
 * @brief Ajouter un pion au plateau
 *
 * @param plateau le plateau sur lequel placer le pion.
 * @param pi coordonée abscisse du pion.
 * @param pj coordonée ordonée du pion.
 * @param symbol le symbole à placer.
 * @return int retourne le nombre de pions adverses affectés @param symbol
 */
int ajouter_pion(Plateau *plateau, int pi, int pj, char symbol,
                 int opts[MAX_OPTS]);

/**
 * @brief Vide le buffer de l'entrée standard
 */
void vider_buffer();

/**
 * @brief récupère l'entrée clavier utilisateur
 * s'il reste des caractères dans le buffer, vide le buffer.
 * @param dest un pointeur sur la chaine de destination.
 * @param taille le nombre de caractères.
 * @return int
 */
int lire_nom(char *dest, int taille, TYPE_I interface, char *message);

/**
 * @brief Pour un joueur donné, selon le type d'interface,
 * récupère les coordonées de la case où placer le pion.
 *
 * @param joueur Le joueur qui doit choisir une case.
 * @param i Variable de retour de la coordonée abscisse.
 * @param j Variable de retour de la coordonée ordonée.
 * @param interface Le type d'interface.
 * @return int Retourne 0 si les coordonées on été récupérés.
 */
int faire_jouer(Joueur joueur, int *i, int *j, TYPE_I interface);

/**
 * @brief Lance une partie d'attax sur un plateau donné.
 *
 * @param p le plateau.
 * @param interface Le type d'interface.
 * @param mode_jeu Le mode_jeu.
 */
void jouer(Plateau *p, TYPE_I interface, MODE_J mode_jeu, int opts[MAX_OPTS]);

/**
 * @brief Alloue une structure joueur.
 * et initialise ces champs.
 * @param nom Le nom du joueur.
 * @param symbol Le symbole utilisé pour le distingué sur le plateau.
 * @return Joueur* Un pointeur sur la structure joueur alloué.
 */
Joueur *make_joueur(char nom[TAILLE_MAX_NOM], char symbol);

/**
 * @brief Renvoie le nombre de pions adverses adjacents à une case pour un
 * symbole donné et les indices d'une case.
 *
 * @param plateau
 * @param pi
 * @param pj
 * @param symbol
 * @return int
 */
int valeur_case(Plateau plateau, int pi, int pj, char symbol);

/**
void affiche_plateau_gu
 * attaxx en fonction de l'écran utilisateur.
 *
 * @return int # TAILLE_RELATIVE pourcentage du côté le plus petit de l'écran
 * utilisateur
 */
int taille_fenetre();

/**
 * @brief Créer et affiche avec la lib MLV
 */
void init_fenetre();

/**
 * @brief Renvoie un entier alétoire
 * entre deux deux nombre.
 *
 * @param min
 * @param max
 * @return int
 */
int random_int(int min, int max);

/**
 * @brief Renvoie les indices de la case associée aux coordonées x,y
 * dans la fenêtre.
 *
 * @param souris_x cordoonnée abscisse dans la fenêtre
 * @param souris_y cordoonnée ordonnée dans la fenêtre
 * @param ci pointeur pour le retour de l'indice de ligne
 * @param cj pointeur pour le retour de l'indice de colonne
 * @return int 1 si une case les coordonées correspondent à une case, 0 sinon.
 */
int coordonees_vers_indices(int souris_x, int souris_y, int *ci, int *cj);

/**
 * @brief Renvoie 0 si une condition d'arrêt de partie est vraie
 * sinon le jeu continue on renvoie 1.
 *
 * @param plateau
 * @param j_courant
 */
int etat_partie(Plateau plateau, Joueur j_courant);

/**
 * @brief Traite les options de jeu passés en argument du programme.
 *
 * @param interface l'interface de jeu
 * @param mode_jeu le mode je jeu
 * @param argc nombre d'arguments
 * @param argv tableau des chaine d'arguments
 */
void options(TYPE_I *interface, MODE_J *mode_jeu, int *opts, int argc,
             char *argv[]);

/**
 * @brief Alloue les différentes structure du jeu
 * selon le type d'interface.
 *
 * @param plateau un pointeur sur un pointeur de tableau
 * @param interface le type d'interface
 * @param mode_jeu le mode de jeu
 * @return int 0 si le plateau n'a pas pu être alloué
 */

int init_jeu(Plateau **plateau, TYPE_I interface, MODE_J mode_jeu);

/**
 * @brief determine si un point est infecter ou non, avec un pourcentage de 60%
 *
 */
int infectionAleatoire();

/**
 * @brief compte le nombre d'adversaire autour d'une case
 *
 * @param plateau
 * @param pi
 * @param pj
 * @param symbol
 * @return int le nombre d'adversaire atour d'une case
 */
int compteAdversaire(Plateau *plateau, int pi, int pj, char symbol);

int main(int argc, char *argv[]) {
    srand(time(NULL));
    Plateau *plateau = NULL;
    // par défaut en ASCII
    TYPE_I interface = CLI;
    // par défaut humain vs ordi
    MODE_J mode_jeu = HO;
    // options du jeu
    int opts[MAX_OPTS] = {0};

    // Options
    options(&interface, &mode_jeu, opts, argc, argv);

    // Initialiser le jeu
    if (init_jeu(&plateau, interface, mode_jeu) == 0) {
        return 1;
    }

    // Jouer
    jouer(plateau, interface, mode_jeu, opts);

    free(plateau);

    if (interface == GUI) {
        MLV_wait_seconds(1200);
        MLV_free_window();
    }

    return 0;
}

// clang -Wall -Wfatal-errors -std=c17 main.c -o main -lMLV

/************************** GUI ***********************************/

int taille_fenetre() {
    unsigned int width, height;
    MLV_get_desktop_size(&width, &height);
    int taille_fenetre = (width > height) ? height : width;
    taille_fenetre = (int)(taille_fenetre * TAILLE_RELATIVE) / 100;
    return taille_fenetre;
}

void init_fenetre() {

    int taille_f = taille_fenetre();
    MLV_create_window("Attax", "", taille_f, taille_f);
    MLV_draw_filled_rectangle(0, 0, taille_f, taille_f, BACKGROUND_COLOR);
    MLV_actualise_window();
}

int coordonees_vers_indices(int souris_x, int souris_y, int *ci, int *cj) {

    // taille de la fenetre
    int taille_f = taille_fenetre();
    int padding_f = (taille_f * 5) / 100;
    MLV_draw_filled_rectangle(0, 0, taille_f, taille_f, BACKGROUND_COLOR);

    // longueur de la grille
    int taille_grille = (taille_f - (padding_f * 2));
    int taille_case = taille_grille / TAILLE_PLATEAU;

    // grille
    int i, j, x = padding_f, y = (taille_f * 5) / 100;
    for (i = 1; i < TAILLE_PLATEAU + 1; i++) {
        for (j = 1; j < TAILLE_PLATEAU + 1; j++) {
            // si la souris est sur la case, mise en évidence
            if (souris_x > x && souris_x < x + taille_case && souris_y > y &&
                souris_y < y + taille_case) {
                *ci = i;
                *cj = j;
                return 1;
            }
            x += taille_case;
        }

        x = padding_f;
        y += taille_case;
    }
    return 0;
}

void affiche_plateau_gui(Plateau plateau, Joueur j_courant) {

    int longeur_nom = 0, hauteur_nom = 0;
    int taille_f = taille_fenetre();
    // background
    MLV_draw_filled_rectangle(0, 0, taille_f, taille_f, BACKGROUND_COLOR);
    int padding_f = (taille_f * 5) / 100;
    char score[TAILLE_PLATEAU * TAILLE_PLATEAU + 1] = {0};
    int longueur_score = 0, hauteur_score = 0;
    int longueur_nom_1 = 0, hauteur_nom_1 = 0;
    int padding_case = (taille_f * 2) / 100;
    // nom centré du joueur courant
    char tour_nom[TAILLE_MAX_NOM + 13] = {'\0'};

    sprintf(tour_nom, "Au tour de %s", j_courant.nom);
    MLV_get_size_of_text(tour_nom, &longeur_nom, &hauteur_nom);
    // espace à gauche et à droit du nom pour centrer le texte
    int marge = (taille_f - longeur_nom) / 2;
    // position y du nom avec marge top 2%
    int pos_nom_y = (taille_f * 2) / 100;

    MLV_draw_text(marge, pos_nom_y, tour_nom, MLV_COLOR_BLACK);
    // affichage joueur 1 en haut à gauche
    MLV_get_size_of_text(plateau.joueurs[0]->nom, &longueur_nom_1,
                         &hauteur_nom_1);
    MLV_draw_text(padding_f, pos_nom_y, plateau.joueurs[0]->nom,
                  MLV_COLOR_BLACK);
    MLV_get_size_of_text(score, &longueur_score, &hauteur_score);
    // affichage score joueur 1 en haut à gauche
    sprintf(score, "%d", plateau.joueurs[0]->score);
    MLV_get_size_of_text(score, &longueur_score, &hauteur_score);
    MLV_draw_text(padding_f + longueur_nom_1 + (taille_f * 2) / 100, pos_nom_y,
                  score, MLV_COLOR_BLACK);
    // affichage pion joueur 1 en haut à gauche
    MLV_draw_filled_circle(
        padding_f + longueur_nom_1 + longueur_score + (taille_f * 4) / 100,
        pos_nom_y + hauteur_nom_1 / 2, hauteur_nom_1 / 3, MLV_COLOR_BLACK);
    // affichage joueur 2 en haut à droite
    int longueur_nom_2 = 0, hauteur_nom_2 = 0;

    MLV_get_size_of_text(plateau.joueurs[1]->nom, &longueur_nom_2,
                         &hauteur_nom_2);
    MLV_draw_text((taille_f - padding_f) - longueur_nom_2, pos_nom_y,
                  plateau.joueurs[1]->nom, MLV_COLOR_BLACK);
    // affichage score joueur 2 en haut à droite
    sprintf(score, "%d", plateau.joueurs[1]->score);
    MLV_get_size_of_text(score, &longueur_score, &hauteur_score);
    MLV_draw_text((taille_f - padding_f) - longueur_nom_2 - longueur_score -
                      +(taille_f * 2) / 100,
                  pos_nom_y, score, MLV_COLOR_BLACK);
    // affichage pion joueur 2 en haut à droite
    MLV_draw_circle(taille_f - padding_f - longueur_score -
                        (longueur_nom_2 + (taille_f * 4) / 100),
                    pos_nom_y + hauteur_nom_2 / 2, hauteur_nom_1 / 3,
                    MLV_COLOR_BLACK);

    // longueur de la grille
    int taille_grille = (taille_f - (padding_f * 2));
    int taille_case = taille_grille / TAILLE_PLATEAU;

    // grille
    int i, j, x = padding_f, y = (taille_f * 5) / 100;
    int souris_x = 0, souris_y = 0;

    MLV_get_mouse_position(&souris_x, &souris_y);
    for (i = 1; i < TAILLE_PLATEAU + 1; i++) {
        for (j = 1; j < TAILLE_PLATEAU + 1; j++) {
            // print bordure de la case
            MLV_draw_rectangle(x, y, taille_case, taille_case, MLV_COLOR_BLACK);
            // si la souris est sur la case, mise en évidence
            if (souris_x > x && souris_x < x + taille_case && souris_y > y &&
                souris_y < y + taille_case) {
                MLV_draw_filled_rectangle(
                    x, y, taille_case, taille_case,
                    MLV_convert_rgba_to_color(200, 200, 200, 117));
            }

            switch (plateau.plateau[i][j]) {

            case SYMBOL_1:
                MLV_draw_filled_circle(
                    x + (taille_case / 2), y + (taille_case / 2),
                    (taille_case - padding_case) / 2, MLV_COLOR_BLACK);
                break;
            case SYMBOL_2:
                MLV_draw_circle(x + (taille_case / 2), y + (taille_case / 2),
                                (taille_case - padding_case) / 2,
                                MLV_COLOR_BLACK);
                break;
            default:;
            }

            x += taille_case;
        }
        x = padding_f;
        y += taille_case;
    }
    MLV_actualise_window();
}

void affiche_gagnant(Plateau p, TYPE_I interface) {

    int taille_f = taille_fenetre();
    int padding_f = (taille_f * 5) / 100;
    char message[300];
    if (p.joueurs[0]->score == p.joueurs[1]->score) {
        sprintf(message, "Egalité %d à %d.", p.joueurs[0]->score,
                p.joueurs[1]->score);
    } else if (p.joueurs[0]->score > p.joueurs[1]->score) {
        sprintf(message, "Bravo %s, vous avez gagné %d à %d.", p.joueurs[0]->nom,
                p.joueurs[0]->score, p.joueurs[1]->score);
    } else {
        sprintf(message, "Bravo %s, vous avez gagné %d à %d.", p.joueurs[1]->nom,
                p.joueurs[1]->score, p.joueurs[0]->score);
    }
    if (interface == GUI) {

        MLV_draw_text_box(padding_f, (taille_f * 20) / 100,
                          taille_f - padding_f * 2, (taille_f * 20) / 100,
                          message, 2, MLV_COLOR_BLACK, MLV_COLOR_BLACK,
                          MLV_COLOR_WHITE, MLV_TEXT_LEFT, MLV_HORIZONTAL_CENTER,
                          MLV_VERTICAL_CENTER);
        MLV_actualise_window();

    } else {
        printf("%s", message);
        printf("\n");
    }
}

/************************** CLI ***********************************/
void affiche_plateau_cli(Plateau plateau) {
    int i, j;

    for (i = 0; i < TAILLE_PLATEAU + 2; i++) {
        for (j = 0; j < TAILLE_PLATEAU + 2; j++) {
            printf("%c", plateau.plateau[i][j]);
            printf(" ");
        }
        printf("\n");
    }
}

/************************** Jeu ***********************************/

int valeur_case(Plateau plateau, int pi, int pj, char symbol) {
    int nbr_adverses = 0;
    // vérifications des pions adjacents
    for (int i = pi - 1; i <= pi + 1; i++) {
        for (int j = pj - 1; j <= pj + 1; j++) {
            if (plateau.plateau[i][j] != VIDE &&
                plateau.plateau[i][j] != symbol &&
                plateau.plateau[i][j] != BORD) {
                nbr_adverses += 1;
            }
        }
    }

    return nbr_adverses;
}

int etat_partie(Plateau plateau, Joueur j_courant) {
    if (plateau.joueurs[0]->score == 0)
        return 0;

    if (plateau.joueurs[1]->score == 0)
        return 0;

    if (plateau.joueurs[0]->score + plateau.joueurs[1]->score ==
        TAILLE_PLATEAU * TAILLE_PLATEAU)
        return 0;

    // on regarde s'il existe au moins un pion adverse
    // adjacent à une case libre
    int case_ok = 0;
    int c = 0;

    for (int i = 1; i < TAILLE_PLATEAU + 1; i++) {
        for (int j = 1; j < TAILLE_PLATEAU + 1; j++) {
            if (plateau.plateau[i][j] == VIDE) {
                // vérifications des pions adjacents
                c = valeur_case(plateau, i, j, j_courant.symbol);
                case_ok += c;
            }
        }
    }

    return case_ok;
}

void jouer(Plateau *p, TYPE_I interface, MODE_J mode_jeu, int opts[MAX_OPTS]) {
    // indice du joueur courant
    int numj = 0;
    // pointeur sur le joueur courant;
    Joueur *joueur = p->joueurs[numj];
    // coordonées de la case
    int i = -1, j = -1;
    // sert à la détermination de la case la plus aventageuse pour l'ordi
    int max = 0, valcase = 0;
    // nombre de pions modifiés après placement d'un pion
    int pions_retournes = 0;
    // premier affichage du plateau

    int continuer = etat_partie(*p, *joueur);
    while (continuer) {

        if (mode_jeu == HO &&
            (numj)) { // si mode de jeu HvsO et le joueur est l'ordi

            // faudrait mettre ça dans une fonction
            for (int a = 1; a < TAILLE_PLATEAU + 1; a++) {
                for (int o = 1; o < TAILLE_PLATEAU + 1; o++) {
                    if (p->plateau[a][o] == VIDE) {
                        valcase = valeur_case(*p, a, o, joueur->symbol);
                        if (max < valcase) {
                            i = a;
                            j = o;
                            max = valcase;
                        }
                    }
                }
            }
            MLV_wait_milliseconds(
                250); // on temporise sinon on voit pas ce qu'il se passe
        } else {
            faire_jouer(*joueur, &i, &j, interface);
        }

        // si le joueur parviens à placer un pion
        if ((pions_retournes = ajouter_pion(p, i, j, joueur->symbol, opts)) !=
            0) {
            // reaffichage du plateau
            affiche_plateau(*p, *joueur, interface);
            // mise à jour des points
            joueur->score += pions_retournes + 1;
            // indice du joueur courant passe au suivant
            numj = (numj + 1) % 2;
            // mise à jour des points adverses
            p->joueurs[numj]->score -= pions_retournes;

            // affichage des scores
            if (interface == CLI) {
                printf("Score actuel : %s(%c) %d", p->joueurs[0]->nom,
                       p->joueurs[0]->symbol, p->joueurs[0]->score);
                printf(" - %s(%c) %d\n\n", p->joueurs[1]->nom,
                       p->joueurs[1]->symbol, p->joueurs[1]->score);
            }
            // remise à zero des indices de case
            i = -1, j = -1;
            max = 0;
            // changement de joueur
            joueur = p->joueurs[numj];
            // on check le plateau seulement après avoir joué
            // et changé de joueur
            // et pas dans la condition du while pour éviter de tester
            // chaque case toute les 50 ms (c'est beaucoup trop)
            continuer = etat_partie(*p, *joueur);
        }
        // nous permet de metre en évidence
        // la case survolée
        if (interface == GUI) {
            affiche_plateau(*p, *joueur, interface);
            MLV_wait_milliseconds(50);
        }
    }
    affiche_gagnant(*p, interface);
}

Joueur *make_joueur(char nom[TAILLE_MAX_NOM], char symbol) {
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

int InfectionAleatoire() {
    int proba = rand() % 11;

    if (proba > 6)
        return 0;
    else
        return 1;
}

int ajouter_pion(Plateau *plateau, int pi, int pj, char symbol,
                 int opts[MAX_OPTS]) {

    if (pi < 0 || pj < 0 || pi > TAILLE_PLATEAU || pj > TAILLE_PLATEAU ||
        plateau->plateau[pi][pj] != VIDE)
        return 0;

    int i, j, nbr_adverses = 0;
    // Changement de symbole des pions adjacents
    for (i = pi - 1; i <= pi + 1; i++) {
        for (j = pj - 1; j <= pj + 1; j++) {
            if (plateau->plateau[i][j] != VIDE &&
                plateau->plateau[i][j] != BORD &&
                plateau->plateau[i][j] != symbol) {
                if (opts[I]) {
                    if (InfectionAleatoire()) {
                        plateau->plateau[i][j] = symbol;
                    }
                } else {
                    plateau->plateau[i][j] = symbol;
                }
                nbr_adverses += 1;
            }
        }
    }
    if (nbr_adverses) {
        plateau->plateau[pi][pj] = symbol;
    }

    return nbr_adverses;
}

Plateau *creer_plateau(Joueur *joueur_1, Joueur *joueur_2) {
    Plateau *plateau;
    int i, j;
    if ((plateau = (Plateau *)malloc(sizeof(Plateau))) == NULL) {

        return NULL;
    }

    for (i = 0; i < TAILLE_PLATEAU + 2; i++) {
        for (j = 0; j < TAILLE_PLATEAU + 2; j++) {
            if (i < TAILLE_PLATEAU + 1 && i > 0 && j < TAILLE_PLATEAU + 1 &&
                j > 0) {
                plateau->plateau[i][j] = VIDE;
            } else {
                plateau->plateau[i][j] = BORD;
            }
        }
    }
    // ajout joueurs au plateau
    plateau->joueurs[0] = joueur_1;
    plateau->joueurs[1] = joueur_2;
    // position initiale des pions
    plateau->plateau[1][1] = joueur_2->symbol;
    plateau->plateau[TAILLE_PLATEAU][TAILLE_PLATEAU] = joueur_2->symbol;
    plateau->plateau[1][TAILLE_PLATEAU] = joueur_1->symbol;
    plateau->plateau[TAILLE_PLATEAU][1] = joueur_1->symbol;
    // mise à jour des points des joueurs
    plateau->joueurs[0]->score = 2;
    plateau->joueurs[1]->score = 2;
    return plateau;
}

int faire_jouer(Joueur joueur, int *i, int *j, TYPE_I interface) {

    if (interface == GUI) {
        if (MLV_get_mouse_button_state(MLV_BUTTON_LEFT) == MLV_PRESSED) {
            int sx, sy;
            MLV_get_mouse_position(&sx, &sy);
            return coordonees_vers_indices(sx, sy, i, j);
        }
        return 0;
    }
    printf("%s (%c), veuillez saisir les coordonnées où jouer (entre 1 et "
           "%d) : ",
           joueur.nom, joueur.symbol, TAILLE_PLATEAU);

    int r = scanf("%d %d", i, j);
    if (r != 2) {
        vider_buffer();
    }

    return r;
}

int init_jeu(Plateau **plateau, TYPE_I interface, MODE_J mode_jeu) {

    Joueur *joueur_1 = NULL;
    Joueur *joueur_2 = NULL;
    char nom[TAILLE_MAX_NOM];
    char message[200];

    if (interface == GUI) {
        init_fenetre();
    }

    switch (mode_jeu) {
    // Humain vs Humain
    case HH:
        // input des noms dans le terminal
        do {
            sprintf(message, "Quel est le nom du premier joueur (symbol %c) : ",
                    SYMBOL_1);
            lire_nom(nom, TAILLE_MAX_NOM, interface, message);
        } while ((joueur_1 = make_joueur(nom, SYMBOL_1)) == NULL);

        do {
            sprintf(message, "Quel est le nom du premier joueur (symbol %c) : ",
                    SYMBOL_2);
            lire_nom(nom, TAILLE_MAX_NOM, interface, message);
        } while ((joueur_2 = make_joueur(nom, SYMBOL_2)) == NULL);

        break;
    // Humain vs Ordi
    case HO:

        do {

            sprintf(message, "Quel est le nom du premier joueur (symbol %c) : ",
                    SYMBOL_1);
            lire_nom(nom, TAILLE_MAX_NOM, interface, message);
        } while ((joueur_1 = make_joueur(nom, SYMBOL_1)) == NULL);

        joueur_2 = make_joueur(NOM_ORDI, SYMBOL_2);

        break;
    default:
        printf("Erreur. Aucun mode de jeu défini.");
        return 0;
    }

    if ((*plateau = creer_plateau(joueur_1, joueur_2)) == NULL)
        return 0;

    if (interface == CLI) {
        printf("\n");
        affiche_plateau(*(*plateau), *(*plateau)->joueurs[0], interface);
        printf("\n");
    }

    return 1;
}

/************************** Utils ***********************************/

void vider_buffer() {

    while (getchar() != '\n')
        ;
}

int lire_nom(char *dest, int taille, TYPE_I interface, char *message) {
    char *cr = NULL;
    char *input = NULL;
    int taille_f = taille_fenetre();
    int padding_f = (taille_f * 5) / 100;

    if (interface == GUI) {

        MLV_wait_input_box(padding_f, (taille_f * 20) / 100,
                           taille_f - padding_f * 2, (taille_f * 20) / 100,
                           MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                           message, &input);

        // on limite le nombre de caractère
        for (int i = 0; *input && i < TAILLE_MAX_NOM + 2; i++) {
            dest[i] = input[i];
        }
    } else if (interface == CLI) {
        printf("%s", message);
        if (fgets(dest, taille, stdin)) {
            // on chercher le retour chariot
            if ((cr = strchr(dest, '\n'))) {
                *cr = '\0';

            } else { // sinon il reste des char dans le buffer
                vider_buffer();
            }
            return 1;
        }
        vider_buffer();
    }

    return 0;
}

void affiche_plateau(Plateau plateau, Joueur j_courant, TYPE_I interface) {
    switch (interface) {
    case CLI:
        affiche_plateau_cli(plateau);
        break;
    case GUI:
        affiche_plateau_gui(plateau, j_courant);
        break;
    default:
        printf("Interface non définie.");
        exit(1);
    }
}

int random_int(int min, int max) { return min + rand() % (max + 1 - min); }

void options(TYPE_I *interface, MODE_J *mode_jeu, int *opts, int argc,
             char *argv[]) {

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
                    *interface = CLI;
                    break;
                case 'g':
                    *interface = GUI;
                    break;
                case 'h':
                    *mode_jeu = HH;
                    break;
                case 'o':
                    *mode_jeu = HO;
                    break;
                case 'i':
                    opts[I] = 1;
                default:
                    continue;
                }
            }
        }
    }
}
