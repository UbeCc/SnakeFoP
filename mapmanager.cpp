#include "mapmanager.h"
#include <fstream>
#include <sstream>

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

    Map map;

    ifs >> map.width >> map.height;

    if (!ifs.good())
    {
        throw runtime_error("Failed to read map width and height");
    }

    for (int i = 0; i < 4; i++)
    {
        ifs >> map.borderIsObstacle[i];
    }

    if (!ifs.good())
    {
        throw runtime_error("Failed to read border");
    }

    int obstacleCount;
    ifs >> obstacleCount;

    if (!ifs.good())
    {
        throw runtime_error("Failed to read obstacles count");
    }

    if (obstacleCount < 0)
    {
        throw runtime_error("Negative obstacles count");
    }

    for (int i = 0; i < obstacleCount; i++)
    {
        Point obstacle;
        ifs >> obstacle.x >> obstacle.y;
        map.obstacles.push_back(obstacle);

        if (!ifs.good())
        {
            throw runtime_error("Failed to read obstacle " + to_string(i));
        }
    }

    int portalCount;
    ifs >> portalCount;

    if (!ifs.good())
    {
        if (ifs.eof())
        {
            portalCount = 0;
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
        Point portal1, portal2;
        ifs >> portal1.x >> portal1.y >> portal2.x >> portal2.y;
        map.portals.push_back(make_pair(portal1, portal2));

        if (!ifs.good())
        {
            throw runtime_error("Failed to read portal " + to_string(i));
        }
    }

    ifs.close();

    return map;
}

void MapManager::SaveMap(const string &path, const Map &map)
{
    ofstream ofs(path);

    if (!ofs.is_open())
    {
        throw runtime_error("Failed to open file " + path);
    }

    ofs << GetMapString(map);
    ofs.close();
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
        ss << portal.first.x << " " << portal.first.y << " " << portal.second.x << " " << portal.second.y << endl;
    }

    return ss.str();
}