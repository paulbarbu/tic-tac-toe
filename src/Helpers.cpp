#include "Helpers.hpp"

/**
 * Returns true with a probability of p
 *
 * If passed 20 for example the probability to return true is 20%
 * Inspiration source: http://users.softlab.ntua.gr/~ttsiod/amIsick.html
 *
 * @param float p the probability
 *
 * @return bool there's a probability of p to return true, otherwise false
 */
bool probability(float p){
    return sf::Randomizer::Random(0, 100) < p;
}
