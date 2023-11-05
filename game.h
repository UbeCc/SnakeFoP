#ifndef SNAKEFOP_GAME_H
#define SNAKEFOP_GAME_H

#include "configmanager.h"
#include "mapmanager.h"
#include <random>

class Game {
public:
    enum Direction {
        Right, Down, Left, Up
    };

    enum State {
        Alive, Dead
    };

private:
    default_random_engine random;
    int mode;
    struct GameStatus {
        const Map mapDefinition;
        const Config config;
        Direction direction;
        Direction nextDirection;
        State state;
        int score;
        int length;
        int desiredLength;

        /// Definition:
        /// If p points to (x, y), this point is occupied by the snake.
        /// If p[x] == -3, then p is the head of the snake.
        /// If p[x] == -4, then p is empty.
        /// If p[x] == -1, then p is an obstacle.
        /// p[x] == -2 was assigned to portals but deprecated.
        /// If p[x] == -5, then p is a food, p[y] being the score.
        ///
        /// 0 ---> x
        /// |
        /// y
        vector<vector<Point>> map;

        /// Definition:
        /// If p points to (x, y), the other side is (x, y).
        /// If p[x] == -4, then p is not a portal.
        vector<vector<Point>> portal;

        vector<Point> foods;

        Point head;
        Point tail;
    } status;

    class SpecialPoint {
    public:
        constexpr static Point Empty {-4, 0};
        constexpr static Point Obstacle {-1, 0};
        constexpr static Point Head {-3, 0};
        constexpr static Point Food {-5, 0};
    };

public:
    Game() = delete;
    Game(const Game &) = delete;
    Game(const Map &map, const Config &config, int _mode);

    /// @returns false if the direction is invalid
    bool ChangeDirection(Direction direction);

    /// @brief Move the snake one step forward.
    void Step(int);

    [[nodiscard]] const GameStatus &GetStatus() const;
    void SetStatus(State);
    [[nodiscard]] string GetStatisticsString() const;

private:
    void GenerateFood();
    void UpdateFood();
};

#endif //SNAKEFOP_GAME_H
