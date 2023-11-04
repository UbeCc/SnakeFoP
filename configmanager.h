#ifndef SNAKEFOP_CONFIGMANAGER_H
#define SNAKEFOP_CONFIGMANAGER_H

#include <array>
#include <string>

using namespace std;

struct Config {
    int level;
    int randomSeed;
    int foodCount;
    array<float, 3> foodProbabilities;
};

class ConfigManager {
private:
    static Config LoadConfigFromStream(istream &stream);

public:
    static Config LoadConfig(const string &path);
    static Config LoadConfigFromString(const string &configString);
    static void SaveConfig(const string &path, const Config &config);
    [[nodiscard]] static string GetConfigString(const Config &config);
};

#endif //SNAKEFOP_CONFIGMANAGER_H
