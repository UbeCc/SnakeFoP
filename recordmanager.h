#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include "mapmanager.h"
#include "configmanager.h"
#include <vector>
#include <utility>

using namespace std;

struct Record {
    string name;
    Map map;
    Config config;
    string sequence;
    vector<pair<Point, int> > foodSequence;
    string moveSequence;
    void reset(Map, Config);
};

class RecordManager
{
public:
    RecordManager();
    static Record LoadRecord(const string &path);
    static void SaveRecord(const string &path, const Record &record);
};

#endif // RECORDMANAGER_H
