# Snake Game

A terminal-based snake game implemented in C++ with minimal dependencies, using `thread` for non-blocking input and
supporting both Windows and Unix systems.

### Features

- **Cross-Platform:** Supports both Windows and Unix (Linux, macOS).
- **Real-Time Input:** Non-blocking input handled via multi-threading.
- **Colorful Gameplay:** Uses ANSI escape codes for colored snake body and fruit.
- **Score Tracking:** Tracks and displays the score in real-time.

### Controls

- Arrow keys to control the snake's direction:
    - **Up, Down, Left, Right** for respective directions.

### Game Mechanics

- The snake grows when it eats the fruit (`*`), and the game ends if the snake hits the screen edge or its own body.
- The goal is to achieve the highest score possible.

### Building and Running

1. Compile with:
   ```sh
   g++ snake_game.cpp -o snake_game -pthread
   ```
2. Run with:

    ```sh
    ./snake_game
    ```
    
### Note
- The game relies on ANSI escape codes for colored output, so it's best played on a compatible terminal.
