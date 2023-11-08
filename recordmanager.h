#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include "mapmanager.h"
#include "configmanager.h"
#include <vector>
#include <utility>

using std::vector;
using std::pair;

struct Record
{
    string name;
    Map map;
    Config config;
    string sequence;
    vector<int> timestamp;
    vector<pair<Point, int> > foodSequence;
    string moveSequence;

    void Reset(const Map &_map, const Config &_config);

    void Reset(const Record &record);
};

class RecordManager
{
public:
    static Record LoadRecord(const string &path, Record &);

    static void SaveRecord(const string &path, const Record &record);
};

#endif // RECORDMANAGER_H
