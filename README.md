# Tic-Tac-Toe Minimax

A small C++ implementation of Tic-Tac-Toe using the **minimax algorithm** to make optimal moves.

The program runs two AI players against each other and visualizes the game directly in the terminal using ANSI escape sequences.

## Features

* Minimax game-tree search
* Two-player AI simulation
* Detects wins, losses, and draws
* Prefers faster wins and slower losses
* Terminal-based board visualization
* Uses modern C++ features including:

  * Generic lambdas
  * Explicit object parameters (`this auto`)
  * `std::jthread`
  * `std::stop_token`
  * `std::print`

## Example

The board is displayed using:

* `O` for the AI
* `X` for the other player

The program automatically plays games continuously, resetting after each game.

## Building

A compiler with support for the required modern C++ features is needed.

For example:

```bash
g++ -std=c++23 -O2 main.cpp -o tic-tac-toe
```

Then run:

```bash
./tic-tac-toe
```

## How It Works

For every possible move, the minimax algorithm recursively explores the resulting game states.

* AI moves maximize the score.
* Human moves minimize the score.
* A win for the AI produces a positive score.
* A win for the opponent produces a negative score.
* A draw produces `0`.

The search depth is incorporated into the score, so the AI prefers winning sooner and delaying losses.

## Notes

This is intentionally a small, self-contained implementation intended as a demonstration of minimax and modern C++.
