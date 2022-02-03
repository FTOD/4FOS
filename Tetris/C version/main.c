#include "board.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"

int main(void){
    srand(time(NULL));
    
    Board board;
    
    initBoard(&board);
    
    Piece piece;
    initPiece(&piece, rand()%6, 0);
    
    newPiece(&board, &piece);
    while(1){
        //Affichage
        system("clear");
        print(&board);
        
        
        if(isCurrentPieceFallen(&board)){
            deletePossibleLines(&board);
            initPiece(&piece, rand()%6, 0);
            newPiece(&board, &piece);
        }
        
        char input;
        scanf("%c", &input);
        switch(input){
            case 'q': moveCurrentPieceLeft(&board);
                    break;
            case 'd': moveCurrentPieceRight(&board);
                    break;
            case 's': moveCurrentPieceDown(&board);
                    break;
            case 'w': rotateCurrentPieceLeft(&board);
                    break;
            case 'c': rotateCurrentPieceRight(&board);
                    break;
        }
    }
    return 0;
}