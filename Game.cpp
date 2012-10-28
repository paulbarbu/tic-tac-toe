#include "Game.hpp"

Game::Game(unsigned int w, unsigned h, const std::string& t)
    : status_area_height(30), board(w, h-status_area_height),
    window(sf::VideoMode(w, h, 32), t, sf::Style::Close),
    input(window.GetInput()), human(1), ai(2, 1) {
    title = t;
    height = h;
}

/**
 * Main game loop
 */
void Game::Loop(){
    std::pair<unsigned int, unsigned int> pos;

    Start();

    while(window.IsOpened()){
        pos = HandleInput();

        if(board.Update(current_player->GetId(), pos.first, pos.second)
            && playing){
            DrawMove(pos.first, pos.second);

            if(current_player == &human){
                current_player = &ai;
            }
            else{
                current_player = &human;
            }

            DisplayCurrentPlayer();
        }

        CheckGameOver();

        window.Display();
    }
}

/**
 * Uses the status area of the window to display the player whose turn it is
 */
void Game::DisplayCurrentPlayer(){
    std::string text;
    if(current_player == &human){
        text = "Your turn!";
    }
    else{
        text = "Computer's turn!";
    }

    DisplayStatus(text);
}

/**
 * Set some default values and clear the screen
 */
void Game::Start(){
    board.Reset();

    window.Clear();

    SetFirstPlayer();
    playing = true;

    DrawGrid();
    DisplayCurrentPlayer();

    window.Display();
}

/**
 * Randomly choose the first player
 *
 * @return int the first player's id
 */
int Game::SetFirstPlayer(){
    if(probability(50)){
        current_player = &human;
    }
    else{
        current_player = &ai;
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
void Game::DrawMove(unsigned int row, unsigned int col){
    sf::Color color;
    sf::Shape sign;

    float thickness = sf::Randomizer::Random(4.f, 6.f);
    float scale_x = sf::Randomizer::Random(0.95f, 1.05f);
    float scale_y = sf::Randomizer::Random(0.95f, 1.05f);

    if(current_player == &human){
        color = sf::Color(239, 39, 93);

        //the O
        float x = 100 * col - 50 + sf::Randomizer::Random(-5.f, 5.f);
        float y = 100 * row - 50 + sf::Randomizer::Random(-5.f, 5.f);
        float radius = 40 - thickness;

        sign = sf::Shape::Circle(x, y, radius, color, thickness, color);
        sign.SetScale(scale_x, scale_y);
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
void Game::DrawGrid(){
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
std::pair<unsigned int, unsigned int> Game::HandleInput(){
    if(!playing && input.IsKeyDown(sf::Key::R)){
        Start();
    }

    while(window.GetEvent(event)){
        if(event.Type == sf::Event::Closed){
            window.Close();
        }
        else if(event.Key.Code == sf::Key::F5){
            sf::Image screen = window.Capture();
            screen.SaveToFile("tic-tac-toe.jpg");
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
void Game::CheckGameOver(){
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

void Game::DisplayStatus(std::string t){
    sf::Shape clear = sf::Shape::Rectangle(0, 300, 300, 330, sf::Color(0, 0, 0));
    window.Draw(clear);

    sf::String text(t);
    text.SetSize(15);
    text.SetColor(sf::Color(255, 255, 255));
    text.SetPosition(5, height - status_area_height);

    window.Draw(text);
}
