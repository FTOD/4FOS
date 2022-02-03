/* piece.h */
#ifndef PIECE_H
#define PIECE_H
 
#include "shapes.h"
#include "stdio.h"
#include "stdlib.h"

typedef int bool;
#define true  1
#define false 0
 
#define PIVOT_X  1
#define PIVOT_Y  2

enum { CYAN = 1, BLUE, ORANGE, YELLOW, GREEN, PURPLE, RED, GHOST }; // Les couleurs de chaque pièce, GHOST est pour la pièce fantôme
enum { O = 0, I, S, Z, L, J, T};
/* Les coordonnées du point de pivot de la pièce */


typedef struct{
    int kind;        // Le type de la pièce
    int orientation; // Son orientation (sa rotation courante)
    int color;       // Sa couleur
 
    int posX;        // Son ordonnée dans l'aire de jeu
    int posY;        // Son abscisse dans l'aire de jeu
}Piece;

 
   
 
Piece* creerPiece(int k, int o);

void setKind(Piece * piece, int k);
void setOrientation(Piece * piece, int o);

int getKind(Piece * piece);
int getOrientation(Piece * piece);

void setColor(Piece * piece, int c);
int getColor(Piece * piece);

void setPosX(Piece * piece, int x);
int getPosX(Piece * piece);

void setPosY(Piece * piece, int y);
int getPosY(Piece * piece);

 
#endif