#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class IPlayer{
    public:
        virtual void Move()=0;
    protected:
        //X or O
        sf::Shape sign;
};

class Game{
    public:
        Game(unsigned int w, unsigned h, const std::string& t)
            : window(sf::VideoMode(w, h, 32), t), input(window.GetInput()){
            width = w;
            height = h;
            title = t;
        }

        void Loop(){
            while(window.IsOpened()){
                while(window.GetEvent(event)){
                    if(event.Type == sf::Event::Closed){
                        window.Close();
                    }
                    else if(event.Type == sf::Event::MouseButtonReleased
                            && event.MouseButton.Button == sf::Mouse::Left){
                        std::cout<<CoordToCell(event.MouseButton.X, event.MouseButton.Y)<<"\n";
                        //TODO: if my turn, paul.GetInput() (will use board.CoordToCell internally), isValidMove(), game.DrawMove(row, col) (by
                        //ctor paul draws on an window), game.UpdateInternalBoard()
                        //
                        //AI will have an overloaded std::pair<int, int> ai.GetInput() (this is minimax), isValidMove(), game.DrawMove(), game.UpdateInternalBoard()
                        //board + game => composition
                        //board + player => aggregation
                    }
                }

                window.Clear();

                DrawGrid();

                window.Display();
            }
        }

    private:
        /**
         * Translate a point's coordinates on the 3x3 grid to a cell position
         *
         * @param unsigned int x the X axis coordinate of the point
         * @param unsigned int y the Y axis coordinate of the point
         *
         * @return int the point's cell number
         */
        int CoordToCell(unsigned int x, unsigned int y){
            int col, row;
            int cells[3][3] = {
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9},
            };

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

            //by combining the row and column we get the cell number
            return cells[row-1][col-1];
        }

        /**
         * Split to window into 9 squares where the X-es or O-s will fit.
         *
         * This splitting is done purely visual by drawing lines
         */
        void DrawGrid(){
            sf::Shape line;

            for(int i=1; i<=2; i++){
                line = sf::Shape::Line(width*(i/3.f), 0,
                        width*(i/3.f), height,
                        3, sf::Color(255, 255, 255));
                window.Draw(line);

                line = sf::Shape::Line(0, height*(i/3.f),
                        height, height*(i/3.f),
                        3, sf::Color(255, 255, 255));
                window.Draw(line);
            }
        }

        std::string title;
        unsigned int width, height;
        sf::RenderWindow window;
        sf::Event event;
        const sf::Input &input;
};

int main(){
    Game game(300, 300, "Tic-tac-toe");
    game.Loop();
}
