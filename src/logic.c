/* Lily Ehsani, lilyehsani
 * CS 152, Winter 2020
 * Project 2
 */

#include <stdlib.h>
#include <stdio.h>
#include "../include/pos.h"
#include "../include/board.h"
#include "../include/logic.h"

game* new_game(unsigned int run, unsigned int width, unsigned int height,
               enum type type) {
    /* checking if the given run works with the given dimensions */
    if (run <= 1) {
        fprintf(stderr, "new_game: run must be greater than 1");
        exit(1);
    }
    if (run > width && run > height) {
        fprintf(stderr, "new_game: run cannot be greater than both width "
                        "and height. r=%d, w=%d, h=%d", run, width, height);
        exit(1);
    }
    /* begin function */
    game* res = (game*)malloc(sizeof(game));
    res->run = run;
    res->b = board_new(width, height, type);
    res->next = BLACK_NEXT;
    return res;
}

void game_free(game* g) {
    board_free(g->b);
    free(g);
}

/* drop_any_piece: observing the laws of gravity, alters the given board
 * by dropping a piece of the given cell color into the given column.
 * outputs 0 if the column is full, 1 if the piece was dropped */
int drop_any_piece(board* b, unsigned int col, cell c) {
    int i = b->height - 1; /* index of the lowest row */
    while (board_get(b, make_pos(i, col)) != EMPTY) {
        i--;
        if (i < 0) { /* if row 0 is also full, i becomes -1 */
            return 0;
        }
    } 
    board_set(b, make_pos(i, col), c);
    return 1;
}

int drop_piece(game* g, unsigned int col) {
    cell c;
    if (g->next == BLACK_NEXT) {
        c = BLACK;
    } else {
        c = WHITE;
    }
    return drop_any_piece(g->b, col, c);
}

void twist(game* g, direction d) {
    unsigned int orig_h = g->b->height, orig_w = g->b->width;
    board* res = board_new(orig_h, orig_w, MATRIX);
    unsigned int i, j;
    for (j = 0; j < orig_w; j++) {
        for (i = 0; i < orig_h; i++) {
            if (d == CCW) {
                cell c = board_get(g->b, make_pos((orig_h - 1 - i), j));
                if (c == EMPTY) {
                } else {
                    drop_any_piece(res, (orig_h - 1 - i), c);
                }
            }
            if (d == CW) {
                cell c = board_get(g->b, make_pos((orig_h - 1 - i),
                                   (orig_w - 1 - j)));
                if (c == EMPTY) {
                } else {
                    drop_any_piece(res, i, c);
                }
            }
        }
    }
    g->b = res;
}

/* board_full: outputs 1 iff all columns in the given board are full,
 * 0 otherwise. helper for game_outcome */
int board_full(game* g) {
    int i = g->b->width - 1;
    while (board_get(g->b, make_pos(0, i)) != EMPTY) {
        i--;
        if (i < 0) {
            return 1;
        }
    }
    return 0;
}

/* vertical: outputs 1 iff the given cell color has a winning vertical
 * run in the given game g. helper for game_outcome */
int vertical(game* g, cell p) {
    if (g->run > g->b->height) {
        return 0;
    }
    int r, c;
    for (c = 0; c < g->b->width; c++) {
        for (r = g->b->height - 1; r >= g->run - 1; r--) {
            unsigned int count = 0, i = r;
            while (board_get(g->b, make_pos(i, c)) == p) {
                count++;
                i--;
                if (count == g->run) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* horizontal: outputs 1 iff the given cell color has a winning horizontal
 * run in the given game g. helper for game_outcome */
int horizontal(game* g, cell p) {
    if (g->run > g->b->width) {
        return 0;
    }
    int r, c;
    for (r = g->b->height - 1; r >= 0; r--) {
        for (c = 0; c <= (g->b->width - g->run); c++) {
            unsigned int count = 0, j = c;
            while (board_get(g->b, make_pos(r, j)) == p) {
                count++;
                j++;
                if (count == g->run) {
                    return 1;
                }
            }   
        }
    }
    return 0;
}

/* diagonal_pos: outputs 1 iff the given cell color has a winning diagonal
 * run in the given game g. pos indicates a positive slope on an xy grid.
 * helper for game_outcome */
int diagonal_pos(game* g, cell p) {
    if (g->run > g->b->width || g->run > g->b->height) {
        return 0;
    }
    int r, c;
    for (r = g->b->height - 1; r >= g->run - 1; r--) {
        for (c = 0; c <= (g->b->width - g->run); c++) {
            unsigned int count = 0, i = r, j = c;
            while (board_get(g->b, make_pos(i, j)) == p) {
                count++;
                i--;
                j++;
                if (count == g->run) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* diagonal_neg: outputs 1 iff the given cell color has a winning diagonal
 * run in the given game g. neg indicates a negative slope on an xy grid.
 * helper for game_outcome */
int diagonal_neg(game* g, cell p) {
     if (g->run > g->b->width || g->run > g->b->height) {
        return 0;
    }
    int r, c;
    for (r = g->b->height - 1; r >= g->run - 1; r--) {
        for (c = g->b->width - 1; c >= g->run - 1; c--) {
            unsigned int count = 0, i = r, j = c;
            while (board_get(g->b, make_pos(i, j)) == p) {
                count++;
                i--;
                j--;
                if (count == g->run) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* win: outputs 1 iff the given cell color has a winning run in any
 * direction. helper for game_outcome */
int win(game* g, cell p) {
    return (vertical(g, p) || horizontal(g, p) || diagonal_pos(g, p) ||
            diagonal_neg(g, p));
}

outcome game_outcome(game* g) {
    if (win(g, WHITE) && win(g, BLACK)) {
        return DRAW;
    } else if (win(g, BLACK)) {
        return BLACK_WIN;
    } else if (win(g, WHITE)) {
        return WHITE_WIN;
    } else if (board_full(g)) {
        return DRAW;
    } else {
        return IN_PROGRESS;
    }
}

