/* Lily Ehsani, lilyehsani
 * CS 152, Winter 2020
 * Project 2
 */

#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "../include/pos.h"
#include "../include/board.h"
#include "../include/logic.h"

/* ASCII Art */
void print_row()
{
    unsigned int i;
    for (i = 0; i < 21; i++) {
        printf("-");
    }
}

void print_welcome()
{
    printf("*");
    print_row();
    printf("*\n|");
    print_row();
    printf("|\n|--|  WELCOME  TO  |--|\n|--|D O W N T U R N|--|\n|");
    print_row();
    printf("|\n*");
    print_row();
    printf("*\n");
    printf("Black pieces look like this: *\nWhite pieces look like this: "
            "o\nDrop your pieces by typing in their column index and "
            "hitting enter.\nTurn the board clockwise by typing > and "
            "hitting enter.\nTurn the board counterclockwise by typing < "
            "and hitting enter.\nTo exit the game, type q and hit enter.\n");
}

void print_goodbye()
{
    printf("Game invented by Dr. Matthew Wachs; implemented by Lily Ehsani."
            "\nSpecial thanks to Professors Shaw and Wachs for inspiring me "
            "\nto pursue computer science!\n");
}

/* begin backend */

/* do_turn: executes a single turn based on given game and user input */
int do_turn(game* g) {
    /* assign place for input char */
    char ch;
    /* print which turn */
    switch (g->next) {
        case BLACK_NEXT:
            printf("Black:\n");
            break;
        case WHITE_NEXT:
            printf("White:\n");
            break;
    }
    /* assign inputted char */
    scanf("%c%*c", &ch);
    /* execute action based on inputted char */
    if (ch == 'q') {
        exit(0);
    }
    if (ch == '<') {
        twist(g, CCW);
        return 1;
    }
    if (ch == '>') {
        twist(g, CW);
        return 1;
    }
    if ((ch > 47 && ch < 58) || (ch > 64 && ch < 91) || 
        (ch > 96 && ch < 123)) { /* if char is a valid index */
        unsigned int col;
        int res;
        if (ch < 58) {
            col = ch - 48; /* column index for digits */
        } else if (ch > 64 && ch < 91) {
            col = ch - 55; /* column index for uppercase letters */
        } else {
            col = ch - 61; /* column index for lowercase letters */
        }
        if (col >= g->b->width) { /* check if col is in bounds */
            printf("Column out of bounds. Please input a column index "
                   "within the board.\n");
            return 0;
        }
        res = drop_piece(g, col);
        if (res == 0) { /* if dropping piece was unsuccessful */
            printf("Column full. Please input a column index with "
                    "available space.\n");
        }
        return res; /* true if piece was dropped, false otherwise */
    } else { /* if char is not a valid index or twist direction */
        printf("Invalid input. Please specify a column index or twist "
               "direction.\n");
        return 0;
    }
}

/* print_outcome: prints an outcome message given an outcome */
void print_outcome(outcome o) {
    switch (o) {
        case BLACK_WIN:
            printf("Black wins! Thanks for playing.\n");
            break;
        case WHITE_WIN:
            printf("White wins! Thanks for playing.\n");
            break;
        case DRAW:
            printf("Draw! Thanks for playing.\n");
            break;
        case IN_PROGRESS:
            printf("In progress. No winner yet.\n");
            break;
        default:
            fprintf(stderr, "print_outcome: invalid outcome");
            exit(1);
    }
}

/* change_turn: changes the turn within the given game struct */
turn change_turn(game* g) {
    if (g->next == BLACK_NEXT) {
        return WHITE_NEXT;
    } else {
        return BLACK_NEXT;
    }
}

/* play_game: executes an entire game from start to finish given a
 * new game */
void play_game(game* g) {
    print_welcome(); /* ASCII art and rules */
    printf("Get %d pieces in a row to win. Have fun!\n\n", g->run);
    while (game_outcome(g) == IN_PROGRESS) {
        board_show(g->b);
        int res = do_turn(g);
        while (res == 0) { /* repeats until turn is successful */
            board_show(g->b);
            res = do_turn(g);
        }
        g->next = change_turn(g);
    }
    board_show(g->b);
    print_outcome(game_outcome(g));
    print_goodbye();
}

/* main: takes in arguments for board width, height, run, and type,
 * and creates and plays a game with those parameters */
int main(int argc, char *argv[]) {
    unsigned int i, rep = -1, w = -1, h = -1, r = -1;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-w")) {
            w = atoi(argv[i + 1]);
        }
        if (!strcmp(argv[i], "-h")) {
            h = atoi(argv[i + 1]);
        }
        if (!strcmp(argv[i], "-r")) {
            r = atoi(argv[i + 1]);
        }
        if (!strcmp(argv[i], "-m")) {
            if (rep == -1) {
                rep = MATRIX;
            } else {
                fprintf(stderr, "Please specify either -m or -b, not both.\n");
                exit(1);
            }
        }
        if (!strcmp(argv[i], "-b")) {
            if (rep == -1) {
                rep = BITS;
            } else {
                fprintf(stderr, "Please specify either -m or -b, not both.\n");
                exit(1);
            }
        }
    }
    if (w == -1 || h == -1 || r == -1) {
        fprintf(stderr, "Missing input. Please specify -w, -h, and -r, "
                        "each followed by an unsigned int.\n");
        exit(1);
    }
    if (rep != MATRIX && rep != BITS) {
        fprintf(stderr, "Missing input. Please specify -b or -m for bits "
                        "or matrix board representation, respectively.\n");
        exit(1);
    }
    game* g = new_game(r, w, h, (rep == MATRIX)? MATRIX:BITS);
    play_game(g);
    return 0;
}
