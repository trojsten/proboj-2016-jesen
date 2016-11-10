#ifndef MAPA_H
#define MAPA_H

#include <fstream>
#include <vector>

using namespace std;

enum square_type {
    EMPTY, WALL, SPAWN, BONUS_SPAWN, FAST_BONUS, STONE_BONUS
};

struct game_map {
    int width, height;
    vector<vector<square_type>> squares;
    
    game_map(){}

    game_map(int width, int height);
    
    bool load (string filename) ;
};

#endif
