/*
 * fonction.h
 *
 *  Created on: 22 janv. 2020
 *      Author: leob3
 */

#ifndef FONCTION_H_
#define FONCTION_H_

void fenetre(unsigned char character1, unsigned char character2);
void menu_accueil(void);
void appuye_touche(void);
void recap_regle(void);
void interface(unsigned char x, unsigned char y);
void deplacement_vaisseau(signed char depla);
void jeu(void);
void affiche_enemis(void);
void position_enemis(unsigned char k,unsigned char hauteur_L1,unsigned char hauteur_L2);
void mouvement_enemis(void);
void mouvement_tir(void);
void init(void);
void interface_fin_lose(void);
void interface_fin_win(void);
unsigned char condition_fin(void);

#endif /* FONCTION_H_ */
