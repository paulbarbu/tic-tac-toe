#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>

class Board{
    public:
        Board(unsigned int w, unsigned int h, int f=0)
            : board(3, std::vector<int>(3, f)) {
            width = w;
            height = h;
            fill = f;
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

        void Update(int id, unsigned int row, unsigned int col){
            //Note: the vector is 0-indexed
            board[row-1][col-1] = id;
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
            if(board[row-1][col-1] == fill){
                return true;
            }

            return false;
        }

        int GetWinner(){
            return 0;//TODO
        }

        unsigned int GetWidth(){
            return width;
        }

        unsigned int GetHeight(){
            return height;
        }

    private:
        unsigned int width, height;
        std::vector< std::vector<int> > board;
        int fill;
};

class Player{
    public:
        Player(int i) : id(i) {}

        virtual std::pair<unsigned int, unsigned int> GetInput()=0;

        virtual int GetId(){
            return id;
        }

    protected:
        const int id;
        //X or O
        sf::Shape sign;
};

class HumanPlayer : public Player {
    public:
        HumanPlayer(int id, const sf::Input &i, const Board &b)
            : Player(id), board(b), input(i) {}

        std::pair<unsigned int, unsigned int> GetInput(){
            //TODO: ask about the continous click spray
            if(input.IsMouseButtonDown(sf::Mouse::Left)){
                return board.CoordToPos(input.GetMouseX(), input.GetMouseY());
            }

            return std::make_pair(0, 0);
        }

    private:
        const Board &board;
        const sf::Input &input;
};

class AiPlayer : public Player{
    public:
        AiPlayer(int id) : Player(id) {}

        std::pair<unsigned int, unsigned int> GetInput(){
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

        void DrawMove(unsigned int row, unsigned int col){
            //TODO: use current_player->Shape to draw the shape
        }

        //TODO: if my turn, paul.GetInput() (will use board.CoordToCell internally), isValidMove(), game.DrawMove(row, col),
        // game.UpdateInternalBoard()
        //
        //AI will have an overloaded std::pair<int, int> ai.GetInput() (this is minimax), isValidMove(), game.DrawMove(), game.UpdateInternalBoard()
        //board + game => composition
        //board + player => aggregation
        void Loop(){
            std::pair<unsigned int, unsigned int> pos;
            current_player = &human;

            while(window.IsOpened()){
                while(window.GetEvent(event)){
                    if(event.Type == sf::Event::Closed){
                        window.Close();
                    }
                    else if(event.Type == sf::Event::MouseButtonReleased
                            && event.MouseButton.Button == sf::Mouse::Left
                            && current_player == &human){ //human's turn
                        pos = board.CoordToPos(event.MouseButton.X, event.MouseButton.Y);
                        std::cout<<pos.first<<" "<<pos.second<<"\n";
                    }
                }

                if(current_player == &ai){//TODO: get rid of this after properly getting input
                    pos = current_player->GetInput();
                }

                if(pos.first != 0 && pos.second != 0){
                    if(board.IsValidMove(pos.first, pos.second)){
                        DrawMove(pos.first, pos.second);
                        board.Update(current_player->GetId(), pos.first, pos.second);

                        std::cout<<"valid\n";

                        if(current_player == &human){
                            current_player = &ai;
                            std::cout<<"AI's move!\n";
                        }
                        else{
                            current_player = &human;
                            std::cout<<"Your move!\n";
                        }
                    }
                    else{
                        std::cout<<"invalid\n";
                    }
                }

                int winner_id = board.GetWinner();
                //TODO: Check for game over

                window.Clear();
                DrawGrid();
                window.Display();

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
