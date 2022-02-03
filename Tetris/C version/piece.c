/* piece.cpp */
#include "piece.h"
 

 
void initPiece(Piece * piece, int k, int o)
{
    //piece = malloc(sizeof(Piece));
    piece->kind = k;
    piece->orientation = o;
}
 

 
void setKind(Piece * piece, int k)
{
    piece->kind = k;
}
 
void setOrientation(Piece * piece, int o)
{
    piece->orientation = o;
}
 
int getKind(Piece * piece)
{
    return piece->kind;
}
 
int getOrientation(Piece * piece)
{
    return piece->orientation;
}
 
void setColor(Piece * piece, int c)
{
    piece->color = c;
}
 
int getColor(Piece * piece)
{
    return piece->color;
}
 
void setPosX(Piece * piece, int x)
{
    piece->posX = x;
}
 
int getPosX(Piece * piece)
{
    return piece->posX;
}
 
void setPosY(Piece * piece, int y)
{
    piece->posY = y;
}
 
int getPosY(Piece * piece)
{
    return piece->posY;
}