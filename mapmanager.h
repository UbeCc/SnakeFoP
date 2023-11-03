#ifndef SNAKEFOP_MAPMANAGER_H
#define SNAKEFOP_MAPMANAGER_H

#include <array>
#include <vector>
#include <string>

using namespace std;

struct Point
{
    int x, y;
};

struct Map
{
    int width, height;

    // Top, bottom, left, right
    array<bool, 4> borderIsObstacle;
    vector<Point> obstacles;
    vector<pair<Point, Point>> portals;
};

class MapManager
{
public:
    static Map LoadMap(const string &path);
    static void SaveMap(const string &path, const Map &map);
    static string GetMapString(const Map &map);
};

#endif //SNAKEFOP_MAPMANAGER_H