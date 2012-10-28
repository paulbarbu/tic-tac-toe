#ifndef AIPLAYER_HPP_GUARD
#define AIPLAYER_HPP_GUARD

#include <SFML/Window.hpp>

#include "Player.hpp"

class AiPlayer : public Player {
    public:
        AiPlayer(int id, int o_id);
        std::pair<unsigned int, unsigned int> GetInput(sf::Event, Board b);

    protected:
        int GetScore(int winner);
        std::pair<int, std::pair<unsigned int, unsigned int> > Max(Board b);
        std::pair<int, std::pair<unsigned int, unsigned int> > Min(Board b);
        std::pair<unsigned int, unsigned int> Minimax(Board b);

    private:
        int opponent_id;
};

#endif
