#include "raylib.h"

#define FRAME 20

typedef struct Turn{
    int x;
    int y;
}Turn;

int Minimax(int* board, int playerID);
void computerMove(int* board);
int win(int* board, int* startPos, int* endPos);

int main(void)
{
    
    const int screenWidth = 900;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "TicTacToe");

    int scene = 0;

    int tableWidth = 220;
    int tableHeigth = 220;
    
    Texture2D cross = LoadTexture("Assets/cross.png");
    Texture2D circle = LoadTexture("Assets/circle.png");
    Image icon = LoadImage("Assets/cross.png");

    Rectangle table[3][3] = { 0 };

    int tableArr[3][3] = { 0 };
    bool circleStatus[3][3] = { 0 };

    Turn CPU = { -1, -1 };

    int playerId = -1;
    int CPUId = 1;
    int startPos = 0;
    int endPos = 0;

    Vector2 mousePos = { 0, 0 };

    bool turnCPU = false;

    bool winPlayer = false;
    bool winCPU = false;
    bool draw = false;
    bool gameEnd = false;

    int playerWins = 0;
    int CPUWins = 0;

    int framesCounter = 0;// frames since the game start
    int counterFPS = 0; // FPS 

    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            table[y][x].width = tableWidth;
            table[y][x].height = tableHeigth;
            table[y][x].x = FRAME + y * FRAME + y * table[y][x].width;
            table[y][x].y = FRAME + x * FRAME + x * table[y][x].height;
        }
    }

    SetTargetFPS(60);
    SetWindowIcon(icon);

    counterFPS = GetFPS();

    SetWindowTitle(TextFormat("Tic-Tac-Toe FPS %i", counterFPS));

    while (!WindowShouldClose() && !IsKeyPressed(KEY_ESCAPE)) 
    {

        //UPDATE
        if ((scene == 1) && (!winPlayer) && (!winCPU) && (!draw))
        {
            //Player move
            mousePos = GetMousePosition();
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    if (CheckCollisionPointRec(mousePos, table[y][x]))
                    {
                        if (tableArr[y][x] == 0) circleStatus[y][x] = true;
                        else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && tableArr[y][x] == 0)
                        {
                            tableArr[y][x] = playerId;
                            turnCPU = true;
                        }
                    }
                    else if (!CheckCollisionPointRec(mousePos, table[y][x]))
                    {
                        circleStatus[y][x] = false;
                    }
                }
            }
            //CPU move
            if (turnCPU)
            {
                computerMove(tableArr);
                turnCPU = false;
            }

            int winner = win(tableArr, &startPos, &endPos);
            // Winner check
            if (winner == playerId)
            {
                winPlayer = true;
                gameEnd = true;
            }
            else if (winner == CPUId)
            {
                winCPU = true;
                gameEnd = true;
            }

            // Draw check
            int counter = 0;
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    if (tableArr[y][x] != 0) counter++;
                }
            }
            if (counter >= 9)
            {
                draw = true;
                gameEnd = true;
            }
        }
        framesCounter++;

        switch (scene)
        {
            case 0:
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) scene++;
            }
            case 1:
            {
                if (IsKeyPressed(KEY_R))
                {
                    for (int y = 0; y < 3; y++)
                    {
                        for (int x = 0; x < 3; x++)
                        {
                            tableArr[y][x] = 0;
                        }
                    }
                    CPUWins = 0;
                    playerWins = 0;
                }

                if (IsKeyPressed(KEY_ENTER) && gameEnd)
                {
                    for (int y = 0; y < 3; y++)
                    {
                        for (int x = 0; x < 3; x++)
                        {
                             tableArr[y][x] = 0;
                        }
                    }
                    playerWins++;
                    winPlayer = false;
                    winCPU = false;
                    draw = false;
                    gameEnd = false;
                }
            }
            default:break;
        }
    
        BeginDrawing();

        switch (scene)
        {
            case 0:
            {

                ClearBackground(RAYWHITE);

                DrawText("Welcome to TIC-TAC-TOE", 40, screenHeight / 2 + 40, 40, BLACK);
                if ((framesCounter / 30) % 2 == 0) DrawText("Click to play!", 60, screenHeight / 2 + 90, 40, BLACK);

            }break;
            case 1:
            {

                ClearBackground(BLACK);

                for (int x = 0; x < 3; x++)// Drawing the table
                {
                    for (int y = 0; y < 3; y++)
                    {
                        DrawRectangleRec(table[y][x], RAYWHITE);
                    }
                }

                for (int x = 0; x < 3; x++)// Drawing the textures
                {
                    for (int y = 0; y < 3; y++)
                    {
                        if (tableArr[y][x] == playerId || circleStatus[y][x])
                        {
                            DrawTexture(circle, table[y][x].x, table[y][x].y, WHITE);
                        }
                        else if (tableArr[y][x] == CPUId)
                        {
                            DrawTexture(cross, table[y][x].x, table[y][x].y, WHITE);
                        }
                    }
                }

                Vector2 starPosV = { table[startPos / 3][startPos % 3].x + tableWidth / 2, table[startPos / 3][startPos % 3].y + tableHeigth / 2 };
                Vector2 endPosV = { table[endPos / 3][endPos % 3].x + tableWidth / 2, table[endPos / 3][endPos % 3].y + tableHeigth / 2 };

                if (winPlayer == true)
                {
                    DrawLineEx(starPosV, endPosV, 60, RED);
                    if ((framesCounter / 30) % 2 == 0) DrawText("YOU WIN!", 50, 330, 60, GREEN);
                    DrawText("Press ENTER to continue", 50, 450, 40, RED);

                }
                else if (winCPU == true)
                {
                    DrawLineEx(starPosV, endPosV, 60, RED);
                    if ((framesCounter / 30) % 2 == 0) DrawText("CPU WIN!", 50, 330, 60, GREEN);
                    DrawText("Press ENTER to continue", 50, 450, 40, RED);
                }
                else if (draw == true)
                {
                    if ((framesCounter / 30) % 2 == 0) DrawText("DRAW!", 50, 330, 60, RED);
                    DrawText("Press ENTER to continue", 50, 450, 40, RED);
                }

                DrawText(TextFormat("Player games won: %i", playerWins), 20, 750, 30, RAYWHITE);
                DrawText(TextFormat("CPU games won: %i", CPUWins), 20, 790, 30, RAYWHITE);
                DrawText("Press R to restart", 20, 830, 30, RAYWHITE);
                DrawText("Press ESC to exit", 20, 870, 30, RAYWHITE);

            }break;
            default:break;
        }
        EndDrawing();
    }

    UnloadTexture(cross);
    UnloadTexture(circle);
    
    CloseWindow(); 

    return 0;
}

int win(int* board, int* startPos, int* endPos) 
{
    unsigned wins[8][3] = { {0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6} };
    for (int i = 0; i < 8; ++i) 
    {
        if (board[wins[i][0]] != 0 &&
            board[wins[i][0]] == board[wins[i][1]] &&
            board[wins[i][0]] == board[wins[i][2]])
        {
            if (startPos && endPos)
            {
                *startPos = wins[i][0];
                *endPos = wins[i][2];
            }
            return board[wins[i][2]];
        }
    }
    return 0;
}

int Minimax(int* board, int playerID)
{
    int winner = win(board, 0, 0);
    if (winner != 0) return winner * playerID;

    int move = -1;
    int score = -2;//Losing moves are preferred to no move
    
    for (int i = 0; i < 9; ++i) 
    {//For all moves,
        
        if (board[i] == 0) 
        {//If legal
            board[i] = playerID;//Try the move
            int thisScore = -Minimax(board, playerID * -1);
            if (thisScore > score) 
            {
                score = thisScore;
                move = i;
            }//Pick the one that's worst for the opponent
            board[i] = 0;//Reset board after try
        }
        
    }
    if (move == -1) return 0;
    return score;
}
void computerMove(int* board) 
{
    int move = -1;
    int score = -2;

    for (int i = 0; i < 9; ++i) 
    {
        if (board[i] == 0) {
            board[i] = 1;
            int tempScore = -Minimax(board, -1);
            board[i] = 0;
            if (tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    if (move >= 0 && move < 9)
    {
        board[move] = 1;
    }
}