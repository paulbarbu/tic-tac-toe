CXX = clang++
CXX_FLAGS = -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -std=c++0x

DEBUG_FLAGS = -g

SFML_LIBS = -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio

all:
	$(CXX) $(CXX_FLAGS) -o tic-tac-toe.exe main.cpp $(SFML_LIBS)

debug:
	$(CXX) $(CXX_FLAGS) $(DEBUG_FLAGS) -o tic-tac-toe.exe $@.cpp $(SFML_LIBS)
