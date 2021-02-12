#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum playertypes{WHITE, BLACK};
enum availibility{OCCUPIED, EMPTY, ATTACKABLE};

char chess_board[8][8][3] =
    {
        {{'B', 'R', '0'}, {'B', 'N', '0'}, {'B', 'B', '0'}, {'B', 'Q', '0'}, {'B', 'K', '0'}, {'B', 'B', '0'}, {'B', 'N', '0'}, {'B', 'R', '0'}},
        {{'B', 'P', '0'}, {'B', 'P', '0'}, {'B', 'P', '0'}, {'B', 'P', '0'}, {'B', 'P', '0'}, {'B', 'P', '0'}, {'B', 'P', '0'}, {'B', 'P', '0'}},
        {{'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}},
        {{'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}},
        {{'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}},
        {{'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}, {'_', '_', '0'}},
        {{'W', 'P', '0'}, {'W', 'P', '0'}, {'W', 'P', '0'}, {'W', 'P', '0'}, {'W', 'P', '0'}, {'W', 'P', '0'}, {'W', 'P', '0'}, {'W', 'P', '0'}},
        {{'W', 'R', '0'}, {'W', 'N', '0'}, {'W', 'B', '0'}, {'W', 'Q', '0'}, {'W', 'K', '0'}, {'W', 'B', '0'}, {'W', 'N', '0'}, {'W', 'R', '0'}}
    };

int positionX, positionY;
int tempPositionX, tempPositionY;
int capturerX, capturerY;
int capturedX, capturedY;
int x, y, i, n, m = 1;
int player;
int castlingType;
int move, promotionSelection;
int availibleMoves[100];
int turn, time;

int of, tf, ul, ur, ob, tb, ll, lr;

char cplayer, cenemy;
char capturedPiece[2], capturerPiece[2];
char square[1024];
char buff[1024];

void display();
void play();
void pieceMove();
void preparations();
int endGame();

void capturing();
void promotion();

void resetAvailibleMoves();
void resetPawnMoves();
void resetCapturing();

int castling();
int canPromote();

void kingSideCastle();
void queenSideCastle();

void getPawnMoves();
void getRookMoves();
void getKnightMoves();
void getBishopMoves();
void getQueenMoves();
void getKingMoves();

int main()
{
    printf("Welcome to Terminal Chess Game!\nIt is a classic 2 player chess game.\nWhen selecting a piece you must format your input as D3, A5 etc.\nPress Enter to Continue.");
    getchar();
    while (1)
    {
        time = turn / 2;

        system("cls");
        printf("Chess Board (t = %d)\n", time);
        display();

        if (endGame() == 1)
        {
            if (player == WHITE)
                printf("\n\nPlayer 1(White) has won!\n");
            if (player == BLACK)
                printf("\n\nPlayer 2(Black) has won!\n");
            return 0;
        }
        
        if (turn % 2 == 0)
            player = WHITE;
        else
            player = BLACK;
        preparations();
    }
}

void display()
{
    for (int i = 0; i < 8; i++)
    {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                printf("%c", chess_board[i][j][k]);
            }
            printf(" ");
        }
        printf("\n");
    }
    printf("  ");
    for (int i = 0; i < 8; i++)
    {
        printf("%c  ", i + 65);
    }
}

void play()
{
    m = 1;

    if (player == WHITE)
    {
        cplayer = 'W';
        cenemy = 'B';
    }
    else if (player == BLACK)
    {
        cplayer = 'B';
        cenemy = 'W';
    }

selectPiece:
    printf("\nWhich piece you want to move: ");
    if (!fgets(square, 1024, stdin))
    {
        printf("\nInvalid selection please select again.\n");
        goto selectPiece;
    }
    square[strcspn(square, "\n")] = 0;
    if (strlen(square) > 2)
    {
        printf("\nInvalid selection please select again.\n");
        goto selectPiece;
    }

    n = ((int)square[0] - 64) * 10 + ((int)square[1] - 48);
    
    if (n < 11 || n > 88)
    {
        printf("\nInvalid selection please select again.\n");
        goto selectPiece;
    }

    x = n / 10;
    y = n % 10;

    positionX = 8 - y;
    positionY = x - 1;

    if (chess_board[positionX][positionY][0] == cplayer)
    {
        pieceMove();
    }
    else
    {
        printf("Invalid selection please select again.\n");
        goto selectPiece;
    }
}

void pieceMove()
{
    move = 0;
    switch (chess_board[positionX][positionY][1])
    {
    case 'P':
        getPawnMoves();
        break;
    case 'R':
        getRookMoves();
        break;
    case 'N':
        getKnightMoves(); 
        break;
    case 'B':
        getBishopMoves();
        break;
    case 'Q':
        getQueenMoves();
        break;
    case 'K':
        getKingMoves();
        break;
    }
    
    if (availibleMoves[1] == 0)
    {
        printf("\nNo moves availible, please select again.\n");
        play();
    }
    else
    {
        while (move == 0)
        {
        selectMove:
            printf("\nPlease select a move: ");
            if (!fgets(buff, 1024, stdin))
                goto selectMove;
            move = atoi(buff);
        }

        if (move <= m - 1 && move > 0)
        {
            tempPositionX = availibleMoves[move] / 10;
            tempPositionY = availibleMoves[move] % 10;

            if (chess_board[tempPositionX][tempPositionY][0] == cenemy)
                capturing();
            
            for (int i = 0; i < 2; i++)
            {
                chess_board[tempPositionX][tempPositionY][2] = '1';
                chess_board[tempPositionX][tempPositionY][i] = chess_board[positionX][positionY][i];
                chess_board[positionX][positionY][i] = '_';
            }
        }
        else if (castlingType == 3 && move > m - 1 && move <= m + 1)
        {
            if (move == m)
                queenSideCastle();
            else if (move == m + 1)
                kingSideCastle();
        }
        else if (castlingType == 2 && move == m)
            kingSideCastle();
        else if (castlingType == 1 && move == m)
            queenSideCastle();
        else if (move > m - 1 || move <= 0)
        {
            m = 1;
            printf("\nInvalid selection, please select again.\n");
            pieceMove();
        }
    }

    promotion();

    resetAvailibleMoves();
    resetPawnMoves();
    castlingType = 0;
}

void getPawnMoves()
{
    if (chess_board[positionX + 1][positionY][0] == '_')
        ob = EMPTY;
    if (chess_board[positionX + 2][positionY][0] == '_' && positionX == 1)
        tb = EMPTY;
    if (chess_board[positionX - 1][positionY][0] == '_')
        of = EMPTY;
    if (chess_board[positionX - 2][positionY][0] == '_' && positionX == 6)
        tf = EMPTY;
    if (chess_board[positionX - 1][positionY + 1][0] == cenemy)
        ur = ATTACKABLE;
    if (chess_board[positionX - 1][positionY - 1][0] == cenemy)
        ul = ATTACKABLE;
    if (chess_board[positionX + 1][positionY + 1][0] == cenemy)
        lr = ATTACKABLE;
    if (chess_board[positionX + 1][positionY - 1][0] == cenemy)
        ll = ATTACKABLE;

    if (player == WHITE)
    {
        if (of == EMPTY && tf == EMPTY)
        {
            printf("%d)%c%d %d)%c%d ", m, x + 64, y + 1, m + 1, x + 64, y + 2);
            availibleMoves[m] = (positionX - 1) * 10 + (positionY);
            m++;
            availibleMoves[m] = (positionX - 2) * 10 + (positionY);
            m++;
        }
        if (of == EMPTY && tf != EMPTY)
        {
            printf("%d)%c%d ", m, x + 64, y + 1);
            availibleMoves[m] = (positionX - 1) * 10 + (positionY);
            m++;
        }
        if (ur == ATTACKABLE)
        {
            printf("%d)%c%d ", m, x + 1 + 64, y + 1);
            availibleMoves[m] = (positionX - 1) * 10 + (positionY + 1);
            m++;
        }
        if (ul == ATTACKABLE)
        {
            printf("%d)%c%d ", m, x - 1 + 64, y + 1);
            availibleMoves[m] = (positionX - 1) * 10 + (positionY - 1);
            m++;
        }
    }
    else if (player == BLACK)
    {
        if (ob == EMPTY && tb == EMPTY)
        {
            printf("%d)%c%d %d)%c%d ", m, x + 64, y - 1, m + 1, x + 64, y - 2);
            availibleMoves[m] = (positionX + 1) * 10 + (positionY);
            m++;
            availibleMoves[m] = (positionX + 2) * 10 + (positionY);
            m++;
        }
        if (ob == EMPTY && tb != EMPTY)
        {
            printf("%d)%c%d ", m, x + 64, y - 1);
            availibleMoves[m] = (positionX + 1) * 10 + (positionY);
            m++;
        }
        if (lr == ATTACKABLE)
        {
            printf("%d)%c%d ", m, x + 1 + 64, y - 1);
            availibleMoves[m] = (positionX + 1) * 10 + (positionY + 1);
            m++;
        }
        if (ll == ATTACKABLE)
        {
            printf("%d)%c%d ", m, x - 1 + 64, y - 1);
            availibleMoves[m] = (positionX + 1) * 10 + (positionY - 1);
            m++;
        }
    }
}

void getRookMoves()
{
    for (i = 1; positionY - i >= 0; i++)
    {
        if (chess_board[positionX][positionY - i][0] == cplayer)
            break;
        if (chess_board[positionX][positionY - i][0] == cenemy)
        {
            printf("%d)%c%d ", m, x - i + 64, y);
            availibleMoves[m] = positionX * 10 + (positionY - i);
            m++;
            break;
        }
        else if (chess_board[positionX][positionY - i][0] == '_')
        {
            printf("%d)%c%d ", m, x - i + 64, y);
            availibleMoves[m] = positionX * 10 + (positionY - i);
            m++;
        }
    }
    for (i = 1; positionY + i <= 7; i++)
    {
        if (chess_board[positionX][positionY + i][0] == cplayer)
            break;
        if (chess_board[positionX][positionY + i][0] == cenemy)
        {
            printf("%d)%c%d ", m, x + i + 64, y);
            availibleMoves[m] = positionX * 10 + (positionY + i);
            m++;
            break;
        }
        else if (chess_board[positionX][positionY + i][0] == '_')
        {
            printf("%d)%c%d ", m, x + i + 64, y);
            availibleMoves[m] = positionX * 10 + (positionY + i);
            m++;
        }
    }
    for (i = 1; positionX - i >= 0; i++)
    {
        if (chess_board[positionX - i][positionY][0] == cplayer)
            break;
        if (chess_board[positionX - i][positionY][0] == cenemy)
        {
            printf("%d)%c%d ", m, x + 64, y + i);
            availibleMoves[m] = (positionX - i) * 10 + positionY;
            m++;
            break;
        }
        else if (chess_board[positionX - i][positionY][0] == '_')
        {
            printf("%d)%c%d ", m, x + 64, y + i);
            availibleMoves[m] = (positionX - i) * 10 + positionY;
            m++;
        }
    }
    for (i = 1; positionX + i <= 7; i++)
    {
        if (chess_board[positionX + i][positionY][0] == cplayer)
            break;
        if (chess_board[positionX + i][positionY][0] == cenemy)
        {
            printf("%d)%c%d ", m, x + 64, y - i);
            availibleMoves[m] = (positionX + i) * 10 + positionY;
            m++;
            break;
        }
        else if (chess_board[positionX + i][positionY][0] == '_')
        {
            printf("%d)%c%d ", m, x + 64, y - i);
            availibleMoves[m] = (positionX + i) * 10 + positionY;
            m++;
        }
    }
}

void getBishopMoves()
{
    for (i = 1; positionX + i <= 7 && positionY + i <= 7; i++)
    {
        if (chess_board[positionX + i][positionY + i][0] == cplayer)
            break;
        if (chess_board[positionX + i][positionY + i][0] == cenemy)
        {
            printf("%d)%c%d ", m, x + i + 64, y - i);
            availibleMoves[m] = (positionX + i) * 10 + (positionY + i);
            m++;
            break;
        }
        else if (chess_board[positionX + i][positionY + i][0] == '_')
        {
            printf("%d)%c%d ", m, x + i + 64, y - i);
            availibleMoves[m] = (positionX + i) * 10 + (positionY + i);
            m++;
        }
    }
    for (i = 1; positionX - i >= 0 && positionY + i <= 7; i++)
    {
        if (chess_board[positionX - i][positionY + i][0] == cplayer)
            break;
        if (chess_board[positionX - i][positionY + i][0] == cenemy)
        {
            printf("%d)%c%d ", m, x + i + 64, y + i);
            availibleMoves[m] = (positionX - i) * 10 + (positionY + i);
            m++;
            break;
        }
        else if (chess_board[positionX - i][positionY + i][0] == '_')
        {
            printf("%d)%c%d ", m, x + i + 64, y + i);
            availibleMoves[m] = (positionX - i) * 10 + (positionY + i);
            m++;
        }
    }
    for (i = 1; positionX + i <= 7 && positionY - i >= 0; i++)
    {
        if (chess_board[positionX + i][positionY - i][0] == cplayer)
            break;
        if (chess_board[positionX + i][positionY - i][0] == cenemy)
        {
            printf("%d)%c%d ", m, x - i + 64, y - i);
            availibleMoves[m] = (positionX + i) * 10 + (positionY - i);
            m++;
            break;
        }
        else if (chess_board[positionX + i][positionY - i][0] == '_')
        {
            printf("%d)%c%d ", m, x - i + 64, y - i);
            availibleMoves[m] = (positionX + i) * 10 + (positionY - i);
            m++;
        }
    }
    for (i = 1; positionX - i >= 0 && positionY - i >= 0; i++)
    {
        if (chess_board[positionX - i][positionY - i][0] == cplayer)
            break;
        if (chess_board[positionX - i][positionY - i][0] == cenemy)
        {
            printf("%d)%c%d ", m, x - i + 64, y + i);
            availibleMoves[m] = (positionX - i) * 10 + (positionY - i);
            m++;
            break;
        }
        else if (chess_board[positionX - i][positionY - i][0] == '_')
        {
            printf("%d)%c%d ", m, x - i + 64, y + i);
            availibleMoves[m] = (positionX - i) * 10 + (positionY - i);
            m++;
        }
    }
}

void getQueenMoves()
{
    getRookMoves();
    getBishopMoves();
}

void getKnightMoves()
{
    if (chess_board[positionX - 2][positionY + 1][0] != cplayer && positionX - 2 >= 0 && positionY + 1 <= 7)
    {
        printf("%d)%c%d ", m, x + 1 + 64, y + 2);
        availibleMoves[m] = (positionX - 2) * 10 + (positionY + 1);
        m++;
    }
    if (chess_board[positionX - 1][positionY + 2][0] != cplayer && positionX - 1 >= 0 && positionY + 2 <= 7)
    {
        printf("%d)%c%d ", m, x + 2 + 64, y + 1);
        availibleMoves[m] = (positionX - 1) * 10 + (positionY + 2);
        m++;
    }
    if (chess_board[positionX + 1][positionY + 2][0] != cplayer && positionX + 1 <= 7 && positionY + 1 <= 7)
    {
        printf("%d)%c%d ", m, x + 2 + 64, y - 1);
        availibleMoves[m] = (positionX + 1) * 10 + (positionY + 2);
        m++;
    }
    if (chess_board[positionX + 2][positionY + 1][0] != cplayer && positionX + 2 <= 7 && positionY + 1 <= 7)
    {
        printf("%d)%c%d ", m, x + 1 + 64, y - 2);
        availibleMoves[m] = (positionX + 2) * 10 + (positionY + 1);
        m++;
    }
    if (chess_board[positionX + 2][positionY - 1][0] != cplayer && positionX + 2 <= 7 && positionY - 1 >= 0)
    {
        printf("%d)%c%d ", m, x - 1 + 64, y - 2);
        availibleMoves[m] = (positionX + 2) * 10 + (positionY - 1);
        m++;
    }
    if (chess_board[positionX + 1][positionY - 2][0] != cplayer && positionX + 1 <= 7 && positionY - 2 >= 0)
    {
        printf("%d)%c%d ", m, x - 2 + 64, y - 1);
        availibleMoves[m] = (positionX + 1) * 10 + (positionY - 2);
        m++;
    }
    if (chess_board[positionX - 1][positionY - 2][0] != cplayer && positionX - 1 >= 0 && positionY - 2 >= 0)
    {
        printf("%d)%c%d ", m, x - 2 + 64, y + 1);
        availibleMoves[m] = (positionX - 1) * 10 + (positionY - 2);
        m++;
    }
    if (chess_board[positionX - 2][positionY - 1][0] != cplayer && positionX - 2 >= 0 && positionY - 1 >= 0)
    {
        printf("%d)%c%d ", m, x - 1 + 64, y + 2);
        availibleMoves[m] = (positionX - 2) * 10 + (positionY - 1);
        m++;
    }
}

void getKingMoves()
{
    if (chess_board[positionX - 1][positionY - 1][0] != cplayer && positionX - 1 >= 0 && positionY - 1 >= 0)
    {
        printf("%d)%c%d ", m, x - 1 + 64, y + 1);
        availibleMoves[m] = (positionX - 1) * 10 + (positionY - 1);
        m++;
    }
    if (chess_board[positionX - 1][positionY][0] != cplayer && positionX - 1 >= 0)
    {
        printf("%d)%c%d ", m, x + 64, y + 1);
        availibleMoves[m] = (positionX - 1) * 10 + (positionY);
        m++;
    }
    if (chess_board[positionX - 1][positionY + 1][0] != cplayer && positionX - 1 >= 0 && positionY + 1 <= 7)
    {
        printf("%d)%c%d ", m, x + 1 + 64, y + 1);
        availibleMoves[m] = (positionX - 1) * 10 + (positionY + 1);
        m++;
    }
    if (chess_board[positionX][positionY - 1][0] != cplayer && positionY - 1 >= 0)
    {
        printf("%d)%c%d ", m, x - 1 + 64, y);
        availibleMoves[m] = (positionX) * 10 + (positionY - 1);
        m++;
    }
    if (chess_board[positionX][positionY + 1][0] != cplayer && positionY + 1 <= 7)
    {
        printf("%d)%c%d ", m, x + 1 + 64, y);
        availibleMoves[m] = (positionX) * 10 + (positionY + 1);
        m++;
    }
    if (chess_board[positionX + 1][positionY - 1][0] != cplayer && positionX + 1 <= 7 && positionY - 1 >= 0)
    {
        printf("%d)%c%d ", m, x - 1 + 64, y - 1);
        availibleMoves[m] = (positionX + 1) * 10 + (positionY - 1);
        m++;
    }
    if (chess_board[positionX + 1][positionY][0] != cplayer && positionX + 1 <= 7)
    {
        printf("%d)%c%d ", m, x + 64, y - 1);
        availibleMoves[m] = (positionX + 1) * 10 + (positionY);
        m++;
    }
    if (chess_board[positionX + 1][positionY + 1][0] != cplayer && positionX + 1 <= 7 && positionY + 1 <= 7)
    {
        printf("%d)%c%d ", m, x + 1 + 64, y - 1);
        availibleMoves[m] = (positionX + 1) * 10 + (positionY + 1);
        m++;
    }

    castlingType = castling();
    if (castling() == 3)
        printf("%d)Queen Side Castle %d)King Side Castle", m, m + 1);
    else if (castling() == 2)
        printf("%d)King Side Castle", m);
    else if (castling() == 1)
        printf("%d)Queen Side Castle", m);
}

int castling()
{
    int qsc = 0;
    int ksc = 0;

    if (chess_board[positionX][positionY][2] == '0' && chess_board[positionX][positionY + 1][0] == '_' && chess_board[positionX][positionY + 2][0] == '_' && chess_board[positionX][positionY + 3][2] == '0')
        ksc = 1;
    if (chess_board[positionX][positionY][2] == '0' && chess_board[positionX][positionY - 1][0] == '_' && chess_board[positionX][positionY - 2][0] == '_' && chess_board[positionX][positionY - 3][0] == '_' && chess_board[positionX][positionY - 4][2] == '0')
        qsc = 1;

    if (ksc == 1 && qsc == 1)
        return 3;
    else if (ksc == 1)
        return 2;
    else if (qsc == 1)
        return 1;
    else
        return 0;
}

void kingSideCastle()
{
    for (i = 0; i < 2; i++)
    {
        chess_board[positionX][positionY + 2][i] = chess_board[positionX][positionY][i];
        chess_board[positionX][positionY][i] = '_';
        chess_board[positionX][positionY + 2][2] = '1';

        chess_board[positionX][positionY + 1][i] = chess_board[positionX][positionY + 3][i];
        chess_board[positionX][positionY + 3][i] = '_';
        chess_board[positionX][positionY + 1][2] = '1';
    }
}

void queenSideCastle()
{
    for (i = 0; i < 2; i++)
    {
        chess_board[positionX][positionY - 2][i] = chess_board[positionX][positionY][i];
        chess_board[positionX][positionY][i] = '_';
        chess_board[positionX][positionY - 2][2] = '1';

        chess_board[positionX][positionY - 1][i] = chess_board[positionX][positionY - 4][i];
        chess_board[positionX][positionY - 4][i] = '_';
        chess_board[positionX][positionY - 1][2] = '1';
    }
}

int canPromote()
{
    if (player == WHITE)
    {
        if (tempPositionX == 0)
            return 1;
        else
            return 0;
    }
    if (player == BLACK)
    {
        if (tempPositionX == 7)
            return 1;
        else
            return 0;
    }
}

void promotion()
{
    if (chess_board[tempPositionX][tempPositionY][1] == 'P' && canPromote() == 1)
    {
        while (promotionSelection == 0)
        {
        selectPromotion:
            printf("\n1)Rook 2)Knight 3)Bishop 4)Queen\n");
            printf("\nPlease choose a promotion: ");
            if (!fgets(buff, 1024, stdin))
                goto selectPromotion;
            promotionSelection = atoi(buff);
        }
        
        switch (promotionSelection)
        {
        case 1:
            chess_board[tempPositionX][tempPositionY][1] = 'R';
            break;
        case 2:
            chess_board[tempPositionX][tempPositionY][1] = 'N';
            break;
        case 3:
            chess_board[tempPositionX][tempPositionY][1] = 'B';
            break;
        case 4:
            chess_board[tempPositionX][tempPositionY][1] = 'Q';
            break;
        default:
            printf("\nInvalid selection, please select again.\n");
            goto selectPromotion;
        }
    }
}

void preparations()
{
    if (player == WHITE)
    {
        printf("\n\nPlayer 1 (White)\n");
        if (capturerX != 0)
            printf("\nElimination %c%d %c%c => %c%d %c%c\n", capturerX + 64, capturerY, capturerPiece[0], capturerPiece[1], capturedX + 64, capturedY, capturedPiece[0], capturedPiece[1]);
        resetCapturing();
        play();
        turn++;
    }
    else if (player == BLACK)
    {
        printf("\n\nPlayer 2 (Black)\n");
        if (capturerX != 0)
            printf("\nElimination %c%d %c%c => %c%d %c%c\n", capturerX + 64, capturerY, capturerPiece[0], capturerPiece[1], capturedX + 64, capturedY, capturedPiece[0], capturedPiece[1]);
        resetCapturing();
        play();
        turn++;
    }
}

void capturing()
{
    capturerX = x; 
    capturerY = y;

    capturedX = tempPositionY + 1;
    capturedY = 8 - tempPositionX;

    capturerPiece[0] = cplayer;
    capturerPiece[1] = chess_board[positionX][positionY][1];

    capturedPiece[0] = cenemy;
    capturedPiece[1] = chess_board[tempPositionX][tempPositionY][1];
}

int endGame()
{
    if (capturedPiece[1] == 'K')
        return 1;
    else
        return 0;
}

void resetPawnMoves()
{
    of = OCCUPIED, tf = OCCUPIED, ul = OCCUPIED, ur = OCCUPIED, ob = OCCUPIED, tb = OCCUPIED, ll = OCCUPIED, lr = OCCUPIED;
}

void resetAvailibleMoves()
{
    for (int i = 0; i < 100; i++)
    {
        availibleMoves[i] = 0;
    }
}

void resetCapturing()
{
    capturerX = 0;
}