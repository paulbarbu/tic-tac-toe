#ifndef HUMANPLAYER_HPP_GUARD
#define HUMANPLAYER_HPP_GUARD

#include <SFML/Window.hpp>

#include "Player.hpp"

class HumanPlayer : public Player {
    public:
        HumanPlayer(int id);
        std::pair<unsigned int, unsigned int> GetInput(sf::Event event, Board b);
};

#endif
