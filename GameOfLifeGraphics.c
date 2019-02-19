//ETEC2110.01
//Jackie Ha (╯°□°）╯︵ ┻━┻
//Assignment: Lab 7, Program 1
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
//#include "GameofLife.h"

//Screen dimension constants
int SCREEN_WIDTH = 1001;
int SCREEN_HEIGHT = 501;


int main(int argc, char *argv[])
{
    //int i, j;
    int running = 1;    //1 = true, 0 = false
    int paused = 0;
    Uint32 total = 0;
    //time_t t;
    int gameBoard[BOARD_WIDTH][BOARD_HEIGHT];
    init_board(gameBoard, 10000);
    SDL_Event event;
    SDL_Window* window = NULL; //the window we'll be rendering to
    SDL_Surface* screenSurface = NULL; //the surface contained by the window


    //Initialize random number generator???
    //srand((unsigned) time(&t));

    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL not initialized. SDL_Error: %s\n", SDL_GetError());
        }
    else
        {
            //Create window
            window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

            //Get window surface
            screenSurface = SDL_GetWindowSurface(window);

            while(running)
                {
                    Uint32 elapsed = SDL_GetTicks()/1000;
                    total += elapsed;
                    if(paused == 1)
                    {
                        total = 0;
                    }
                    if(total > 0)
                        {
                            next_generation(gameBoard);
                            total = 0;
                        }
                    while(SDL_PollEvent(&event) != 0)
                        {
                            const Uint8 *state = SDL_GetKeyboardState(NULL);
                            //Exits the screen when user clicks 'x' or presses escape
                            if(event.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE] || state[SDL_SCANCODE_Q])
                                {
                                    running = 0;
                                }
                            //Resets the screen with a new board of 10000 randomly chosen living cells
                            else if(event.type == SDL_KEYDOWN && state[SDL_SCANCODE_R])
                                {
                                    init_board(gameBoard, 10000);
                                    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format,
                                                                    0xFF, 0xFF, 0xFF));
                                    display_board(gameBoard, screenSurface);
                                    SDL_UpdateWindowSurface(window);
                                }
                            //Pauses/un-pauses the
                            else if(event.type == SDL_KEYDOWN && state[SDL_SCANCODE_P])
                                {
                                    paused = !paused;
                                }
                            //Adds user-specified amount of cells
                            else if(event.type == SDL_KEYDOWN && state[SDL_SCANCODE_A])
                                {
                                    add_cells(gameBoard, 100);
                                    display_board(gameBoard, screenSurface);
                                    SDL_UpdateWindowSurface(window);
                                }
                            //Cycles through all of the living cells and uses a 10% chance that each cell will be killed off
                            else if(event.type == SDL_KEYDOWN && state[SDL_SCANCODE_K])
                                {
                                    printf("cycle");
                                }
                            else if(event.type == SDL_KEYDOWN && state[SDL_SCANCODE_SPACE])
                                {
                                    if(paused == 1)
                                        {
                                            next_generation(gameBoard);
                                        }
                                }
                        }

                    //Fill the surface white
                    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format,
                                                                        0xFF, 0xFF, 0xFF));
                    //Draw cells
                    display_board(gameBoard, screenSurface);

                    //Update the surface
                    SDL_UpdateWindowSurface(window);

                    SDL_Delay(60);
                }
        }
        return 0;
}



//Function that initializes the game of life board
void init_board(int board[][BOARD_HEIGHT], int num_alive)
{
    int i, j, a;
    int r, ra;  //variables for picking random cells

    //Fills 2d array with 0's
    for(i = 0; i < BOARD_WIDTH; i++)
    {
        for(j = 0; j < BOARD_HEIGHT; j++)
        {
            board[i][j] = 0;
        }
    }
    //Picks num_alive cells to turn into 1's
    srand(time(NULL));
    for(a = 0; a < num_alive; a++)
    {
        r = rand() % BOARD_WIDTH;
        ra = rand() % BOARD_HEIGHT;
        board[r][ra] = 1;
    }
}

//Function that displays live cells
void display_board(int board[][BOARD_HEIGHT], SDL_Surface* surf)
{
    int x, y;
    for(x = 0; x < BOARD_WIDTH; x++)
    {
        for(y = 0; y < BOARD_HEIGHT; y++)
        {
            if(board[x][y] == 1)
            {
                SDL_Rect rect = {(x*5) + 1, (y*5) + 1, 4, 4};
                SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, 0x00, 0x00, 0xFF));
            }
        }
    }
}

//Function that finds the number of live neighbors around a cell
int num_neighbors(int board[][BOARD_HEIGHT], int x, int y)
{
    int alive = 0;

    //Bounds checking! All cells outside of the board are considered dead
    if(((x != -1) || (x != BOARD_WIDTH)) && ((y != -1) || (y != BOARD_HEIGHT)))
    {
        if(board[x-1][y-1] == 1){alive++;}
        if(board[x][y-1] == 1){alive++;}
        if(board[x+1][y-1] == 1){alive++;}
        if(board[x-1][y] == 1){alive++;}
        if(board[x+1][y] == 1){alive++;}
        if(board[x-1][y+1] == 1){alive++;}
        if(board[x][y+1] == 1){alive++;}
        if(board[x+1][y+1] == 1){alive++;}
    }
    return alive;
}

//Function that determines the next state of the specified cell
int next_state(int board[][BOARD_HEIGHT], int x, int y)
{
    if((board[x][y] ==  1) & ((num_neighbors(board, x, y) == 2) || (num_neighbors(board, x, y) == 3)))
        {return 1;}
    if((board[x][y] == 0) & (num_neighbors(board, x, y) == 3))
        {return 1;}
    return 0;
}

//Function that generates the next generation of cells
void next_generation(int board[][BOARD_HEIGHT])
{
    int i, j;
    int temp[BOARD_WIDTH][BOARD_HEIGHT];

    for(i = 0; i < BOARD_WIDTH; i++)
    {
        for(j = 0; j < BOARD_HEIGHT; j++)
        {
            if(next_state(board, i, j) == 1)
            {
                temp[i][j] = 1;
            }
            else
            {
                temp[i][j] = 0;
            }
        }
    }
    memcpy(board, temp, sizeof(temp));
}

//Function that adds new_cells amount of living cells to the board
void add_cells(int board[][BOARD_HEIGHT], int new_cells)
{
    int a;
    int r, ra;

    srand(time(NULL));
    for(a = 0; a < new_cells; a++)
    {
        r = rand() % BOARD_WIDTH;
        ra = rand() % BOARD_HEIGHT;
        if(board[r][ra] != 1)
        {
            board[r][ra] = 1;
        }
    }
}
