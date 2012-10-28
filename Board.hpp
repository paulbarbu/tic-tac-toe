#ifndef BOARD_HPP_GUARD
#define BOARD_HPP_GUARD

#include <vector>

class Board{
    public:
        Board(unsigned int w, unsigned int h, int e=0);
        bool Update(int id, unsigned int row, unsigned int col);
        int GetWinner();
        void Reset();
        bool Reset(unsigned int row, unsigned int col);
        std::vector< std::pair<unsigned int, unsigned int> > GetPossibleMoves();
        std::pair<unsigned int, unsigned int> CoordToPos(unsigned int x,
                unsigned int y) const;

        unsigned int GetWidth(){
            return width;
        }

        unsigned int GetHeight(){
            return height;
        }

    protected:
        bool IsValidRowCol(unsigned int row, unsigned int col);
        bool IsValidMove(unsigned int row, unsigned int col);

    private:
        unsigned int width, height;
        int empty;
        std::vector< std::vector<int> > board;
};

#endif
