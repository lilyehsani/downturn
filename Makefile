play: pos.h pos.c board.h board.c logic.h logic.c play.c
	clang -Wall -g -O0 -o play pos.c board.c logic.c play.c

graphic: pos.h pos.c board.h board.c logic.h logic.c play.h play.c graphics.c
	clang -Wall -g -O0 -o graphic pos.c board.c logic.c play.c graphics.c
