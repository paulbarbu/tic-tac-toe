CXX = clang++
CXX_FLAGS = -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -std=c++0x

DEBUG_FLAGS = -g

SFML_LIBS = -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio

APP_NAME = tic-tac-toe.exe

SRC_FILES = main.cpp Game.cpp Board.cpp HumanPlayer.cpp AiPlayer.cpp Helpers.cpp

executable:
	$(CXX) $(CXX_FLAGS) -O3 -o $(APP_NAME) $(SRC_FILES) $(SFML_LIBS)

debug:
	$(CXX) $(CXX_FLAGS) $(DEBUG_FLAGS) -o $(APP_NAME) $(SRC_FILES) $(SFML_LIBS)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf *.o *.exe
