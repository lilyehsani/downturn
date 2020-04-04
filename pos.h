/* Lily Ehsani, lilyehsani
 * CS 152, Winter 2020
 * Project 2
 */

#ifndef POS_H
#define POS_H

struct pos {
    unsigned int r, c;
};

typedef struct pos pos;

/* make_pos: outputs a position with the given row and column values */
pos make_pos(unsigned int r, unsigned int c);

#endif /* POS_H */
