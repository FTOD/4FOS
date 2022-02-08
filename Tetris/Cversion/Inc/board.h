#ifndef BOARD_H
#define BOARD_H
 
#include "piece.h"
 
enum  { FREE, FILLED };
 
#define BOARD_HEIGHT 20
#define BOARD_WIDTH  10
 
#define ORIGIN_X     0
#define ORIGIN_Y     5

typedef struct{
    Piece * currentPiece;
    int area[BOARD_WIDTH][BOARD_HEIGHT];
}Board;


    
 
void flood(Board * board, int i, int j, int px, int py, int k, int o, int value, bool visited[][SIZE]);
void floodB(Board * board, int i, int j, int px, int py, int k, int o, bool * flag, bool visited[][SIZE]);
void floodFill(Board * board, int i, int j, int px, int py, int k, int o, int value);

void initBoard(Board * board);

void setCurrentPiece(Board * board, Piece * p);
Piece * getCurrentPiece(Board * board);

void drawPiece(Board * board, Piece * p);
void clearPiece(Board * board, Piece * p);

void newPiece(Board * board, Piece * p);

bool isCurrentPieceMovable(Board * board, int x, int y);
bool isCurrentPieceRotable(Board * board, int o);

void moveCurrentPieceDown(Board * board);
void moveCurrentPieceLeft(Board * board);
void moveCurrentPieceRight(Board * board);

void rotateCurrentPieceLeft(Board * board);
void rotateCurrentPieceRight(Board * board);

void deleteLine(Board * board, int y);
int deletePossibleLines(Board * board);

void dropCurrentPiece(Board * board);
bool isCurrentPieceFallen(Board * board);

bool isGameOver(Board * board);

void print(Board * board);
void clear(Board * board);

 
#endif