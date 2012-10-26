#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

class Board{
    public:
        Board(unsigned int w, unsigned int h, int e=0)
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
        std::pair<unsigned int, unsigned int> CoordToPos(unsigned int x,
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
        bool Update(int id, unsigned int row, unsigned int col){
            if(IsValidMove(row, col)){
                //Note: the vector is 0-indexed
                board[row-1][col-1] = id;

                return true;
            }

            return false;
        }

        int GetWinner(){
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

        unsigned int GetWidth(){
            return width;
        }

        unsigned int GetHeight(){
            return height;
        }

        std::vector< std::vector<int> > board;
    protected:
        /**
         * Check if the given position is within the board
         *
         * @param unsigned int row the row number that should be checked
         * @param unsigned int col the column number that should be checked
         *
         * @return bool true if the position is on the board, else false
         */
        bool IsValidRowCol(unsigned int row, unsigned int col){
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
        bool IsValidMove(unsigned int row, unsigned int col){
            //Note: the vector is 0-indexed
            if(IsValidRowCol(row, col) && board[row-1][col-1] == empty){
                return true;
            }

            return false;
        }

    private:
        unsigned int width, height;
        int empty;
};

class Player{
    public:
        Player(int i) : id(i) {}

        virtual std::pair<unsigned int, unsigned int> GetInput(sf::Event)=0;

        virtual int GetId(){
            return id;
        }

    protected:
        const int id;
};

class HumanPlayer : public Player {
    public:
        HumanPlayer(int id, const sf::Input &i, const Board &b)
            : Player(id), board(b) {}

        /**
         * Get input from the user using the mouse
         *
         * @param sf::Event event the event that is checked for the user's click
         *
         * @return std::pair<unsigned int, unsigned int> a position (row, col)
         * on the board if the user clicked, else some default position (0, 0)
         */
        std::pair<unsigned int, unsigned int> GetInput(sf::Event event){
            if(event.Type == sf::Event::MouseButtonReleased
                && event.MouseButton.Button == sf::Mouse::Left){
                return board.CoordToPos(event.MouseButton.X, event.MouseButton.Y);
            }

            return std::make_pair(0, 0);
        }

    private:
        const Board &board;
};

class AiPlayer : public Player{
    public:
        AiPlayer(int id) : Player(id) {}

        std::pair<unsigned int, unsigned int> GetInput(sf::Event){
            int row = sf::Randomizer::Random(1, 3);
            int col = sf::Randomizer::Random(1, 3);

            return std::make_pair(row, col);
        }
};

class Game{
    public:
        Game(unsigned int w, unsigned h, const std::string& t)
            : board(w, h), window(sf::VideoMode(w, h, 32), t),
            human(1, window.GetInput(), board), ai(2) {
            title = t;
        }

        /**
         * Draw a mark at the given position
         *
         * The position should be relative to the board, internally this position is
         * translated to the window's coordinates, also this method figures out
         * on its own the current player in order to draw the correct marker
         *
         * @param unsigned int row the row on the board to draw the marker at
         * @param unsigned int col the column on the board to draw the marker at
         */
        void DrawMove(unsigned int row, unsigned int col){
            //TODO: try and combine two shapes (for the X)
            sf::Color color;
            sf::Shape sign;

            float thickness = sf::Randomizer::Random(4.f, 6.f);

            if(current_player == &human){
                color = sf::Color(239, 39, 93);

                //the O
                float x = 100 * col - 50 + sf::Randomizer::Random(-10.f, 10.f);
                float y = 100 * row - 50 + sf::Randomizer::Random(-10.f, 10.f);
                float radius = 40 - thickness;

                sign = sf::Shape::Circle(x, y, radius, color, thickness, color);
                sign.EnableFill(false);
            }
            else{
                color = sf::Color(39, 239, 184);

                //the X is in fact two lines
                float x1 = 100 * (col-1) + sf::Randomizer::Random(3.f, 10.f);
                float y1 = 100 * (row-1) + sf::Randomizer::Random(3.f, 10.f);

                float x2 = 100 * col - sf::Randomizer::Random(3.f, 10.f);
                float y2 = 100 * row - sf::Randomizer::Random(3.f, 10.f);

                sign = sf::Shape::Line(x1, y1, x2, y2, thickness, color);
                window.Draw(sign);

                x1 = 100 * col - sf::Randomizer::Random(3.f, 10.f);
                y1 = 100 * (row-1) + sf::Randomizer::Random(3.f, 10.f);

                x2 = 100 * (col-1) + sf::Randomizer::Random(3.f, 10.f);
                y2 = 100 * row - sf::Randomizer::Random(3.f, 10.f);

                sign = sf::Shape::Line(x1, y1, x2, y2, thickness, color);
            }

            window.Draw(sign);
        }

        /**
         * Main game loop
         */
        void Loop(){
            std::pair<unsigned int, unsigned int> pos;
            current_player = &human;

            window.Clear();
            DrawGrid();

            while(window.IsOpened()){
                while(window.GetEvent(event)){
                    if(event.Type == sf::Event::Closed){
                        window.Close();
                    }
                    else{
                        pos = current_player->GetInput(event);
                    }
                }

                if(board.Update(current_player->GetId(), pos.first, pos.second)){
                    DrawMove(pos.first, pos.second);

                    if(current_player == &human){
                        current_player = &ai;
                    }
                    else{
                        current_player = &human;
                    }
                }

                window.Display();

                int winner_id = board.GetWinner();
                if(winner_id == human.GetId()){
                    std::cout<<"The human won!\n";

                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            std::cout<<board.board[i][j]<<" ";
                        }
                        std::cout<<"\n";
                    }

                    return;
                }
                else if(winner_id == ai.GetId()){
                    std::cout<<"The computer won!\n";
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            std::cout<<board.board[i][j]<<" ";
                        }
                        std::cout<<"\n";
                    }
                    return;
                }
                else if (winner_id < 0){
                    std::cout<<"It's a draw!\n";
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            std::cout<<board.board[i][j]<<" ";
                        }
                        std::cout<<"\n";
                    }
                    return;
                }

                pos = std::make_pair(0, 0);
            }
        }

    private:
        /**
         * Split to window into 9 squares where the X-es or O-s will fit.
         *
         * This splitting is done purely visual by drawing lines
         */
        void DrawGrid(){
            sf::Shape line;

            for(int i=1; i<=2; i++){
                line = sf::Shape::Line(board.GetWidth()*(i/3.f), 0,
                        board.GetWidth()*(i/3.f), board.GetHeight(),
                        3, sf::Color(255, 255, 255));
                window.Draw(line);

                line = sf::Shape::Line(0, board.GetHeight()*(i/3.f),
                        board.GetHeight(), board.GetHeight()*(i/3.f),
                        3, sf::Color(255, 255, 255));
                window.Draw(line);
            }
        }

        Board board;
        std::string title;
        sf::RenderWindow window;
        sf::Event event;
        Player *current_player;
        HumanPlayer human;
        AiPlayer ai;
};

int main(){
    Game game(300, 300, "Tic-tac-toe");
    game.Loop();
}
