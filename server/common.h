#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <map>

using namespace std;

struct point {
    int x, y;
};

struct block {
    int owned_by;
    int crossed_by;
};

struct player {
    point position;

    int score;

    //TODO: bonuses
};

struct game_state {
    vector<player> players;
    vector<block> blocks;
};

#endif


#ifdef reflection
// tieto udaje pouziva marshal.cpp aby vedel ako tie struktury ukladat a nacitavat

reflection(point)
    member(x)
    member(y)
end()

reflection(block)
    member(owned_by)
    member(crossed_by)
end()

reflection(player)
    member(position)
    member(score)
end()

reflection(game_state)
    member(players)
    member(blocks)
end()

#endif
