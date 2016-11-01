#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <map>

using namespace std;

struct point {
    int x, y;
};

struct block {
    int color;
    bool solid;
};

struct player {
    int color;
    point position;

    int score;

    //TODO: bonuses
};

struct area {
    vector<player> players;
    map<point, vector<block>> blocks;
};

#endif


#ifdef reflection
// tieto udaje pouziva marshal.cpp aby vedel ako tie struktury ukladat a nacitavat

#endif
