/* Lily Ehsani, lilyehsani
 * CS 152, Winter 2020
 * Project 2
 */

#ifndef BOARD_H
#define BOARD_H

#include "pos.h"

enum cell {
    EMPTY,
    BLACK,
    WHITE
};

typedef enum cell cell;

union board_rep {
    enum cell** matrix;
    unsigned int* bits;
};

typedef union board_rep board_rep;

enum type {
    MATRIX, BITS
};

struct board {
    unsigned int width, height;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* board_new: outputs a pointer to an empty board with the given dimensions 
 * of the given type */
board* board_new(unsigned int width, unsigned int height, enum type type);

/* board_free: frees the memory allocated for the given board pointer */
void board_free(board* b);

/* board_show: prints the given board to the screen with the correct row
 * and column labels, '*' representing a black piece, 'o' a white, and 
 * '.' an exmpty space */
void board_show(board* b);

/* board_get: outputs the cell (enumeration) of the given board at the
 * given position */
cell board_get(board* b, pos p);

/* board_set: changes the given board to have the given cell at the given
 * position, instead of whatever cell was there before */
void board_set(board* b, pos p, cell c);

#endif /* BOARD_H */
