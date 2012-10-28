#include "Board.hpp"

Board::Board(unsigned int w, unsigned int h, int e)
            : board(3, std::vector<int>(3, e)) {
    width = w;
    height = h;
    empty = e;
}

/**
 * Translate a point's coordinates on the 3x3 grid to a cell position
 *
 * @param unsigned int x the X axis coordinate of the point
 * @param unsigned int y the Y axis coordinate of the point
 *
 * @return std::pair<unsigned int, unsigned int> the point's row and
 * column number on the board
 */
std::pair<unsigned int, unsigned int> Board::CoordToPos(unsigned int x,
        unsigned int y) const {
    int col, row;

    for(int i=1; i<=3; i++){
        //check the column clicked
        if((i-1) * (width/3) <= x && x <= i * (width/3)){
            col = i;
        }

        //check the row clicked
        if((i-1) * (width/3) <= y && y <= i * (width/3)){
            row = i;
        }
    }

    return std::pair<unsigned int, unsigned int>(row, col);
}

/**
 * Mark a position on the board with the player's id
 *
 * @param int id the marker to put on the board( the player's id)
 * @param unsigned int row the row on the board to be marked
 * @param unsigned int row the column on the board to be marked
 *
 * @return bool if the position was marked, false is returned of the
 * give row or column is not on the board or if the given position is
 * already marked
 */
bool Board::Update(int id, unsigned int row, unsigned int col){
    if(IsValidMove(row, col)){
        //Note: the vector is 0-indexed
        board[row-1][col-1] = id;

        return true;
    }

    return false;
}

/**
 * Check for a winner on the board
 *
 * @return int the marker (a player's id) that has won, if it's a draw
 * -1 is returned or if there is no winner yet, returns 0
 */
int Board::GetWinner(){
    //diagonal checking makes sense only if the middle was marked by a
    //player
    if(board[1][1] != empty){
        if(board[0][0] == board[1][1] && board[1][1] == board[2][2]
            && board[0][0] == board[2][2]){
            return board[0][0]; //first diagonal
        }
        else if(board[0][2] == board[1][1] && board[1][1] == board[2][0]
                && board[0][2] == board[2][0]){
            return board[0][2]; //second diagonal
        }
    }

    for(int i=0; i<board.capacity(); i++){
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i]
            && board[0][i] == board[2][i] && board[0][i] != empty){
            return board[0][i]; //a column
        }
        else if(board[i][0] == board[i][1] && board[i][1] == board[i][2]
            && board[i][0] == board[i][2] && board[i][0] != empty){
            return board[i][0]; //a row
        }
    }

    //check for draw
    for(int i=0; i<board.capacity(); i++){
        for(int j=0; j<board.capacity(); j++){
            if(board[i][j] == empty){
                //if the the board is not yet filled by player
                //markers (id's) and there is no winner then the game
                //should contine
                return empty;
            }
        }
    }

    //indeed it's a draw because the board is filled by markers but
    //there's no winner
    return -1;
}

/**
 * Reset the whole board to its initial state
 */
void Board::Reset(){
    for(int i=0; i<board.capacity(); i++){
        std::fill(board[i].begin(), board[i].end(), empty);
    }
}

/**
 * Reset a position on the board to it's initial state
 *
 * @param unsigned int row the row of the position
 * @param unsigned int col the column of the position
 *
 * @return true iof the reset was successful, else false
 */
bool Board::Reset(unsigned int row, unsigned int col){
    if(IsValidRowCol(row, col)){
        //Note: the vector is 0-indexed
        board[row-1][col-1] = empty;
        return true;
    }

    return false;
}

std::vector< std::pair<unsigned int, unsigned int> > Board::GetPossibleMoves(){
    std::vector< std::pair<unsigned int, unsigned int> > x;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(board[i][j] == empty){
                x.push_back(std::make_pair(i+1, j+1));
            }
        }
    }

    return x;
}

/**
 * Check if the given position is within the board
 *
 * @param unsigned int row the row number that should be checked
 * @param unsigned int col the column number that should be checked
 *
 * @return bool true if the position is on the board, else false
 */
bool Board::IsValidRowCol(unsigned int row, unsigned int col){
    if(row > 0 && col > 0 &&  row <= board.capacity() &&
        col <= board[0].capacity()){
        return true;
    }

    return false;
}

/**
 * Validates a possible move into a position on the board
 *
 * @param unsigned int row the row where a move will be made
 * @param unsigned int col the column where the move will be made
 *
 * @return true if the position is valid, otherwise false
 */
bool Board::IsValidMove(unsigned int row, unsigned int col){
    //Note: the vector is 0-indexed
    if(IsValidRowCol(row, col) && board[row-1][col-1] == empty){
        return true;
    }

    return false;
}
