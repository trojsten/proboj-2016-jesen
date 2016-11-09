#ifndef MAPA_H
#define MAPA_H

#include <fstream>
#include <vector>

using namespace std;

enum square_type {
    EMPTY, SPAWN
};

struct game_map {
    int width, height;
    vector<vector<square_type>> squares;
    
    game_map(){}

    game_map(int width, int height);
    
    void load (fstream& mapfile) ;
};

#endif
