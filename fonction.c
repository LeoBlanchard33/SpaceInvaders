/*
 * fonction.c
 *
 *  Created on: 22 janv. 2020
 *      Author: leob3
 */

#include "vt100.h"
#include "serial.h"
#include "fonction.h"

typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char mort;
} monstre_t;
monstre_t enemis[16];

typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char etat;
} tir_t;
tir_t tir[1];
tir_t tir_enemis[8];

static unsigned char vx = 43;
static unsigned char vy = 26;
unsigned char fin = 0;
unsigned char score[16];
unsigned char s = 0;

//Contour du jeu
void fenetre(unsigned char character1, unsigned char character2) {

	unsigned char i;

	for (i = 2; i < 90; i++) {
		vt100_move(i, 1);
		serial_putchar(character1);
		vt100_move(i, 29);
		serial_putchar(character1);
	}
	for (i = 2; i < 30; i++) {
		vt100_move(1, i);
		serial_putchar(character2);
		vt100_move(90, i);
		serial_putchar(character2);
	}
}

//Premier affichage
void menu_accueil(void) {
	fenetre('_', '|');
	vt100_move(38, 10);
	serial_puts("Space Invader");
	vt100_move(43, 15);
	serial_puts("PLAY");
	vt100_move(70, 28);
	serial_puts("By Leo Blanchard");
	vt100_move(34, 20);
	serial_puts("Appuyer sur une touche");
	appuye_touche();
	recap_regle();
	init();
	vt100_clear_screen();
	fenetre('_', '|');
}

//Detection d'un appui de touche du clavier
void appuye_touche(void) {
	signed char push = -1;
	while (push == -1) {
		push = serial_get_last_char();
	}
}

//Deuxieme affichage
void recap_regle(void) {
	vt100_clear_screen();
	fenetre('_', '|');
	vt100_move(5, 4);
	serial_puts("Vous contrôlez un vaisseau pouvant tirer");
	vt100_move(5, 6);
	serial_puts("Les ennemis descendent dans la zone de jeu");
	vt100_move(5, 8);
	serial_puts("Tuer tout les ennemis avant qu'ils atteignent votre vaisseau");
	vt100_move(5, 14);
	serial_puts("Fonctionnement :");
	vt100_move(5, 16);
	serial_puts("Q : Deplacement à gauche et D : Deplacment à droite");
	vt100_move(5, 18);
	serial_puts(
			"Z : Tirer (Attendre que le tir ce finisse avant de tirer à nouveau)");
	vt100_move(65, 28);
	serial_puts("Appuyer sur une touche");
	appuye_touche();
}

//Initialisation
void init(void) {
	for (int i = 0; i <= 15; i++) {
		enemis[i].mort = 0;
		score[i] = 0;
	}
	fin = 0;
	tir[0].x = 15;
	tir[0].y = 0;
	for (int i = 0; i <= 7; i++) {
		tir_enemis[i].y = 0;
	}
}

//Calcul des positions des 16 ennemis
void position_enemis(unsigned char k, unsigned char hauteur_L1,
		unsigned char hauteur_L2) {

	for (int j = 0; j <= 7; j++) {
		enemis[j].x = k;
		enemis[j].y = hauteur_L1;
		enemis[j + 8].x = k;
		enemis[j + 8].y = hauteur_L2;
		k = k + 8;
	}
}

//Affiche les 16 ennemis
void affiche_enemis(void) {
	for (int i = 0; i <= 15; i++) {
		if (enemis[i].mort != 1) {
			vt100_move(enemis[i].x, enemis[i].y);
			serial_puts(" ~O~ ");
			vt100_move(enemis[i].x, enemis[i].y - 1);
			serial_puts("     ");
		}
	}
}

//Initialise le tir si la touche est presser
void affiche_tir(signed char feu) {
	if (feu == 'z' || feu == 'Z') {
		tir[0].x = vx + 3;
		tir[0].y = 25;
	}
}

//Initialise les tirs ennemis si la touche est presser
void affiche_tir_enemis(signed char feu) {
	if (feu == 'z' || feu == 'Z') {
		for (int i = 0; i <= 7; i++) {
			tir_enemis[i].y = enemis[i + 8].y + 2;
			tir_enemis[i].x = enemis[i + 8].x + 1;
		}
	}
}

//Lancement du tir
void tir_go(void) {
	vt100_move(tir[0].x, tir[0].y);
	serial_puts("|");
	vt100_move(tir[0].x, tir[0].y + 1);
	serial_puts(" ");
}

//Lancement des tirs ennemis
void tir_enemis_go(void) {
	for (int i = 0; i <= 7; i++) {
		if (tir_enemis[i].y != 0) {
			vt100_move(tir_enemis[i].x, tir_enemis[i].y);
			serial_puts("|");
			vt100_move(tir_enemis[i].x, tir_enemis[i].y - 1);
			serial_puts(" ");
		}
	}
}

//Affichage et deplacement du vaisseau
void deplacement_vaisseau(signed char depla) {
	vt100_move(vx, vy);
	serial_puts(" ('0') ");

	if (vx < 84) {
		if (depla == 'd' || depla == 'D') {
			vx = vx + 1;
			vt100_move(vx, vy);
			serial_puts(" ('0') ");
		}
	}
	if (vx > 1) {
		if (depla == 'q' || depla == 'Q') {
			vx = vx - 1;
			vt100_move(vx, vy);
			serial_puts(" ('0') ");
		}
	}
}

//Calcul du deplacement des ennemis
float k = 4;
double hauteur_L1 = 5;
double hauteur_L2 = 9;
float direction = 0.2;
void mouvement_enemis(void) {
	k = k + direction;
	if (k > 28) {
		direction = -0.2;
	}
	if (k < 4) {
		direction = 0.2;
	}
	if (hauteur_L2 < 24) {
		hauteur_L1 = hauteur_L1 + 0.1;
		hauteur_L2 = hauteur_L2 + 0.1;
	}
	if (hauteur_L2 > 23) {
		fin = 1;
	}
	position_enemis(k, hauteur_L1, hauteur_L2);
}

//Test si un ennemis est touché par un tir
void hit_box(void) {
	for (int i = 0; i <= 15; i++) {
		if ((enemis[i].x + 1 == tir[0].x || enemis[i].x + 2 == tir[0].x
				|| enemis[i].x + 3 == tir[0].x) && (enemis[i].y == tir[0].y)) {
			enemis[i].mort = 1;
			vt100_move(enemis[i].x, enemis[i].y);
			serial_puts("     ");
			tir[0].y = 0;
		}
	}
}

//Calcul du deplacement du tir
void mouvement_tir(void) {
	if (tir[0].y >= 3) {
		tir[0].y = tir[0].y - 1;
		tir_go();
	}
	vt100_move(tir[0].x, 2);
	serial_puts(" ");
}

//Calcul des deplacements des tirs ennemis
void mouvement_tir_enemis(void) {
	for (int i = 0; i <= 7; i++) {
		if (enemis[i + 8].y <= tir_enemis[i].y) {
			tir_enemis[i].y = tir_enemis[i].y + 1;
			tir_enemis_go();
		}
		vt100_move(tir_enemis[i].x, 28);
		serial_puts(" ");
		if (tir_enemis[i].y >= 28) {
			tir_enemis[i].y = 0;
		}
	}
}

//Test de fin de jeu
unsigned char condition_fin(void) {
	if (fin == 1) {
		interface_fin_lose();
		return 1;
	}
	if (fin == 2) {
		interface_fin_win();
		return 1;
	}
	return 0;
}

//Affichage de fin du jeu
void interface_fin_lose(void) {
	vt100_clear_screen();
	fenetre('_', '|');
	vt100_move(38, 4);
	serial_puts("Space Invader");
	vt100_move(10, 8);
	serial_puts("Dommage");
	vt100_move(10, 12);
	serial_puts("Vous avez perdu :(");
	vt100_move(10, 23);
	serial_puts("Appuyer sur une touche pour relancer le jeu");
	vt100_move(70, 28);
	serial_puts("By Leo Blanchard");
	appuye_touche();
}
void interface_fin_win(void) {
	vt100_clear_screen();
	fenetre('_', '|');
	vt100_move(38, 4);
	serial_puts("Space Invader");
	vt100_move(10, 8);
	serial_puts("Bravo");
	vt100_move(10, 12);
	serial_puts("Vous avez vaincu tous les ennemis :)");
	vt100_move(10, 23);
	serial_puts("Appuyer sur une touche pour relancer le jeu");
	vt100_move(70, 28);
	serial_puts("By Leo Blanchard");
	appuye_touche();
}

//Affichage du nombre d'ennemis tuer
void c_score(void) {
	for (int i = 0; i <= 15; i++) {
		if (enemis[i].mort == 1) {
			score[i] = 1;
			s = score[0] + score[1] + score[2] + score[3] + score[4] + score[5]
					+ score[6] + score[7] + score[8] + score[9] + score[10]
					+ score[11] + score[12] + score[13] + score[14] + score[15];
		}
		if (s == 1) {
			vt100_move(17, 3);
			serial_puts("1");
		}
		if (s == 2) {
			vt100_move(17, 3);
			serial_puts("2");
		}
		if (s == 3) {
			vt100_move(17, 3);
			serial_puts("3");
		}
		if (s == 4) {
			vt100_move(17, 3);
			serial_puts("4");
		}
		if (s == 5) {
			vt100_move(17, 3);
			serial_puts("5");
		}
		if (s == 6) {
			vt100_move(17, 3);
			serial_puts("6");
		}
		if (s == 7) {
			vt100_move(17, 3);
			serial_puts("7");
		}
		if (s == 8) {
			vt100_move(17, 3);
			serial_puts("8");
		}
		if (s == 9) {
			vt100_move(17, 3);
			serial_puts("9");
		}
		if (s == 10) {
			vt100_move(17, 3);
			serial_puts("10");
		}
		if (s == 11) {
			vt100_move(17, 3);
			serial_puts("11");
		}
		if (s == 12) {
			vt100_move(17, 3);
			serial_puts("12");
		}
		if (s == 13) {
			vt100_move(17, 3);
			serial_puts("13");
		}
		if (s == 14) {
			vt100_move(17, 3);
			serial_puts("14");
		}
		if (s == 15) {
			vt100_move(17, 3);
			serial_puts("15");
		}
		if (s == 16) {
			vt100_move(17, 3);
			serial_puts("16");
			fin = 2;
		}
		vt100_move(3, 3);
		serial_puts("Ennemis tuer :");
	}
}

//Appel des fonctions
void jeu(void) {
	signed char touche = serial_get_last_char();
	mouvement_enemis();
	mouvement_tir();
	mouvement_tir_enemis();
	affiche_tir(touche);
	affiche_tir_enemis(touche);
	hit_box();
	affiche_enemis();
	c_score();
	deplacement_vaisseau(touche);
	condition_fin();
}
