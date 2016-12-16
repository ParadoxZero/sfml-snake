
VPATH = src
BUILDDIR = build
$(shell mkdir -p build)
$(shell mkdir -p bin)
CC = g++ 
CFLAGS  = -g -std=c++11
LIBS=-lsfml-graphics -lsfml-window -lsfml-system


default: bin/game


bin/game:  build/main.o build/game.o build/food.o build/snake.o
	$(CC) $(CFLAGS) -o bin/game build/main.o build/game.o build/food.o build/snake.o $(LIBS)

build/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

	
