/* Lily Ehsani, lilyehsani
 * CS 152, Winter 2020
 * Project 2
 */

#ifndef PLAY_H
#define PLAY_H

#include "logic.h"

void print_row();

void print_welcome();

void print_goodbye();

int do_turn(game* g);

void print_outcome(outcome o);

turn change_turn(game* g);

void play_game(game* g);