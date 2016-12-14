
PATH = src
BUILDDIR = build

CC = g++
CFLAGS  = -g
LIBS=-lsfml-graphics -lsfml-window -lsfml-system


default: game


game:  build/main.o build/game.o build/food.o build/snake.o
	$(CC) $(CFLAGS) -o game main.o game.o snake.o food.o $(LIBS)


build/game.o: src/game.cpp
	$(CC) $(CFLAGS) -c src/game.cpp

build/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp

build/food.o: src/food.cpp
	$(CC) $(CFLAGS) -c src/food.cpp

build/snake.o: src/snake.cpp
	$(CC) $(CFLAGS) -c src/snake.cpp


clean: 
	$(RM) count %(BIULDDIR)/*.o *~
