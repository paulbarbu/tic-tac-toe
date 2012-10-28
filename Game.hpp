#ifndef GAME_HPP_GUARD
#define GAME_HPP_GUARD

#include <SFML/Graphics.hpp>

#include "Helpers.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "HumanPlayer.hpp"
#include "AiPlayer.hpp"

class Game{
    public:
        Game(unsigned int w, unsigned h, const std::string& t);
        void Loop();

    protected:
        void DisplayCurrentPlayer();
        void Start();
        int SetFirstPlayer();
        void DrawMove(unsigned int row, unsigned int col);
        void DrawGrid();
        std::pair<unsigned int, unsigned int> HandleInput();
        void CheckGameOver();
        void DisplayStatus(std::string t);

    private:
        unsigned int height;
        unsigned int status_area_height;
        Board board;
        std::string title;
        sf::RenderWindow window;
        const sf::Input &input;
        sf::Event event;
        Player *current_player;
        HumanPlayer human;
        AiPlayer ai;
        bool playing;
};

#endif
