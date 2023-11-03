#ifndef SNAKEFOP_CONFIGMANAGER_H
#define SNAKEFOP_CONFIGMANAGER_H

#include <array>
#include <string>

using namespace std;

struct Config {
    int level;
    unsigned int randomSeed;
    int foodCount;
    array<float, 3> foodProbabilities;
};

class ConfigManager {
public:
    static Config LoadConfig(const string &path);
    static void SaveConfig(const string &path, const Config &config);
    static string GetConfigString(const Config &config);
};

#endif //SNAKEFOP_CONFIGMANAGER_H
