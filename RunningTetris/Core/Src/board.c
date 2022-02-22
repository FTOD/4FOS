/* board.cpp */
#include "../Inc/board.h"
#include "../Inc/os.h"

#define TAILLE_CASE  11
#define DECALAGE_GAUCHE 20
#define DECALAGE_HAUT 20

 
void flood(Board * board, int i, int j, int px, int py, int k, int o, int value, bool visited[][SIZE])
{
    if(px < 0 || px >= SIZE || py < 0 || py >= SIZE || visited[px][py] || PIECES[k][o][px][py] == FREE)
        return;
    visited[px][py] = true;

    board->area[j][i] = value;
    flood(board, i, j - 1, px, py - 1, k, o, value, visited);
    flood(board, i + 1, j, px + 1, py, k, o, value, visited);
    flood(board, i, j + 1, px, py + 1, k, o, value, visited);
    flood(board, i - 1, j, px - 1, py, k, o, value, visited);
}
 
void floodB(Board * board, int i, int j, int px, int py, int k, int o, bool * flag, bool visited[][SIZE])
{
    if(px < 0 || px >= SIZE || py < 0 || py >= SIZE || visited[px][py] || PIECES[k][o][px][py] == FREE)
        return;
 
    visited[px][py] = true;
 
    if(i < 0 || i >= BOARD_HEIGHT || j < 0 || j >= BOARD_WIDTH || board->area[j][i] != FREE )
    {
        *flag = false;
        return;
    }
 
    floodB(board, i, j - 1, px, py - 1, k, o, flag, visited);
    floodB(board, i + 1, j, px + 1, py, k, o, flag, visited);
    floodB(board, i, j + 1, px, py + 1, k, o, flag, visited);
    floodB(board, i - 1, j, px - 1, py, k, o, flag, visited);
}
 
void floodFill(Board * board, int i, int j, int px, int py, int k, int o, int value)
{
    bool visited[SIZE][SIZE];
 
    for(int l = 0; l < SIZE; ++l)
        for(int m = 0; m < SIZE; ++m)
            visited[l][m] = false;
    flood(board, i, j, px, py, k, o, value, visited);
}
 
void initBoard(Board * board)
{
    //board = malloc(sizeof(Board));
    //board->area = malloc(sizeof(int)*BOARD_HEIGHT*BOARD_WIDTH);
    for(int i = 0; i < BOARD_WIDTH; ++i)
        for(int j = 0; j < BOARD_HEIGHT; ++j)
            board->area[i][j] = FREE;
    
}
 
void drawPiece(Board * board, Piece * piece)
{
    int i = getPosX(piece);
    int j = getPosY(piece);
 
    int k = getKind(piece);
    int o = getOrientation(piece);
 
    switch(k)
    {
        case I:
            setColor(piece, CYA);
            break;
        case J:
            setColor(piece, BLU);
            break;
        case L:
            setColor(piece, ORANG);
            break;
        case O:
            setColor(piece, YELLO);
            break;
        case S:
            setColor(piece, GREE);
            break;
        case T:
            setColor(piece, PURPL);
            break;
        case Z:
            setColor(piece, RE);
            break;
        default:
            break;
    }
    floodFill(board, i, j, PIVOT_X, PIVOT_Y, k, o, getColor(piece));
}
 
void clearPiece(Board * board, Piece * piece)
{
    int i = getPosX(piece);
    int j = getPosY(piece);
 
    int k = getKind(piece);
    int o = getOrientation(piece);
 
    floodFill(board, i, j, PIVOT_X, PIVOT_Y, k, o, FREE);
}
 
void newPiece(Board * board, Piece * piece)
{
        
    setPosX(piece, ORIGIN_X);
    setPosY(piece, ORIGIN_Y);
 
    drawPiece(board, piece);
    setCurrentPiece(board, piece);
}

void setCurrentPiece(Board * board, Piece * piece)
{
    board->currentPiece = piece;
}
 
Piece * getCurrentPiece(Board * board)
{
    return board->currentPiece;
}
 
bool isCurrentPieceMovable(Board * board, int x, int y)
{
    clearPiece(board, board->currentPiece);
 
    bool movable = true;
 
    bool visited[SIZE][SIZE];
 
    for(int l = 0; l < SIZE; ++l)
        for(int m = 0; m < SIZE; ++m)
            visited[l][m] = false;
 
    int k = getKind(board->currentPiece);
    int o = getOrientation(board->currentPiece);
 
    floodB(board, x, y, PIVOT_X, PIVOT_Y, k, o, &movable, visited);
 
    drawPiece(board, board->currentPiece);
 
    return movable;
}
 
bool isCurrentPieceRotable(Board * board, int o)
{
    clearPiece(board, board->currentPiece);
 
    bool rotable = true;
 
    bool visited[SIZE][SIZE];
 
    for(int i = 0; i < SIZE; ++i)
        for(int j = 0; j < SIZE; ++j)
            visited[i][j] = false;
 
    int k = getKind(board->currentPiece);
 
    floodB(board, getPosX(board->currentPiece), getPosY(board->currentPiece), PIVOT_X, PIVOT_Y, k, o, &rotable, visited);
 
    drawPiece(board, board->currentPiece);
 
    return rotable;
}
 
void moveCurrentPieceDown(Board * board)
{
    int x = getPosX(board->currentPiece);
    int y = getPosY(board->currentPiece);
 
    if(isCurrentPieceMovable(board, x + 1, y))
    {
        clearPiece(board, board->currentPiece);
        setPosX(board->currentPiece, x + 1);
 
        drawPiece(board, board->currentPiece);
    }
}

int moveCurrentPieceDownBot(Board * board)
{
    if(isCurrentPieceFallen(board)){
        return 0;
    }
    
    moveCurrentPieceDown(board);
    return 1;
}
 
void moveCurrentPieceLeft(Board * board)
{
    int x = getPosX(board->currentPiece);
    int y = getPosY(board->currentPiece);
 
    if(isCurrentPieceMovable(board, x, y - 1))
    {
        clearPiece(board, board->currentPiece);
        setPosY(board->currentPiece, y - 1);
 
        drawPiece(board, board->currentPiece);
    }
}
 
void moveCurrentPieceRight(Board * board)
{
    int x = getPosX(board->currentPiece);
    int y = getPosY(board->currentPiece);
 
    if(isCurrentPieceMovable(board, x, y + 1))
    {
        clearPiece(board, board->currentPiece);
        setPosY(board->currentPiece, y + 1);
 
        drawPiece(board, board->currentPiece);
    }
}
 
void rotateCurrentPieceLeft(Board * board)
{
    int o = getOrientation(board->currentPiece);
 
    if(o > 0)
        o--;
    else
        o = NB_ROTATIONS - 1;
 
    if(isCurrentPieceRotable(board, o))
    {
        clearPiece(board, board->currentPiece);
 
        setOrientation(board->currentPiece, o);
        drawPiece(board, board->currentPiece);
    }
}
 
void rotateCurrentPieceRight(Board * board)
{
    int o = getOrientation(board->currentPiece);
 
    if(o < NB_ROTATIONS - 1)
        o++;
    else
        o = 0;
 
    if(isCurrentPieceRotable(board, o))
    {
        clearPiece(board, board->currentPiece);
        setOrientation(board->currentPiece, o);
 
        drawPiece(board, board->currentPiece);
    }
}
 
void deleteLine(Board * board, int y)
{
    clearPiece(board, board->currentPiece);
 
    for(int j = y; j > 0; --j)
    {
        for(int i = 0; i < BOARD_WIDTH; ++i)
            board->area[i][j] = board->area[i][j-1];
    }
 
    drawPiece(board, board->currentPiece);
}
 
int deletePossibleLines(Board * board)
{
    int nbLinesDeleted = 0;
 
    for(int j = 0; j < BOARD_HEIGHT; ++j)
    {
        int i = 0;
 
        for(; i < BOARD_WIDTH && board->area[i][j] != FREE; ++i);
 
        if(i == BOARD_WIDTH)
        {
            nbLinesDeleted++;
            deleteLine(board, j);
        }
    }
 
    return nbLinesDeleted;
}
 
void dropCurrentPiece(Board * board)
{
    int x = getPosX(board->currentPiece);
    int y = getPosY(board->currentPiece);
 
    while(isCurrentPieceMovable(board, x++, y))
        moveCurrentPieceDown(board);
}
 
bool isCurrentPieceFallen(Board * board)
{
    int x = getPosX(board->currentPiece);
    int y = getPosY(board->currentPiece);
 
    if(isCurrentPieceMovable(board, x + 1, y))
        return false;
 
    return true;
}
 
int isGameOver(Board * board)
{
    for(int i = 0; i < BOARD_WIDTH; ++i)
    {
        if(board->area[i][0] != FREE)
            return 1;
    }
 
    return 0;
}

void print(Board * board)
{   
    static int previousArea[BOARD_WIDTH][BOARD_HEIGHT] = {{FREE}};
    uint16_t color;
    for(int j = 0; j < BOARD_HEIGHT; ++j)
    {
        for(int i = 0; i < BOARD_WIDTH; ++i)
        {
            if( board->area[i][j] != previousArea[i][j]){
                switch(board->area[i][j]){
                    case FREE:
                        color = BLACK;
                        break;
                    case CYA:
                        color = CYAN;
                        break;
                    case ORANG:
                        color = ORANGE;
                        break;
                    case RE:
                        color = RED;
                        break;
                    case PURPL:
                        color = PURPLE;
                        break;
                    case GREE:
                        color = GREEN;
                        break;
                    case YELLO:
                        color = YELLOW;
                        break;
                    case BLU:
                        color = BLUE;
                        break;
                    default:
                        color = WHITE;
                        break;
                }
                fillRect(TFTWIDTH -1 -DECALAGE_HAUT -(j*TAILLE_CASE), i*TAILLE_CASE + DECALAGE_GAUCHE, TAILLE_CASE,TAILLE_CASE,color);
            }
            previousArea[i][j] = board->area[i][j];
        } 
    }
    
}

void printBorders(){
    fillRect(TFTWIDTH-DECALAGE_HAUT-((BOARD_HEIGHT-1)*TAILLE_CASE)-3,DECALAGE_GAUCHE-2,TAILLE_CASE*BOARD_HEIGHT+4,TAILLE_CASE*BOARD_WIDTH+4,WHITE);
    fillRect(TFTWIDTH-DECALAGE_HAUT-((BOARD_HEIGHT-1)*TAILLE_CASE)-1,DECALAGE_GAUCHE,TAILLE_CASE*BOARD_HEIGHT,TAILLE_CASE*BOARD_WIDTH,BLACK);

    fillRect(TFTWIDTH - 80, BOARD_WIDTH*TAILLE_CASE  + DECALAGE_GAUCHE + 15, 72, 72,WHITE);
    fillRect(TFTWIDTH - 78, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 17 , 68, 68,BLACK);
}

void printNextPiece(int id){
    fillRect(TFTWIDTH - 78, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 17 , 68, 68,BLACK);
    switch(id){
        case I:
            fillRect(TFTWIDTH - 80 + 30, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 12, 12, 48, CYAN);
            break;
        case L:
            fillRect(TFTWIDTH - 80 + 36, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 18, 12, 36, ORANGE);
            fillRect(TFTWIDTH - 80 + 24, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 18, 12, 12, ORANGE);
            break;
        case Z:
            fillRect(TFTWIDTH - 80 + 36, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 18, 12, 24, RED);
            fillRect(TFTWIDTH - 80 + 24, BOARD_WIDTH*TAILLE_CASE  + DECALAGE_GAUCHE + 15 + 30, 12, 24, RED);
            break;
        case T:
            fillRect(TFTWIDTH - 80 + 36, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 18, 12, 36 ,PURPLE);
            fillRect(TFTWIDTH - 80 + 24, BOARD_WIDTH*TAILLE_CASE  + DECALAGE_GAUCHE + 15 + 30, 12, 12 ,PURPLE);
            break;
        case S:
            fillRect(TFTWIDTH - 80 + 24, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 18, 12, 24, GREEN);
            fillRect(TFTWIDTH - 80 + 36, BOARD_WIDTH*TAILLE_CASE  + DECALAGE_GAUCHE + 15 + 30, 12, 24, GREEN);
            break;
        case O:
            fillRect(TFTWIDTH - 80 + 24, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 24, 24, 24, YELLOW);
            break;
        case J:
            fillRect(TFTWIDTH - 80 +  36, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 18, 12, 36, BLUE);
            fillRect(TFTWIDTH - 80 +  24, BOARD_WIDTH*TAILLE_CASE + DECALAGE_GAUCHE + 15 + 42, 12, 12, BLUE);
            break;
        default:
            break;
    }
}


void clear(Board * board)
{
    for(int i = 0; i < BOARD_WIDTH; ++i)
    {
        for(int j = 0; j < BOARD_HEIGHT; ++j)
            board->area[i][j] = FREE;
    }
}