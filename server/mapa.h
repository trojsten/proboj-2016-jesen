#ifndef MAPA_H
#define MAPA_H

#include <vector>

using namespace std;

enum square_type {
    EMPTY, SPAWN
};

struct game_map {
    int width, height;
    vector<vector<square_type>> squares;

    game_map(int width, int height);
};

#endif
