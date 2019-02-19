#ifndef GAMEOFLIFE_H_INCLUDED
#define GAMEOFLIFE_H_INCLUDED

#define BOARD_HEIGHT 100
#define BOARD_WIDTH 200


void init_board(int board[][BOARD_HEIGHT], int num_alive);
void display_board(int board[][BOARD_HEIGHT], SDL_Surface* surf);
int num_neighbors(int board[][BOARD_HEIGHT], int x, int y);
int next_state(int board[][BOARD_HEIGHT], int x, int y);
void next_generation(int board[][BOARD_HEIGHT]);
void add_cells(int board[][BOARD_HEIGHT], int new_cells);

#endif // GAMEOFLIFE_H_INCLUDED
