#include <bits/stdc++.h>
#include <thread> // to have a second thread for input

using namespace std;

#ifdef _WIN32

#include <conio.h>

int getCharInput() {
	return (int) getch();
}

#else
// UNIX specific non-buffered input
#include <termios.h>
#include <unistd.h>
#include <cstdio>
// gets non-buffered input character
int getCharInput() {
    struct termios oldattr{}, newattr{};
    tcgetattr(STDIN_FILENO, &oldattr); // Get current terminal attributes
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // Apply new attributes immediately
    const int ch = getchar();                   // Read a single character
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // Restore original attributes
    return ch;
}
#endif

// clears the screen
void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

// gets arrow input
int getKeyboardInput() {
#ifndef _WIN32
    getCharInput();
#endif
    getCharInput();
    return getCharInput();
}

#define FOOD '*'
#define BODY '+'
#define HEAD 'O'
#define EDGE '#'
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// keyboard arrow keys
enum key {
#ifdef _WIN32
	up = 72, left = 75, right = 77, down = 80
#else
    up = 65, down, right, left
#endif
};

// represents a character coordinates on the screen
struct coordinate {
    int x;
    int y;

    coordinate(const int a, const int b) : x(a), y(b) {}

    bool operator==(const coordinate& other) const {
        return x == other.x and y == other.y;
    }
};

deque<coordinate> snake;
int foodX, foodY; // food position
int direction;    // snake direction
bool isActive;
auto sleepTime = 0.13s;
int score;

// raplaces a character on the screen
void replace(coordinate cor, const char c) {
    auto& [a, b] = cor;
    cout << "\033[" << a << ";" << b << "H" << c << flush;
}

void print() {
    cout << "\033[1;32m"; // colors snake body green
    for (const auto& coordinate : snake)
        replace(coordinate, BODY); // color reset
    cout << "\033[0m" << flush;
    replace(snake.back(), HEAD);
}

void getDir() {
    while (isActive)
        direction = getKeyboardInput();
}

void drawFruit(coordinate cor, char c) {
    cout << "\033[1;31m"; // colors fruit red
    replace({cor.x, cor.y}, FOOD);
    cout << "\033[0m" << flush;
}

void gameLoop() {
    // second thread to get input without interrupting the main thread
    thread inputDir(getDir);

    while (true) {
        print(); // prints the snake and score changes

        // creating new head based on the current head
        coordinate newHead = snake.back();
        auto& [x, y] = newHead;

        // wait time
        this_thread::sleep_for(sleepTime);

        // changes direction based on arrow input
        switch (direction) {
            case key::up:
                x -= 1;
                break;
            case key::down:
                x += 1;
                break;
            case key::right:
                y += 1;
                break;
            case key::left:
                y -= 1;
                break;
            default:
                cerr << "error"; // wrong input
        }

        // checks for game end when snake touches edges or hits itself
        if (x <= 1 or x >= SCREEN_HEIGHT
            or y <= 1 or y >= SCREEN_WIDTH - 1
            or find(snake.begin(), snake.end(), newHead) != snake.end()) {
            const string over = "game over!";
            replace({SCREEN_HEIGHT + 1, (SCREEN_WIDTH / 2) - static_cast<int>(over.length()) + 1}, ' ');
            cout << over << endl;

            isActive = false;
            inputDir.detach(); // stops the input thread
            break;
        }

        // adds the new head
        snake.emplace_back(newHead);

        // snake eats the fruit
        if (x == foodX and y == foodY) {
            ++score;
            replace({SCREEN_HEIGHT, 11}, ' '); // updating score
            cout << score;

            auto generateRandomNumber = [](const int min, const int max) {
                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
                return static_cast<int>(dist6(rng));
            };

            // new random food position
            foodX = generateRandomNumber(2, SCREEN_HEIGHT - 1);
            foodY = generateRandomNumber(2, SCREEN_WIDTH - 1);

            drawFruit({foodX, foodY}, FOOD);
        } else {
            replace(snake.front(), ' ');
            snake.pop_front(); // removes the tail
        }
    }
}

void setup() {
    foodX = SCREEN_HEIGHT / 2;
    foodY = SCREEN_WIDTH / 2;
    isActive = true;
    score = 0;

    // initializing snake direction
    direction = key::right;

    // initial snake
    snake.emplace_back(2, 2);
    snake.emplace_back(2, 3);
    snake.emplace_back(2, 4);

    // initail screen clear
    clearScreen();

    // draws the 4 edges
    for (int i = 1; i < SCREEN_WIDTH - 2; i += 2)
        replace({1, i}, EDGE);

    for (int i = 1; i < SCREEN_WIDTH; i += 2)
        replace({SCREEN_HEIGHT, i}, EDGE);

    for (int i = 1; i < SCREEN_HEIGHT; ++i)
        replace({i, 1}, EDGE);

    for (int i = 1; i < SCREEN_HEIGHT; ++i)
        replace({i, SCREEN_WIDTH - 1}, EDGE);

    // drawing the fruit for the first time
    drawFruit({foodX, foodY}, FOOD);

    // draws score
    replace({SCREEN_HEIGHT, 4}, ' ');
    cout << "Score: " << score;

    // removes an extra character after score
    replace({SCREEN_HEIGHT, 13}, ' ');
}

int main() {
    cout << "\033[?25l"; // hides the cursor
    clearScreen();

    cout << R"(
 ____              _         ____
/ ___| _ __   __ _| | _____ / ___| __ _ _ __ ___   ___
\___ \| '_ \ / _` | |/ / _ \ |  _ / _` | '_ ` _ \ / _ \
 ___) | | | | (_| |   <  __/ |_| | (_| | | | | | |  __/
|____/|_| |_|\__,_|_|\_\___|\____|\__,_|_| |_| |_|\___|

)" << "Press any key to start!";

    getCharInput(); // waits for input to start the game

    setup();
    gameLoop();

    //	 FREE PALESTINE
}
