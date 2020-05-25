TETRIS_game : Main.o GameStart1P.o GameStart2P.o
	gcc Main.o GameStart1P.o GameStart2P.o -lncursesw -o TETRIS_game

GameStart1P.o : GameStart1P.c
	gcc -std=c99 -c GameStart1P.c -lncursesw

GameStart2P.o : GameStart2P.c
	gcc -std=c99 -c GameStart2P.c -lncursesw	
	
Main.o : Main.c GameStart1P.h GameStart2P.h
	gcc -std=c99 -c Main.c	-lncursesw



clean:
	rm -rf $(TARGET)
	rm -rf *.o
	rm -rf .tmp_versions


distclean: clean
	orm -rf /nfs/$(TARGET)
