
VPATH = src
BUILD = build
BIN = bin
$(shell mkdir -p build)
$(shell mkdir -p bin)

CC = g++ 
CFLAGS  = -g -std=c++11
LIBS=-lsfml-graphics -lsfml-window -lsfml-system


G++_VER_LT48 := $(shell expr `$(CC) -dumpversion | cut -f1-2 -d.` \< 4.8 )
ifeq ("$(G++_VER_LT48)","1")
$(error old version of g++ not supported, upgrade to 4.8 or higher)
endif


default: bin/game


$(BIN)/game: $(BUILD)/main.o $(BUILD)/game.o $(BUILD)/food.o $(BUILD)/snake.o
	$(CC) $(CFLAGS) -o $(BIN)/game $(BUILD)/main.o $(BUILD)/game.o $(BUILD)/food.o $(BUILD)/snake.o $(LIBS)
	

$(BUILD)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@


