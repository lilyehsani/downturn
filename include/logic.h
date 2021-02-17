/* Lily Ehsani, lilyehsani
 * CS 152, Winter 2020
 * Project 2
 */

#ifndef LOGIC_H
#define LOGIC_H

#include "board.h"

enum turn {
    BLACK_NEXT,
    WHITE_NEXT
};

typedef enum turn turn;

enum outcome {
    BLACK_WIN,
    WHITE_WIN,
    DRAW,
    IN_PROGRESS
};

typedef enum outcome outcome;

enum direction {
    CW, CCW
};

typedef enum direction direction;

struct game {
    unsigned int run;
    board* b;
    turn next;
};

typedef struct game game;

/* new_game: outputs a pointer to a game with an empty board of the given
 * dimensions of the given type, with the given run parameter */
game* new_game(unsigned int run, unsigned int width, unsigned int height,
               enum type type);

/* game_free: frees the board within a game pointer and the game pointer */
void game_free(game* g);

/* drop_piece: following the laws of gravity, drops a piece of whichever
 * turn it is in the game in the given column */
int drop_piece(game* g, unsigned int col);

/* twist: rotates the board in the given game in the given direction */
void twist(game* g, direction d);

/* the following are helper functions for game_outcome. I moved them to this
 * file because I test them in my evidence file. full documentation can be
 * found in logic.c */
int board_full(game* g);

int vertical(game* g, cell p);

int horizontal(game* g, cell p);

int diagonal_pos(game* g, cell p);

int diagonal_neg(game* g, cell p);

/* game_outcome: outputs the correct output of the given game if it is
 * finished, outputs in progress if not */
outcome game_outcome(game* g);

#endif /* LOGIC_H */
