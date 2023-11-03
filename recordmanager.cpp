#include "recordmanager.h"
#include <fstream>
#include <sstream>

/**
 * @brief Load record from given path.
 *
 * @remark Record file format:
 * L1: config file path
 * L2: map file path
 * L2+: movement (M {W, A, S, D})
 *          up: W; left: A; down: S; right: D
 *      or food generation (F, X, Y, V)
 *          position: (X, Y); score: V
 */
Record RecordManager::LoadRecord(const string &path) {
    ifstream ifs(path);
    Record record;

    if(!ifs.is_open()) {
        throw runtime_error("Failed to open file " + path);
    }
    string configPath, mapPath;
    ifs >> configPath >> mapPath;
    if(!ifs.good()) {
        throw runtime_error("Failed ot load map or config\nConfig Path: " + configPath + "\nMap Path: " + mapPath);
    }
    record.config = ConfigManager::LoadConfig(configPath);
    record.map = MapManager::LoadMap(mapPath);
    char op;
    while(ifs >> op) {
        if(op == 'F') {
            char x, y;
            int value;
            ifs >> x >> y >> value;
            record.foodSequence.push_back(make_pair(Point{x, y}, value));
        }
        else if(op == 'M') {
            char dire;
            ifs >> dire;
            record.moveSequence += dire;
        }
        record.sequence += op;
    }
    record.configPath = configPath;
    record.mapPath = mapPath;
    return record;
}

void RecordManager::SaveRecord(const string &path, const Record &record) {
    ofstream ofs(path);
    if(!ofs.is_open()) {
        throw runtime_error("Failed to open file " + path);
    }
    ofs << record.configPath << "\n" << record.mapPath;
    int foodPtr = 0, movePtr = 0, seqPtr = 0, seqLen = record.sequence.length();
    while(seqPtr != seqLen) {
        char op = record.sequence[seqPtr];
        if(op == 'F') {
            int x = record.foodSequence[foodPtr].first.x, y = record.foodSequence[foodPtr].first.y, v = record.foodSequence[foodPtr].second;
            foodPtr++;
            ofs << "F " << x << " " << y << " " << v << "\n";
        } else if(op == 'M') {
            char dire = record.moveSequence[movePtr];
            movePtr++;
            ofs << "M " << dire << "\n";
        }
        seqPtr++;
    }
}

RecordManager::RecordManager(){}
