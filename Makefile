graphic: pos.h pos.c board.h board.c logic.h logic.c graphics.c
	clang -I /usr/include/SDL -Wall -g -O0 -o graphic pos.c board.c logic.c graphics.c -lSDL2