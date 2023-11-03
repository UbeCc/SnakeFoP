#include "configmanager.h"

#include <fstream>
#include <sstream>

using namespace std;

Config ConfigManager::LoadConfig(const std::string &path) {
    ifstream ifs(path);

    if (!ifs.is_open()) {
        throw runtime_error("Failed to open file " + path);
    }

    Config config;

    ifs >> config.level;

    if (!ifs.good()) {
        throw runtime_error("Failed to read level");
    }

    ifs >> config.randomSeed;

    if (!ifs.good()) {
        throw runtime_error("Failed to read random seed");
    }

    if (config.randomSeed == -1) {
        config.randomSeed = time(nullptr);
    }

    ifs >> config.foodCount;

    if (!ifs.good()) {
        throw runtime_error("Failed to read food count");
    }

    if (config.foodCount <= 0 || config.foodCount > 20) {
        throw runtime_error("Food count should be within range [1, 20]");
    }

    ifs >> config.foodProbabilities[0] >> config.foodProbabilities[1] >> config.foodProbabilities[2];

    if (!ifs.good()) {
        throw runtime_error("Failed to read food probabilities");
    }

    if (config.foodProbabilities[0] < 0 || config.foodProbabilities[0] > 1 ||
        config.foodProbabilities[1] < 0 || config.foodProbabilities[1] > 1 ||
        config.foodProbabilities[2] < 0 || config.foodProbabilities[2] > 1) {
        throw runtime_error("Food probabilities should be within range [0, 1]");
    }

    if (abs(config.foodProbabilities[0] + config.foodProbabilities[1] + config.foodProbabilities[2] - 1) > 1e-6) {
        throw runtime_error("Sum of food probabilities should be less than or equal to 1");
    }

    return config;
}

void ConfigManager::SaveConfig(const std::string &path, const Config &config) {
    ofstream ofs(path);

    if (!ofs.is_open()) {
        throw runtime_error("Failed to open file " + path);
    }

    ofs << GetConfigString(config);
}

string ConfigManager::GetConfigString(const Config &config) {
    stringstream ss;
    ss << config.level << endl;
    ss << config.randomSeed << endl;
    ss << config.foodCount << endl;
    ss << config.foodProbabilities[0] << " " << config.foodProbabilities[1] << " " << config.foodProbabilities[2] << endl;
    return ss.str();
}