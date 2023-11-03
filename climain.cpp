/// CLI Main, entry point to test background task execution.

#include <iostream>
#include "MapManager.h"

using namespace std;

int main() {
    Map m = MapManager::LoadMap("map.txt");
    cout << MapManager::GetMapString(m);
    return 0;
}