#ifndef PLAYER_HPP_GUARD
#define PLAYER_HPP_GUARD

#include "Board.hpp"

class Player{
    public:
        Player(int i) : id(i) {}
        virtual std::pair<unsigned int, unsigned int> GetInput(sf::Event,
                Board b)=0;

        virtual int GetId(){
            return id;
        }

    protected:
        const int id;
};

#endif
