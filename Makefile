
VPATH = src
BUILDDIR = build
$(shell mkdir -p build)
$(shell mkdir -p bin)
CC = g++4.8 -std=c++11
CFLAGS  = -g 
LIBS=-lsfml-graphics -lsfml-window -lsfml-system


default: game


bin/game:  build/main.o build/game.o build/food.o build/snake.o
	$(CC) $(CFLAGS) -o game build/main.o build/game.o build/food.o build/snake.o $(LIBS)

build/%.o: %.cpp
	$(CC) -c $< -o $@

clean: 
	rm game (BIULDDIR)/*.o 
	
