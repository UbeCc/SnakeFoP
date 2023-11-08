#include "recordmanager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <QDebug>

using std::ifstream;
using std::ofstream;
using std::runtime_error;
using std::endl;

/**
 * @brief Load record from given path.
 *
 * @remark Record file format:
 * L1: config file trunked into one line
 * L2: map file trunked into one line
 * L2+: movement (M {W, A, S, D})
 *          up: W; left: A; down: S; right: D
 *      or food generation (F, X, Y, V)
 *          position: (X, Y); score: V
 */
Record RecordManager::LoadRecord(const string &path, Record &record) {
    record.foodSequence.clear();
    record.moveSequence.clear();
    record.sequence.clear();
    record.timestamp.clear();

    ifstream ifs(path);

    if (!ifs.is_open()) {
        throw runtime_error("打开文件失败：" + path);
    }
    char buffer[1048576];
    ifs.getline(buffer, 1048576);
    if (!ifs.good()) {
        throw runtime_error("打开配置失败");
    }

    record.config = ConfigManager::LoadConfigFromString(buffer);

    ifs.getline(buffer, 1048576);
    if (!ifs.good()) {
        throw runtime_error("打开地图失败");
    }

    record.map = MapManager::LoadMapFromString(buffer);

    char op;
    while (ifs >> op) {
        if (op == 'F') {
            int tme, x, y;
            int value;
            ifs >> tme >> x >> y >> value;
            record.foodSequence.emplace_back(Point{x, y}, value);
            record.timestamp.emplace_back(tme);
        } else if (op == 'M') {
            int tme;
            char dire;
            ifs >> tme >> dire;
            record.moveSequence += dire;
            record.timestamp.emplace_back(tme);
        }
        record.sequence += op;
    }

    return record;
}

void RecordManager::SaveRecord(const string &path, const Record &record) {
    ofstream ofs(path);
    if (!ofs.is_open()) {
        throw runtime_error("打开文件失败：" + path);
    }

    string config = ConfigManager::GetConfigString(record.config);
    replace(config.begin(), config.end(), '\n', ' ');
    ofs << config << "\n";

    string map = MapManager::GetMapString(record.map);
    replace(map.begin(), map.end(), '\n', ' ');
    ofs << map << "\n";

    int foodPtr = 0, movePtr = 0, seqPtr = 0, seqLen = (int) record.sequence.length();
    while (seqPtr != seqLen) {
        char op = record.sequence[seqPtr];
        if (op == 'F') {
            int tme = record.timestamp[seqPtr], x = record.foodSequence[foodPtr].first.x, y = record.foodSequence[foodPtr].first.y, v = record.foodSequence[foodPtr].second;
            foodPtr++;
            ofs << "F " << tme << " " << x << " " << y << " " << v << "\n";
        } else if (op == 'M') {
            int tme = record.timestamp[seqPtr];
            char dire = record.moveSequence[movePtr];
            movePtr++;
            ofs << "M " << tme << " " << dire << "\n";
        }
        seqPtr++;
    }
}

void Record::Reset(const Map &_map, const Config &_config) {
    map = _map;
    config = _config;
    sequence = "";
    while(!timestamp.empty()) timestamp.pop_back();
    while(!foodSequence.empty()) foodSequence.pop_back();
    moveSequence = "";
}

void Record::Reset(const Record& record) {
    map = record.map;
    config = record.config;
    sequence = record.sequence;
    timestamp = record.timestamp;
    foodSequence = record.foodSequence;
    moveSequence = record.moveSequence;
}
