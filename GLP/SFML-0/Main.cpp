#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <thread>
#include <chrono>

#define WINDOW_X 600
#define WINDOW_Y 600
#define LINE_WIDTH 2 //Pixel width of lines in grid

//Add 2 to LINE_COUNT to account for the top border and bottom border of the window
#define LINE_COUNT 30//The total amount of lines in rows and column for the grid

#define ROW_SPACE (WINDOW_Y/LINE_COUNT)
#define COLUMN_SPACE (WINDOW_X/LINE_COUNT)

using namespace std;
using namespace sf;
Color pixelColor = sf::Color::Green;

int currentGrid[LINE_COUNT][LINE_COUNT];
bool gridCreated = false;
bool onScreen = false;
bool running = false;

/* --- Display grid lines to screen --- */
void drawGridLines(RenderWindow& window) {
    //Might be an inefficient way to display grid to screen, might need work for optimization

    RectangleShape rowLine(sf::Vector2f(WINDOW_X, LINE_WIDTH));
    RectangleShape columnLine(sf::Vector2f(LINE_WIDTH, WINDOW_Y));

    //Issue with formatting the last column and row, lines not showing up when drawing to screen
    for (int row = 0; row < LINE_COUNT; row++) {
        int offset = 0;

        //int offset = LINE_WIDTH ? row == 0 || row == LINE_COUNT: row > 0; //Possible solution
        //offset = LINE_WIDTH * 2 ? row == LINE_COUNT - 1 : row < LINE_COUNT - 1;
        rowLine.setPosition(sf::Vector2f(0, ROW_SPACE * row - offset));
        window.draw(rowLine);
    }
    for (int col = 0; col < LINE_COUNT; col++) {
        int offset = 0;

        //int offset = LINE_WIDTH ? col == 0 || col == LINE_COUNT: col > 0; //Possible Solution
        //offset = LINE_WIDTH * 2 ? col == LINE_COUNT - 1 : col < LINE_COUNT - 1;
        columnLine.setPosition(sf::Vector2f(COLUMN_SPACE * col - offset, 0));
        window.draw(columnLine);
    }
}

/* --- Print the current grid from currentGrid to the console and renderWindow --- */
void printGrid(RenderWindow& window) {

    //Print to Console
    cout << "\n\n\n\n\n\n\n" << endl;
    for (int row = 0; row < LINE_COUNT-1; row++) {
        for (int column = 0; column < LINE_COUNT-1; column++) {
            cout << currentGrid[column][row] << " "; //Flip row and column for formatting reasons
        }
        cout << endl;
    }

    for (int row = 0; row < LINE_COUNT - 1; row++) {
        for (int column = 0; column < LINE_COUNT - 1; column++) {
            if (currentGrid[column][row] == 1) {
                RectangleShape highlighted(sf::Vector2f(ROW_SPACE - LINE_WIDTH, COLUMN_SPACE - LINE_WIDTH));
                highlighted.setPosition(sf::Vector2f(column * ROW_SPACE + LINE_WIDTH, row * COLUMN_SPACE + LINE_WIDTH));
                highlighted.setFillColor(pixelColor);

                window.draw(highlighted);
            }
        }
    }
    //Draw to render window
}

/* --- display the current square the mouse is on based off the grid and window position --- */
int* getMousePos(int winX, int winY) {
    sf::Mouse mouse;
    int mousePos[2] = { 0, 0 };

    if (mouse.getPosition().x >= winX && mouse.getPosition().x <= winX + WINDOW_X &&
        mouse.getPosition().y >= winY && mouse.getPosition().y <= winY + WINDOW_Y) {

        mousePos[0] = ((mouse.getPosition().x - ceil(winX) - ROW_SPACE*1.5/2) / ROW_SPACE);
        mousePos[1] = ((mouse.getPosition().y - ceil(winY) - COLUMN_SPACE*2.5/2) / COLUMN_SPACE);
        onScreen = true;
    }
    return mousePos;
}

/* --- return a square in which the mouse is hovering over and change its color --- */
RectangleShape highlightSquare(int winX, int winY, RenderWindow& window) {
    int* mousePos = getMousePos(winX, winY);
    int mouseX = ROW_SPACE * mousePos[0];
    int mouseY = COLUMN_SPACE * mousePos[1];

    RectangleShape highlighted(sf::Vector2f(ROW_SPACE - LINE_WIDTH, COLUMN_SPACE - LINE_WIDTH));
    highlighted.setPosition(sf::Vector2f(mouseX + LINE_WIDTH, mouseY + LINE_WIDTH));
    
    /*
    if (onScreen)
        highlighted.setFillColor(sf::Color::Red);
    else if (!onScreen)
        highlighted.setFillColor(sf::Color::Black);
    */
    highlighted.setFillColor(pixelColor);

    window.draw(highlighted);
    return highlighted;
}

/* --- Completely clear the currentGrid array and set all values to 0 --- */
void clearGrid() {
    for (int row = 0; row < LINE_COUNT - 1; row++) {
        for (int column = 0; column < LINE_COUNT - 1; column++) {
            currentGrid[row][column] = 0;
        }
    }
}

/* --- Update the currentGrid array --- */
void updateGrid(int winX, int winY) {
    int* mousePos = getMousePos(winX, winY);
    currentGrid[mousePos[0]][mousePos[1]] = 1;
}

/* --- Make a move in the game --- */
void iterateNodes() {
    for (int row = 0; row < LINE_COUNT - 1; row++) {

    }
}

void playGame(RenderWindow& window) {
    iterateNodes();
    printGrid(window);
}
int main() {
    // create the window
    RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Conway's Game of Life");
    //window.setPosition(sf::Vector2i(0, 0));

    while (window.isOpen()) { // run the program as long as the window is open
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: //Window was closed
                window.close();
                break;
            case sf::Event::MouseLeft: //Mouse button was pressed
                updateGrid(window.getPosition().x, window.getPosition().y);
                printGrid(window);
                break;
            case sf::Event::KeyPressed: //Keyboard recived input
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    cout << "Starting Game" << endl;
                    running = true;
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                    cout << "Clearing Grid..." << endl;
                    clearGrid();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    running = false;
                }
            break;
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        drawGridLines(window);
        if (!gridCreated) { //Should run only once on the program start
            for (int row = 0; row < LINE_COUNT-1; row++) {
                for (int column = 0; column < LINE_COUNT-1; column++) {
                    currentGrid[row][column] = 0;
                }
            }
            gridCreated = true;
        } else { //Draw the board to the screen
            printGrid(window);
        }


        if (!running) {
            highlightSquare(window.getPosition().x, window.getPosition().y, window); 
        }
        else {
            playGame(window);
        }

        /*cout << "Clearing Memory..." << endl;
        for (int row = 0; row < ROWS; row++) {
            delete[] ptr[row];
        }
        delete[] ptr;
        ptr = 0;
        cout << "Ready... " << endl;
        */

        // end the current frame
        window.display();
        //std::chrono::milliseconds timespan(10);
        //std::this_thread::sleep_for(timespan);
    }

    return 0;
}