#include "mapmanager.h"
#include <fstream>
#include <sstream>

Map MapManager::LoadMapFromStream(std::istream &stream) {

    Map map;

    stream >> map.width >> map.height;

    if (!stream.good())
    {
        throw runtime_error("Failed to read map width and height");
    }

    if (map.width <= 1 || map.height <= 1 || map.width > 100 || map.height > 100)
    {
        throw runtime_error("Invalid map width or height");
    }

    for (int i = 0; i < 4; i++)
    {
        stream >> map.borderIsObstacle[i];
    }

    if (!stream.good())
    {
        throw runtime_error("Failed to read border");
    }

    int obstacleCount;
    stream >> obstacleCount;

    if (!stream.good())
    {
        throw runtime_error("Failed to read obstacles count");
    }

    if (obstacleCount < 0)
    {
        throw runtime_error("Negative obstacles count");
    }

    for (int i = 0; i < obstacleCount; i++)
    {
        Point obstacle{};
        stream >> obstacle.x >> obstacle.y;
        map.obstacles.push_back(obstacle);

        if (!stream.good())
        {
            throw runtime_error("Failed to read obstacle " + to_string(i));
        }

        if (obstacle.x < 0 || obstacle.x >= map.width || obstacle.y < 0 || obstacle.y >= map.height) {
            throw runtime_error("Obstacle out of map");
        }
    }

    int portalCount;
    stream >> portalCount;

    if (!stream.good())
    {
        // Standard map format ends here.
        if (stream.eof())
        {
            map.spawnPoint = {0, 0};
            return map;
        }

        else
        {
            throw runtime_error("Failed to read portals count");
        }
    }

    if (portalCount < 0)
    {
        throw runtime_error("Negative portals count");
    }

    for (int i = 0; i < portalCount; i++)
    {
        Point portal1{}, portal2{};
        stream >> portal1.x >> portal1.y >> portal2.x >> portal2.y;
        map.portals.push_back({portal1, portal2});

        if (!stream.good())
        {
            throw runtime_error("Failed to read portal " + to_string(i));
        }

        if (portal1.x < 0 || portal1.x >= map.width || portal1.y < 0 || portal1.y >= map.height) {
            throw runtime_error("Portal out of map");
        }
    }

    stream >> map.spawnPoint.x >> map.spawnPoint.y;

    if (!stream.good())
    {
        throw runtime_error("Failed to read spawn point");
    }

    if (map.spawnPoint.x < 0 || map.spawnPoint.x >= map.width || map.spawnPoint.y < 0 || map.spawnPoint.y >= map.height)
    {
        throw runtime_error("Spawn point out of map");
    }

    for (auto &o : map.obstacles) {
        if (o.x == map.spawnPoint.x && o.y == map.spawnPoint.y) {
            throw runtime_error("Spawn point is an obstacle");
        }
    }

    for (auto &p : map.portals) {
        if (p[0].x == map.spawnPoint.x && p[0].y == map.spawnPoint.y) {
            throw runtime_error("Spawn point is a portal");
        }
        if (p[1].x == map.spawnPoint.x && p[1].y == map.spawnPoint.y) {
            throw runtime_error("Spawn point is a portal");
        }
    }

    return map;
}

/**
 * @brief Load map from given path.
 *
 * @remark Map file format:
 * L1: width height
 * L2: borderIsObstacle[0] borderIsObstacle[1] borderIsObstacle[2] borderIsObstacle[3]
 * L3: number of obstacles
 * L4+: obstacles (x, y)
 * L4+number of obstacles: number of portals
 * L5+number of obstacles: portals (x1, y1, x2, y2)
 */
Map MapManager::LoadMap(const string &path)
{
    ifstream ifs(path);

    if (!ifs.is_open())
    {
        throw runtime_error("Failed to open file " + path);
    }

    return LoadMapFromStream(ifs);
}

Map MapManager::LoadMapFromString(const string &mapString)
{
    stringstream ss(mapString);
    return LoadMapFromStream(ss);
}

void MapManager::SaveMap(const string &path, const Map &map)
{
    ofstream ofs(path);

    if (!ofs.is_open())
    {
        throw runtime_error("Failed to open file " + path);
    }

    ofs << GetMapString(map);
}

string MapManager::GetMapString(const Map &map)
{
    stringstream ss;
    ss << map.width << " " << map.height << endl;

    for (int i = 0; i < 3; i++)
    {
        ss << map.borderIsObstacle[i] << " ";
    }

    ss << map.borderIsObstacle[3] << endl;

    ss << map.obstacles.size() << endl;

    for (auto obstacle : map.obstacles)
    {
        ss << obstacle.x << " " << obstacle.y << endl;
    }

    ss << map.portals.size() << endl;

    for (auto portal : map.portals)
    {
        ss << portal[0].x << " " << portal[0].y << " " << portal[1].x << " " << portal[1].y << endl;
    }

    return ss.str();
}