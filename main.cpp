#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>

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

        /**
         * Check for a winner on the board
         *
         * @return int the marker (a player's id) that has won, if it's a draw
         * -1 is returned or if there is no winner yet, returns 0
         */
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

        /**
         * Reset the board to its initial state
         */
        void Reset(){
            for(int i=0; i<board.capacity(); i++){
                std::fill(board[i].begin(), board[i].end(), empty);
            }
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

        virtual std::pair<unsigned int, unsigned int> GetInput(sf::Event, Board b)=0;

        virtual int GetId(){
            return id;
        }

    protected:
        const int id;
};

class HumanPlayer : public Player {
    public:
        HumanPlayer(int id, const Board &b) : Player(id), board(b) {}

        /**
         * Get input from the user using the mouse
         *
         * @param sf::Event event the event that is checked for the user's click
         *
         * @return std::pair<unsigned int, unsigned int> a position (row, col)
         * on the board if the user clicked, else some default position (0, 0)
         */
        std::pair<unsigned int, unsigned int> GetInput(sf::Event event, Board b){
            if(event.Type == sf::Event::MouseButtonReleased
                && event.MouseButton.Button == sf::Mouse::Left){
                return board.CoordToPos(event.MouseButton.X, event.MouseButton.Y);
            }

            return std::make_pair(0, 0);
        }

    private:
        const Board &board;
};

class AiPlayer : public Player {
    public:
        AiPlayer(int id) : Player(id) {}

        std::pair<unsigned int, unsigned int> GetInput(sf::Event, Board b){
            std::pair<unsigned int, unsigned int> move;
            int score = Minimax(id, b, 9, move);

            //std::cout<<score<<" "<<move.first<<" "<<move.second<<"\n";

            return move;
        }

    protected:
        int Minimax(int player, Board b, int depth, std::pair<unsigned int, unsigned int> &move){
            int best_score;
            int opponent = 1;

            int winner = b.GetWinner();
            if(depth <= 0 || winner != 0){
                if(winner == id){
                    return 1;
                }
                else if(winner == id+1){
                    return -1;
                }
                else{
                    return 0;
                }
            }

            if(player == id){
                best_score = -1;
            }
            else{
                best_score = 1;
            }

            for(int i=1; i<=3; i++){
                for(int j=1; j<=3; j++){
                    if(b.Update(id == player ? id : opponent, i, j)){
                        int score = Minimax(id == player ? opponent : id, b, depth-1, move);

                        if(player == id){
                            if(best_score < score){
                                best_score = score;
                                move = std::make_pair(i, j);
                            }
                        }
                        else{
                            if(best_score > score){
                                best_score = score;
                                move = std::make_pair(i, j);
                            }
                        }
                    }
                }
            }

            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    std::cout<<b.board[i][j]<<" ";
                }
                std::cout<<"\n";
            }
            std::cout<<"=========\n";

            return best_score;
        }
};

class Game{
    public:
        Game(unsigned int w, unsigned h, const std::string& t)
            : status_area_height(30), board(w, h-status_area_height),
            window(sf::VideoMode(w, h, 32), t, sf::Style::Close),
            human(1, board), ai(2) {
            title = t;
            height = h;
        }

        /**
         * Main game loop
         */
        void Loop(){
            std::pair<unsigned int, unsigned int> pos;

            Start();

            while(window.IsOpened()){
                pos = HandleInput();

                if(board.Update(current_player->GetId(), pos.first, pos.second)
                    && playing){
                    DrawMove(pos.first, pos.second);

                    if(current_player == &human){
                        current_player = &ai;
                        DisplayStatus("Computer's turn!");
                    }
                    else{
                        current_player = &human;
                        DisplayStatus("Your turn!");
                    }
                }

                window.Display();

                CheckGameOver();
            }
        }

    protected:
        /**
         * Set some default values and clear the screen
         */
        void Start(){
            window.Clear();

            SetFirstPlayer();
            playing = true;

            DrawGrid();

            board.Reset();
        }

        /**
         * Randomly choose the first player
         *
         * @return int the first player's id
         */
        int SetFirstPlayer(){
            if(probability(50)){
                current_player = &human;
                DisplayStatus("Your turn!");
            }
            else{
                current_player = &ai;
                DisplayStatus("Computer's turn!");
            }

            return current_player->GetId();
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

        /**
         * Handle everything input related
         */
        std::pair<unsigned int, unsigned int> HandleInput(){
            while(window.GetEvent(event)){
                if(event.Type == sf::Event::Closed){
                    window.Close();
                }
                else if(event.Key.Code == sf::Key::F5){
                    sf::Image screen = window.Capture();
                    screen.SaveToFile("tic-tac-toe.jpg");
                }
                else if(event.Key.Code == sf::Key::R && !playing){
                    Start();
                }
                else{
                    return current_player->GetInput(event, board);
                }
            }

            //safety net: the ai player doesn't need events to make a move
            if(current_player == &ai){
                return current_player->GetInput(event, board);
            }

            return std::make_pair(0, 0);
        }

        /**
         * Verify if the game is over
         */
        void CheckGameOver(){
            //don't check for a winner if the game is already over
            if(!playing){
                return;
            }

            std::map<int, std::string> messages;
            std::map<int, std::string>::iterator winner;

            messages[-1] = "It's a draw!";
            messages[human.GetId()] = "You won!";
            messages[ai.GetId()] = "The computer won!";


            int winner_id = board.GetWinner();

            winner = messages.find(winner_id);
            if(winner != messages.end()){
                DisplayStatus(winner->second + " Press r to play again!");
                playing = false;
            }
        }

        void DisplayStatus(std::string t){
            sf::Shape clear = sf::Shape::Rectangle(0, 300, 300, 330, sf::Color(0, 0, 0));
            window.Draw(clear);

            sf::String text(t);
            text.SetSize(15);
            text.SetColor(sf::Color(255, 255, 255));
            text.SetPosition(5, height - status_area_height);

            window.Draw(text);
        }

    private:
        unsigned int height;
        unsigned int status_area_height;
        Board board;
        std::string title;
        sf::RenderWindow window;
        sf::Event event;
        Player *current_player;
        HumanPlayer human;
        AiPlayer ai;
        bool playing;
};

//TODO: cross the winner's marks
////TODO: check the R thing at the end, sometimes it automatically resets
int main(){
    Game game(300, 330, "Tic-tac-toe");
    game.Loop();
}
