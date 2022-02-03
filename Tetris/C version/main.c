#include "board.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

int main(void){
    srand(time(NULL));
    
    Board * board;
    
    initBoard(board);
    
    fprintf(stderr, "cac");
    Piece * piece = creerPiece(rand()%6, 0);
    
    fprintf(stderr, "YO1"); 
    newPiece(board, piece);
    fprintf(stderr, "YO2");
    while(1){
        //Affichage
        system("clear");
        print(board);
        
        deletePossibleLines(board);
        if(isCurrentPieceFallen(board)){
            //free(board->currentPiece);
            //piece = creerPiece(rand()%6, 0);
            //newPiece(board, piece);
        }
        
        char input;
        scanf("%c", &input);
        switch(input){
            case 'q': moveCurrentPieceLeft(board);
                    break;
            case 'd': moveCurrentPieceRight(board);
                    break;
            case 's': moveCurrentPieceDown(board);
                    break;
            case 'w': rotateCurrentPieceLeft(board);
                    break;
            case 'c': rotateCurrentPieceRight(board);
                    break;
        }
    }
    return 0;
}