/// CLI Main, entry point to test background task execution.

#include <iostream>
#include <csignal>
#include "mapmanager.h"
#include "configmanager.h"
#include "game.h"

using namespace std;

int main() {
    default_random_engine random(time(nullptr));
    Config f = ConfigManager::LoadConfig("config/default.txt");
    Map m = MapManager::LoadMap("maps/test_map.txt");
    Game g(m, f, 0);

    cout << g.GetStatisticsString() << endl;
    while (g.GetStatus().state == Game::State::Alive) {
        if (random() % 5 == 0) {
            g.ChangeDirection((Game::Direction) (random() % 4));
        }

        g.Step();
        cout << g.GetStatisticsString() << endl;
        usleep(50000);
    }

    return 0;
}
