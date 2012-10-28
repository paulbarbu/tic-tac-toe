#include "HumanPlayer.hpp"

HumanPlayer::HumanPlayer(int id) : Player(id) {
}

/**
 * Get input from the user using the mouse
 *
 * @param sf::Event event the event that is checked for the user's click
 * @param Board b the current board of the game
 *
 * @return std::pair<unsigned int, unsigned int> a position (row, col)
 * on the board if the user clicked, else some default position (0, 0)
 */
std::pair<unsigned int, unsigned int> HumanPlayer::GetInput(sf::Event event,
        Board b){
    if(event.Type == sf::Event::MouseButtonReleased
        && event.MouseButton.Button == sf::Mouse::Left){
        return b.CoordToPos(event.MouseButton.X, event.MouseButton.Y);
    }

    return std::make_pair(0, 0);
}
