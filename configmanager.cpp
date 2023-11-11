#include "configmanager.h"

#include <fstream>
#include <sstream>

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
        throw runtime_error("Failed to read Level");
    }

    stream >> config.randomSeed;

    if (!stream.good())
    {
        throw runtime_error("Failed to read random seed");
    }

    if (config.randomSeed == -1)
    {
        config.randomSeed = (int) time(nullptr);
    }

    stream >> config.foodCount;

    if (!stream.good())
    {
        throw runtime_error("Failed to read food count");
    }

    if (config.foodCount <= 0 || config.foodCount > 20)
    {
        throw runtime_error("Food count should be within range [1, 20]");
    }

    stream >> config.foodProbabilities[0] >> config.foodProbabilities[1] >> config.foodProbabilities[2];

    if (!stream.good())
    {
        throw runtime_error("Failed to read food probabilities");
    }

    if (config.foodProbabilities[0] < 0 || config.foodProbabilities[0] > 1 ||
        config.foodProbabilities[1] < 0 || config.foodProbabilities[1] > 1 ||
        config.foodProbabilities[2] < 0 || config.foodProbabilities[2] > 1)
    {
        throw runtime_error("Food probabilities should be within range [0, 1]");
    }

    if (std::abs(config.foodProbabilities[0] + config.foodProbabilities[1] + config.foodProbabilities[2] - 1) > 1e-6)
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
        throw runtime_error("文件操作异常");
    }

    return LoadConfigFromStream(ifs);
}

void ConfigManager::SaveConfig(const std::string &path, const Config &config)
{
    ofstream ofs(path);

    if (!ofs.is_open())
    {
        throw runtime_error("文件操作异常");
    }

    ofs << GetConfigString(config);
}

string ConfigManager::GetConfigString(const Config &config)
{
    stringstream ss;
    ss << config.level << endl;
    ss << config.randomSeed << endl;
    ss << config.foodCount << endl;
    ss << config.foodProbabilities[0] << " " << config.foodProbabilities[1] << " " << config.foodProbabilities[2]
        << endl;
    return ss.str();
}
