#include <iostream>
#include <print>
#include <initializer_list>
#include <chrono>
#include <thread>
#include <stop_token>

template <typename T>
bool AllTheSame(std::initializer_list<T> data)
{
    for (const T& value : data)
        if (value != *data.begin())
            return false;

    return true;
}

int main()
{
    enum class CELL{EMPTY, AI, HUMAN};

    auto minimax = [&](this const auto& self, CELL(&board)[3][3], CELL currentPlayer, int depth) {
        for (int row = 0; row < 3; ++row)
            if ((board[row][0] != CELL::EMPTY) && AllTheSame({board[row][0], board[row][1], board[row][2]}))
                return (board[row][0] == CELL::AI) ? depth : -depth;

        for (int col = 0; col < 3; ++col)
            if ((board[0][col] != CELL::EMPTY) && AllTheSame({board[0][col], board[1][col], board[2][col]}))
                return (board[0][col] == CELL::AI) ? depth : -depth;

        if ((board[0][0] != CELL::EMPTY) && AllTheSame({board[0][0], board[1][1], board[2][2]}))
            return (board[0][0] == CELL::AI) ? depth : -depth;

        if ((board[0][2] != CELL::EMPTY) && AllTheSame({board[0][2], board[1][1], board[2][0]}))
            return (board[0][2] == CELL::AI) ? depth : -depth;

        auto cmp = [currentPlayer](int score, int bestScore) {
            if (currentPlayer == CELL::AI)
                return score > bestScore;
            else
                return score < bestScore;
        };

        const CELL nextPlayer = (currentPlayer == CELL::AI) ? CELL::HUMAN : CELL::AI;
        int bestScore = 0;
        bool foundBestScore = false;

        for (int row = 0; row < 3; ++row)
            for (int col = 0; col < 3; ++col)
                if (board[row][col] == CELL::EMPTY)
                {
                    board[row][col] = currentPlayer;

                    if (const int score = self(board, nextPlayer, depth + 1);
                        !foundBestScore || cmp(score, bestScore))
                    {
                        bestScore = score;
                        foundBestScore = true;
                    }

                    board[row][col] = CELL::EMPTY;
                }

        return foundBestScore ? bestScore : 0;
    };

    std::print("\033[?25l");

    {
        std::jthread thr{[&](std::stop_token st) {
            CELL currentPlayer = CELL::AI;

            while (!st.stop_requested())
            {
                CELL board[3][3] = {
                    {CELL::EMPTY, CELL::EMPTY, CELL::EMPTY},
                    {CELL::EMPTY, CELL::EMPTY, CELL::EMPTY},
                    {CELL::EMPTY, CELL::EMPTY, CELL::EMPTY}
                };

                for (int depth = 0; depth < 10; ++depth)
                {
                    // visualize board:
                    std::print("\033[2J");
                    std::print("\033[0H");
                    for (int row = 0; row < 3; ++row)
                    {
                        for (int col = 0; col < 3; ++col)
                            switch (board[row][col])
                            {
                                case CELL::AI:
                                    std::print("\033[38;2;255;0;0mO");
                                    break;
                                case CELL::HUMAN:
                                    std::print("\033[38;2;0;255;255mX");
                                    break;
                                case CELL::EMPTY:
                                    std::print(" ");
                                    break;
                            }

                        std::println();
                    }
                    std::cout << std::flush;

                    // end game if necessary:
                    bool gameOver = depth == 9;

                    for (int row = 0; row < 3; ++row)
                        if ((board[row][0] != CELL::EMPTY) && AllTheSame({board[row][0], board[row][1], board[row][2]}))
                        {
                            gameOver = true;
                            break;
                        }

                    for (int col = 0; col < 3; ++col)
                        if ((board[0][col] != CELL::EMPTY) && AllTheSame({board[0][col], board[1][col], board[2][col]}))
                        {
                            gameOver = true;
                            break;
                        }

                    if ((board[0][0] != CELL::EMPTY) && AllTheSame({board[0][0], board[1][1], board[2][2]}))
                        gameOver = true;
                    else if ((board[0][2] != CELL::EMPTY) && AllTheSame({board[0][2], board[1][1], board[2][0]}))
                        gameOver = true;

                    if (gameOver)
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        break;
                    }

                    // make move:
                    auto cmp = [currentPlayer](int score, int bestScore) {
                        if (currentPlayer == CELL::AI)
                            return score > bestScore;
                        else
                            return score < bestScore;
                    };

                    const CELL nextPlayer = (currentPlayer == CELL::AI) ? CELL::HUMAN : CELL::AI;
                    int bestScore = (currentPlayer == CELL::AI) ? -100 : 100;
                    int selectedRow = 0,
                        selectedCol = 0;

                    for (int row = 0; row < 3; ++row)
                        for (int col = 0; col < 3; ++col)
                            if (board[row][col] == CELL::EMPTY)
                            {
                                board[row][col] = currentPlayer;

                                if (const int score = minimax(board, nextPlayer, depth + 1); cmp(score, bestScore))
                                {
                                    selectedRow = row;
                                    selectedCol = col;
                                    bestScore = score;
                                }

                                board[row][col] = CELL::EMPTY;
                            }

                    board[selectedRow][selectedCol] = currentPlayer;
                    currentPlayer = nextPlayer;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
            }
        }};
        std::cin.get();
    }

    std::print("\033[?25h");
    std::print("\033[0m");
}