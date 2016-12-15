
VPATH = src
BUILDDIR = build

CC = g++
CFLAGS  = -g
LIBS=-lsfml-graphics -lsfml-window -lsfml-system


default: game


game:  build/main.o build/game.o build/food.o build/snake.o
	$(CC) $(CFLAGS) -o game main.o game.o snake.o food.o $(LIBS)

build/%.o: %.cpp
	$(CC) -c $< -o $@

clean: 
	$(RM) count %(BIULDDIR)/*.o *~
	
