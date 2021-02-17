play: include/pos.h src/pos.c include/board.h src/board.c include/logic.h src/logic.c src/play.c
	clang -Wall -g -O0 -o play src/pos.c src/board.c src/logic.c src/play.c

evidence: pos.h pos.c board.h board.c logic.h logic.c evidence.c
	clang -Wall -g -O0 -o evidence pos.c board.c logic.c evidence.c
