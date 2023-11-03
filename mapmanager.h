#ifndef SNAKEFOP_MAPMANAGER_H
#define SNAKEFOP_MAPMANAGER_H

#include <array>
#include <vector>
#include <string>

using namespace std;

struct Point {
    int x, y;

    bool operator==(const Point &rhs) const {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const Point &rhs) const {
        return !(rhs == *this);
    }
};

struct Map {
    int width, height;

    // Top, bottom, left, right
    array<bool, 4> borderIsObstacle;
    vector<Point> obstacles;
    vector<array<Point, 2>> portals;
    Point spawnPoint;
};

class MapManager {
public:
    static Map LoadMap(const string &path);

    static void SaveMap(const string &path, const Map &map);

    [[nodiscard]] static string GetMapString(const Map &map);
};

#endif //SNAKEFOP_MAPMANAGER_H