/* Lily Ehsani, lilyehsani
 * CS 152, Winter 2020
 * Project 2
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/pos.h"
#include "../include/board.h"

/* num_ints: outputs the amount of ints necessary for an array of the
 * given amount of bits (calls) */
unsigned int num_ints(unsigned int bits) {
    unsigned int count = 1;
    while (bits >= 16) {
        count++;
        bits = bits - 16;
    }
    return count;
}

board* board_new(unsigned int width, unsigned int height, enum type type) {
    /* checking that the given dimensions are appropriate */
    if (width == 0) {
        fprintf(stderr, "board_new: cannot have a width of 0");
        exit(1);
    }
    if (height == 0) {
        fprintf(stderr, "board_new: cannot have a height of 0");
        exit(1);
    }
    if (height < 2 && width < 2) {
        fprintf(stderr, "board_new: board must be larger than 1x1");
        exit(1);
    }
    /* begin function */
    board* res = (board*)malloc(sizeof(board));
    res->width = width;
    res->height = height;
    res->type = type;
    if (type == MATRIX) {
        cell** m = (cell**)malloc(sizeof(cell*) * height);
        unsigned int r, c;
        for (r = 0; r < height; r++) {
            m[r] = (cell*)malloc(sizeof(cell) * width);
            for (c = 0; c < width; c++) {
                m[r][c] = EMPTY;
            }
        }
        res->u.matrix = m;
    } else { /* if (type == BITS) */
        unsigned int cells, elems, i;
        cells = (width * height); /* total # of cells on board */
        elems = num_ints(cells); /* # of elements in array */
        unsigned int* b = (unsigned int*)malloc(sizeof(unsigned int)
                                                * elems);
        for (i = 0; i < elems; i++) {
            b[i] = 0;
        }
        res->u.bits = b;
    }
    return res;
}
        

void board_free(board* b) {
    if (b->type == MATRIX) {
        unsigned int i, rows = b->height;
        for (i = 0; i < rows; i++) {
            free(b->u.matrix[i]);
        }
    } else { /* if (type == BITS) */
        free(b->u.bits);
    }
    free(b);
}

/* print_right_char: prints the correct char for the given board index */
void print_right_char(unsigned int cmp) {
    if (cmp < 10) {
        printf("%d", cmp);
    } else if (cmp < 36) {
        printf("%c", (cmp + 55));
    } else if (cmp < 62) {
        printf("%c", (cmp + 61));
    } else {
        printf("?");
    }
}

void board_show(board* b) {
    printf("  ");
    unsigned int w = b->width, h = b->height;
    unsigned int r, c;
    for (c = 0; c < w; c++) {
        print_right_char(c);
    }
    printf("\n\n");
    for (r = 0; r < h; r++) {
        print_right_char(r);
        printf(" ");
        for (c = 0; c < w; c++) {
            switch (board_get(b, make_pos(r, c))) {
                case EMPTY:
                    printf(".");
                    break;
                case BLACK:
                    printf("*");
                    break;
                case WHITE:
                    printf("o");
                    break;
            }
        }
        printf("\n");
    }
}

/* get_cell_bits: uses bitwise operators to retrieve the type of cell
 * at the given index (0 being the rightmost digits in the int, 1 the
 * next rightmost, etc) in the given int (00 for EMPTY, 01 for BLACK,
 * 10 for WHITE).  */
unsigned int get_cell_bits(unsigned int x, unsigned int index) {
    x = x >> (index * 2);
    x = x & 3;
    return x;
}

/* put_cell_bits: uses bitwise operators to input the cell bits of
 * the given color in the given int at the given index, as
 * specified above */
unsigned int put_cell_bits(unsigned int x, unsigned int index,
                           unsigned int color) {
    unsigned int mask, new;
    mask = 3 << (index * 2);
    mask = ~mask;
    new = x & mask;
    new = new | (color << (index * 2));
    return new;
}

cell board_get(board* b, pos p) {
    /* checking if the given position is appropriate with the dimensions */
    if (p.c >= b->width) {
        fprintf(stderr, "board_get: column position out of bounds");
    }
    if (p.r >= b->height) {
        fprintf(stderr, "board_get: row position out of bounds");
    }
    /* begin function */
    cell res;
    if (b->type == MATRIX) {
        res = b->u.matrix[p.r][p.c];
    } else { /* if (type == BITS) */
        unsigned int elems, index_of_cell, index_of_elem, 
                     index_of_cell_in_elem, x;
        elems = num_ints(b->width * b->height);
        index_of_cell = (p.r * b->width) + p.c;
        index_of_elem = (num_ints(index_of_cell) - 1);
        index_of_cell_in_elem = (index_of_cell - (index_of_elem * 16));
        x = get_cell_bits(b->u.bits[index_of_elem], index_of_cell_in_elem);
        switch (x) {
            case 0:
                res = EMPTY;
                break;
            case 1:
                res = BLACK;
                break;
            case 2:
                res = WHITE;
                break;
        }
    }
    return res;
}

void board_set(board* b, pos p, cell c) {
    /* checking if the given position is appropriate with the dimensions */
    if (p.c >= b->width) {
        fprintf(stderr, "board_set: column position out of bounds");
    }
    if (p.r >= b->height) {
        fprintf(stderr, "board_set: row position out of bounds");
    }
    /* begin function */
    if (b->type == MATRIX) {
        b->u.matrix[p.r][p.c] = c;
    } else { /* if (type == BITS) */
        unsigned int elems, index_of_cell, index_of_elem, x, color;
        elems = num_ints(b->width * b->height);
        index_of_cell = (p.r * b->width) + p.c;
        index_of_elem = (num_ints(index_of_cell) - 1);
        switch (c) {
            case EMPTY:
                color = 0;
                break;
            case BLACK:
                color = 1;
                break;
            case WHITE:
                color = 2;
                break;
        }
        x = put_cell_bits(b->u.bits[index_of_elem],
                          (index_of_cell - (index_of_elem * 16)),
                          color);        
        b->u.bits[index_of_elem] = x;
    }
}
