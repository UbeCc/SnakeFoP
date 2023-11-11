#include "configmanager.h"

#include <fstream>
#include <sstream>
#include <iomanip>

using std::string;
using std::array;
using std::runtime_error;
using std::ofstream;
using std::ifstream;
using std::ostream;
using std::istream;
using std::stringstream;
using std::endl;

Config ConfigManager::LoadConfigFromStream(istream &stream)
{
    Config config{};

    stream >> config.level;

    if (!stream.good())
    {
        throw runtime_error("无法读取难度");
    }

    if (config.level < 1 || config.level > 30)
    {
        throw runtime_error("难度应该在1到30之间");
    }

    stream >> config.randomSeed;

    if (!stream.good())
    {
        throw runtime_error("无法读取随机种子");
    }

    if (config.randomSeed == -1)
    {
        config.randomSeed = (int) time(nullptr);
    }

    stream >> config.foodCount;

    if (!stream.good())
    {
        throw runtime_error("无法读取食物数量");
    }

    if (config.foodCount <= 0 || config.foodCount > 20)
    {
        throw runtime_error("食物数量应该在1到20之间");
    }

    stream >> config.foodProbabilities[0] >> config.foodProbabilities[1] >> config.foodProbabilities[2];

    if (!stream.good())
    {
        throw runtime_error("无法读取食物生成概率");
    }

    if (config.foodProbabilities[0] < 0 || config.foodProbabilities[0] > 1 ||
        config.foodProbabilities[1] < 0 || config.foodProbabilities[1] > 1 ||
        config.foodProbabilities[2] < 0 || config.foodProbabilities[2] > 1)
    {
        throw runtime_error("食物的概率应该在0到1之间");
    }

    if (std::abs(config.foodProbabilities[0] + config.foodProbabilities[1] + config.foodProbabilities[2] - 1) > EPSILON)
    {
        throw runtime_error("食物的概率和应为1");
    }

    return config;
}

Config ConfigManager::LoadConfigFromString(const std::string &configString)
{
    stringstream ss(configString);
    return LoadConfigFromStream(ss);
}

Config ConfigManager::LoadConfig(const std::string &path)
{
    ifstream ifs(path);

    if (!ifs.is_open())
    {
        throw runtime_error("无法打开文件");
    }

    return LoadConfigFromStream(ifs);
}

void ConfigManager::SaveConfig(const std::string &path, const Config &config)
{
    ofstream ofs(path);

    if (!ofs.is_open())
    {
        throw runtime_error("无法打开文件");
    }

    ofs << GetConfigString(config);
}

string ConfigManager::GetConfigString(const Config &config)
{
    stringstream ss;
    ss << config.level << endl;
    ss << config.randomSeed << endl;
    ss << config.foodCount << endl;
    ss << std::fixed << std::setprecision(2) << config.foodProbabilities[0] << " " << config.foodProbabilities[1] << " "
        << config.foodProbabilities[2] << endl;
    return ss.str();
}
