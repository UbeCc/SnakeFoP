#include "mapmanager.h"
#include <fstream>
#include <sstream>

using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::endl;
using std::to_string;
using std::runtime_error;

Map MapManager::LoadMapFromStream(std::istream &stream)
{

    Map map;

    stream >> map.width >> map.height;

    if (!stream.good())
    {
        throw runtime_error("无法读取地图宽度或高度");
    }

    if (map.width <= 1 || map.height <= 1 || map.width > 100 || map.height > 100)
    {
        throw runtime_error("地图宽度或高度不合法");
    }

    for (int i = 0; i < 4; i++)
    {
        stream >> map.borderIsObstacle[i];
    }

    if (!stream.good())
    {
        throw runtime_error("无法读取地图边界情况");
    }

    int obstacleCount;
    stream >> obstacleCount;

    if (!stream.good())
    {
        throw runtime_error("无法读取障碍物数量");
    }

    if (obstacleCount < 0)
    {
        throw runtime_error("障碍物数量不能为负数");
    }

    for (int i = 0; i < obstacleCount; i++)
    {
        Point obstacle{};
        stream >> obstacle.x >> obstacle.y;
        map.obstacles.push_back(obstacle);

        if (!stream.good())
        {
            throw runtime_error("无法读取障碍物" + to_string(i) + "的坐标");
        }

        if (obstacle.x < 0 || obstacle.x >= map.width || obstacle.y < 0 || obstacle.y >= map.height)
        {
            throw runtime_error("障碍物" + to_string(i) + "的坐标超出地图范围");
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
            throw runtime_error("无法读取传送门数量");
        }
    }

    if (portalCount < 0)
    {
        throw runtime_error("传送门数量不能为负数");
    }

    for (int i = 0; i < portalCount; i++)
    {
        Point portal1{}, portal2{};
        stream >> portal1.x >> portal1.y >> portal2.x >> portal2.y;
        map.portals.push_back({portal1, portal2});

        if (!stream.good())
        {
            throw runtime_error("无法读取传送门" + to_string(i) + "的坐标");
        }

        if (portal1.x < 0 || portal1.x >= map.width || portal1.y < 0 || portal1.y >= map.height
            || portal2.x < 0 || portal2.x >= map.width || portal2.y < 0 || portal2.y >= map.height)
        {
            throw runtime_error("传送门" + to_string(i) + "的坐标超出地图范围");
        }

        if (portal1 == portal2)
        {
            throw runtime_error("传送门" + to_string(i) + "的两个端点重合");
        }

        for (const auto &o: map.obstacles)
        {
            if (o == portal1 || o == portal2)
            {
                throw runtime_error("传送门" + to_string(i) + "的端点是障碍物");
            }
        }
    }

    stream >> map.spawnPoint.x >> map.spawnPoint.y;

    if (!stream.good())
    {
        throw runtime_error("无法读取出生点坐标");
    }

    if (map.spawnPoint.x < 0 || map.spawnPoint.x >= map.width || map.spawnPoint.y < 0 || map.spawnPoint.y >= map.height)
    {
        throw runtime_error("出生点坐标超出地图范围");
    }

    for (auto &o: map.obstacles)
    {
        if (o == map.spawnPoint)
        {
            throw runtime_error("出生点不能是障碍物");
        }
    }

    for (auto &p: map.portals)
    {
        if (p[0] == map.spawnPoint || p[1] == map.spawnPoint)
        {
            throw runtime_error("出生点不能是传送门");
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
        throw runtime_error("无法打开文件");
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
        throw runtime_error("无法打开文件");
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

    for (auto obstacle: map.obstacles)
    {
        ss << obstacle.x << " " << obstacle.y << endl;
    }

    ss << map.portals.size() << endl;

    for (auto portal: map.portals)
    {
        ss << portal[0].x << " " << portal[0].y << " " << portal[1].x << " " << portal[1].y << endl;
    }

    ss << map.spawnPoint.x << " " << map.spawnPoint.y << endl;

    return ss.str();
}
