#include "game.h"

#define SYNC_TIME     5000 /* duration of a loop in uS */
#define INITIAL_RYTHM 200  /* number of loop between 2 forced down moves */
#define SPEEDUP_RYTHM 2000 /* number of loop between 2 speedups */
#define SPEEDUP       10   /* value of the speedup */

Board board;
Piece piece;

int movingTimer = 0;
int start       = 0;
int gameOver  = 0;

/* Buttons functions */
void UP(void){
    dropCurrentPiece(&board);
}

void DOWN(void){  
    moveCurrentPieceDown(&board);
    movingTimer = 0;
}

void RIGHT(void){
    moveCurrentPieceRight(&board);
}

void LEFT(void){
    moveCurrentPieceLeft(&board);
}

void A(void){    
    rotateCurrentPieceRight(&board);
}

void B(void){
    rotateCurrentPieceLeft(&board);
}

void START(void){
    start = !start;
}

void displayScore(int score){
    char charScore[6]; 
    sprintf(charScore, "%d", score);
    DrawText(charScore, 120, 150, WHITE, 2, BLACK);
}

void setButtonsInGame(){
    setAfunction(&A);
    setBfunction(&B);
    setUPfunction(&UP);
    setDOWNfunction(&DOWN);
    setRIGHTfunction(&RIGHT);
    setLEFTfunction(&LEFT);
    setSTARTfunction(&START);
}

void setButtonsOutGame(){
    setAfunction(&emptyFunc);
    setBfunction(&emptyFunc);
    setUPfunction(&emptyFunc);
    setDOWNfunction(&emptyFunc);
    setRIGHTfunction(&emptyFunc);
    setLEFTfunction(&emptyFunc);
    setSTARTfunction(&START);
}

int game(){ 
    setButtonsInGame();   
    /* Seeding RNG */
    srand(LL_TIM_GetCounter(TIM4));


        /* VARIABLES */
    /* for the display */
    char charScore[6];
    char charNbDeletedLines[3];
    int timeDisplay  = 0;
    int startDisplay = 0;
    /* for the game */
    int score          = 0;
    int nbDeletedLines = 0;
    int nextPiece      = rand()%7;      // next tetromino
    int rythm          = INITIAL_RYTHM; // falling rythm of the tetromino
    int speedUpCounter = 0;             // counting to know when reduce the rythm
    
    /* game area display */
    fillScreen(BLACK);
    printBorders();

    /* score display */
    DrawText("Score: ", 140, 150, WHITE, 2, BLACK);
    displayScore(score);

    /* initializing the game */
    initBoard(&board);

    /* starting the game */
    initPiece(&piece, rand()%7, 0);
    newPiece(&board, &piece);
    printNextPiece(nextPiece);

    gameOver = 0;
    while(!gameOver){/* game loop */
        //remove
        LL_TIM_DisableCounter(TIM4);
        LL_TIM_SetCounter(TIM4,0);
        LL_TIM_EnableCounter(TIM4);

        print(&board);   
    
        if (movingTimer >= rythm){/* when the game is moving down the tetromino */
        movingTimer = 0;
        
            if(!moveCurrentPieceDownBot(&board)){/* if the tetromino is down down */
                gameOver = isGameOver(&board);

                /* Creating a new tetromino */
                initPiece(&piece, nextPiece, 0);
                nextPiece = rand()%7;
                newPiece(&board, &piece);

                /* Deleting lines and displaying the move */
                nbDeletedLines = deletePossibleLines(&board);
                score += pow(nbDeletedLines, 2) * 100;
                if( nbDeletedLines > 0){
                    startDisplay = 1;
                    if(nbDeletedLines == 4){
                        DrawText("TETRIS!", 80, 150, YELLOW, 2, BLACK);  
                    }
                    else{
                        sprintf(charNbDeletedLines, "%d", nbDeletedLines);
                        DrawText(charNbDeletedLines, 80, 140, WHITE, 2, BLACK);
                        DrawText("LINE!", 80, 160, WHITE, 2, BLACK);
                    }
                }
                
                displayScore(score);
                printNextPiece(nextPiece);
            }
        }

        

        movingTimer++;
        speedUpCounter++;

        /* Displaying the move (when the player deleted a line) */
        if( startDisplay ){
            timeDisplay++;
        }
        if( timeDisplay >= 300 ){
            fillRect(70, 140, 20, 90, BLACK);
            timeDisplay = 0;
            startDisplay = 0;
        }

        /* Speeding up if it's time to */
        if(speedUpCounter >= SPEEDUP_RYTHM){
            speedUpCounter = 0;
            if(rythm > 50){
                rythm -= SPEEDUP;
            }
        }

        SystemCheckUp();  
        sync();
    }

    /* Displaying game over */
    fillScreen(BLACK);
    DrawText("GAME OVER",170,50,RED,4,BLACK);
    DrawText("YOUR SCORE : ",70,50,WHITE,2,BLACK);
    sprintf(charScore,"%d",score);
    DrawText(charScore,50,50,WHITE,2,BLACK);

    start = 0;
    gameOver = 1;

    setButtonsOutGame();   

    return score;
}


int main(void){
    
    /* Set up */
    initSystem();
    setSync(SYNC_TIME);
    setButtonsOutGame();
    SetRotation(1);
    /* VARIABLES */
    int score = 0;
    char charScore[6];

    fillScreen(BLACK);

    while(1){/* main loop */
        if(gameOver == 0){/* if it's the first game */
            DrawText("Press start to play!",140,20,WHITE,2,BLACK);
        }
        else{/* if it's a replay */
            DrawText("Press start to replay",120,10,WHITE,2,BLACK);
        }

        /* Waiting for the player to press START */
        while(!start){
            SystemCheckUp();
        }

        /* Running 1 game */
        score = game();

        /* Saving and displaying the best score */
        updateHighScore(score);
        sprintf(charScore, "%d", getHighScore());
        DrawText("BEST SCORE : ", 30, 50, WHITE, 2, BLACK);
        DrawText(charScore, 10, 50, WHITE, 2, BLACK);   
    }

    return 0;
}