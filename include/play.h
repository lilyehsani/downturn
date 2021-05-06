/* Lily Ehsani, lilyehsani
 * CS 152, Winter 2020
 * Project 2
 */

#ifndef PLAY_H
#define PLAY_H

#include "logic.h"

/* do_turn: executes a single turn based on given game and user input */
int do_turn(game* g);

/* print_outcome: prints an outcome message given an outcome */
void print_outcome(outcome o);

/* change_turn: changes the turn within the given game struct */
turn change_turn(game* g);

/* play_game: executes an entire game from start to finish given a
 * new game */
void play_game(game* g);


#endif /* PLAY_H */