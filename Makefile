
VPATH = src
BUILD = build
BIN = bin
$(shell mkdir -p build)
$(shell mkdir -p bin)
CC = g++ 
CFLAGS  = -g -std=c++11
LIBS=-lsfml-graphics -lsfml-window -lsfml-system

G++_VER_LT48 := $(shell echo `g++ -dumpversion | cut -f1-3 -d.` \< 4.8 | bc )
ifeq ($(G++_VER_LT48),1)
	$(error older version of g++ please update to 4.8 or greater)
endif

default: bin/game


$(BIN)/game:  $(BUILD)/main.o $(BUILD)/game.o $(BUILD)/food.o $(BUILD)/snake.o
	$(CC) $(CFLAGS) -o $(BIN)/game $(BUILD)/main.o $(BUILD)/game.o $(BUILD)/food.o $(BUILD)/snake.o $(LIBS)

$(BUILD)/%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@


