#include <stdexcept>
#include <algorithm>
#include <sstream>
#include "game.h"
#include "widget.h"

using std::runtime_error;
using std::stringstream;
using std::endl;

Game::Game(const Map &map, const Config &config, int mode)
        : random(config.randomSeed),
          mode(mode),
          status({map,
                  config,
                  Right,
                  Right,
                  Alive,
                  0,
                  1,
                  2,
                  vector<vector<Point>>(map.width, vector<Point>(map.height, SpecialPoint::Empty)),
                  vector<vector<Point>>(map.width, vector<Point>(map.height, SpecialPoint::Empty)),
                  vector<Point>(),
                  map.spawnPoint,
                  map.spawnPoint
                 }) {
    for (auto &obstacle: map.obstacles) {
        status.map[obstacle.x][obstacle.y] = SpecialPoint::Obstacle;
    }

    for (auto portal: map.portals) {
        status.portal[portal[0].x][portal[0].y] = portal[1];
        status.portal[portal[1].x][portal[1].y] = portal[0];
    }

    status.map[map.spawnPoint.x][map.spawnPoint.y] = SpecialPoint::Head;
    if (mode == 0) {
        int tot = GenerateFood();
        for(int i = 1; i <= tot; ++i) Widget::UpdateTime(0);
    }
    else if (mode == 1) {
        while(Widget::GetCurrentAction() == 'F') {
            UpdateFood();
            Widget::NextAction();
        }
    }
    else {}
}

bool Game::ChangeDirection(Game::Direction direction) {
    // Cannot turn 180 degrees
    if (status.direction - direction == 2 || status.direction - direction == -2) {
        return false;
    }
    status.direction = direction;
    return true;
}

int Game::Step() {
    int tot = 0;
    auto &head = status.head;
    auto &tail = status.tail;
    auto &map = status.map;
    auto &portal = status.portal;
    auto &mapDefinition = status.mapDefinition;
    auto &direction = status.direction;
    auto &state = status.state;

    if (state == Dead && !Widget::mode) {
        throw runtime_error("The snake is dead");
    }

    // Calculate next head
    Point nextHead = head;

//    Right = 0,
//    Down = 1,
//    Left = 2,
//    Up = 3

    switch (direction) {
        case Right:
            nextHead.x++;
            break;
        case Down:
            nextHead.y++;
            break;
        case Left:
            nextHead.x--;
            break;
        case Up:
            nextHead.y--;
            break;
    }
    // Check if the snake hits the vertical border
    if (nextHead.x < 0 || nextHead.x >= mapDefinition.width) {
        // Hits the vertical wall
        if (mapDefinition.borderIsObstacle[2] || mapDefinition.borderIsObstacle[3]) {
            state = Dead;
            return 0;
        }

        // Teleport to the other side
        if (nextHead.x < 0) {
            nextHead.x = mapDefinition.width - 1;
        } else {
            nextHead.x = 0;
        }
    }

    // Hits the horizontal border
    if (nextHead.y < 0 || nextHead.y >= mapDefinition.height) {
        if (mapDefinition.borderIsObstacle[0] || mapDefinition.borderIsObstacle[1]) {
            state = Dead;
            return 0;
        }

        // Teleport to the other side
        if (nextHead.y < 0) {
            nextHead.y = mapDefinition.height - 1;
        } else {
            nextHead.y = 0;
        }
    }

    // Check if the snake hits a portal
    if (portal[nextHead.x][nextHead.y] != SpecialPoint::Empty) {
        nextHead = status.portal[nextHead.x][nextHead.y];
    }

    // Hits an obstacle
    if (map[nextHead.x][nextHead.y] == SpecialPoint::Obstacle) {
        state = Dead;
        return 0;
    }

    // Hits itself
    if (map[nextHead.x][nextHead.y].x != SpecialPoint::Empty.x
        && map[nextHead.x][nextHead.y].x != SpecialPoint::Food.x
        && !(nextHead == tail)) {
        state = Dead;
        return 0;
    }
    // Check if the snake eats a food
    if (map[nextHead.x][nextHead.y].x == SpecialPoint::Food.x) {
        status.score += map[nextHead.x][nextHead.y].y;
        status.desiredLength += map[nextHead.x][nextHead.y].y;
        status.foods.erase(find(status.foods.begin(), status.foods.end(), nextHead));
        if (mode == 0) tot = GenerateFood();
        else UpdateFood();
    }

    // Move the snake
    Point nextTail = map[tail.x][tail.y];

    // Lengthen the snake
    if (status.length < status.desiredLength) {
        ++status.length;
    }

        // Or go forward
    else {
        map[tail.x][tail.y] = SpecialPoint::Empty;
        tail = nextTail;
    }
    map[head.x][head.y] = nextHead;
    head = nextHead;
    map[head.x][head.y] = SpecialPoint::Head;
    return tot;
}

void Game::SetPreDirection(Direction direc) {
    status.preDirection = direc;
}

const Game::GameStatus &Game::GetStatus() const {
    return status;
}

int Game::GenerateFood() {
    int cnt = 0;
    int retries = 100;
    while (retries-- && (int) status.foods.size() < status.config.foodCount) {
        int x = (int) (random() % status.mapDefinition.width);
        int y = (int) (random() % status.mapDefinition.height);

        if (status.map[x][y] == SpecialPoint::Empty && status.portal[x][y] == SpecialPoint::Empty) {
            status.map[x][y] = SpecialPoint::Food;

            float s = (float) (random() % 10000) / 10000.0f;
            if ((s -= status.config.foodProbabilities[0]) < 0) {
                status.map[x][y].y = 1;
            } else if (s < status.config.foodProbabilities[1]) {
                status.map[x][y].y = 2;
            } else {
                status.map[x][y].y = 3;
            }
            ++cnt;
            Widget::UpdateRecordFood(x, y, status.map[x][y].y);
            status.foods.push_back({x, y});
        }
    }

    // Still not even 1 food
    if (status.foods.empty()) {
        for (int x = 0; x < status.mapDefinition.width; ++x) {
            for (int y = 0; y < status.mapDefinition.height; ++y) {
                if (status.map[x][y] == SpecialPoint::Empty) {
                    status.map[x][y] = SpecialPoint::Food;
                    ++cnt;
                    Widget::UpdateRecordFood(x, y, status.map[x][y].y);
                    status.foods.push_back({x, y});
                    return cnt;
                }
            }
        }
    }

    return cnt;
}

void Game::UpdateFood() {
    auto food = Widget::GetNextFood();
    int x = food.first.x, y = food.first.y;
    status.map[x][y].x = Game::SpecialPoint::Food.x;
    status.map[x][y].y = food.second;
    status.foods.push_back({x, y});
}

string Game::GetStatisticsString() const {
    stringstream ss;
    ss << "Score: " << status.score << ", Length: " << status.length << ", State: "
       << (status.state == Alive ? "Alive" : "Dead") << endl << endl;
    ss << (status.mapDefinition.borderIsObstacle[0] ? string(status.mapDefinition.width + 2, '#') : string(
            status.mapDefinition.width + 2, '-')) << endl;
    for (int y = 0; y < status.mapDefinition.height; ++y) {
        ss << (status.mapDefinition.borderIsObstacle[2] ? "#" : "|");
        for (int x = 0; x < status.mapDefinition.width; ++x) {
            if (status.map[x][y] == SpecialPoint::Empty) {
                if (status.portal[x][y] != SpecialPoint::Empty) {
                    ss << "P";
                } else {
                    ss << " ";
                }
            } else if (status.map[x][y] == SpecialPoint::Obstacle) {
                ss << "#";
            } else if (status.map[x][y] == SpecialPoint::Head) {
                ss << "O";
            } else if (status.map[x][y].x == SpecialPoint::Food.x) {
                ss << status.map[x][y].y;
            } else {
                ss << "o";
            }
        }
        ss << (status.mapDefinition.borderIsObstacle[3] ? "#" : "|");
        ss << endl;
    }

    ss << (status.mapDefinition.borderIsObstacle[1] ? string(status.mapDefinition.width + 2, '#') : string(
            status.mapDefinition.width + 2, '-')) << endl;
    ss << endl;

    return ss.str();
}

void Game::SetStatus(State state) {
    status.state = state;
}
