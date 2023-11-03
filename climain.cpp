/// CLI Main, entry point to test background task execution.

#include <iostream>
#include "mapmanager.h"
#include "configmanager.h"

using namespace std;

int main() {
    Config f = ConfigManager::LoadConfig("config/default.txt");
    return 0;
}