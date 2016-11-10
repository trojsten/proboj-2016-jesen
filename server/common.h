#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <map>

#include "marshal.h"
#include "mapa.h"

using namespace std;

enum direction {
    LEFT, RIGHT, UP, DOWN
};

struct player_command {
    direction dir;
};

struct point {
    int x, y;
};

struct block {
    square_type type;
    int owned_by;
    int crossed_by;
};

struct player {
    point position;
    direction dir;
    bool alive;

    int score;

    //TODO: bonuses
};

struct game_state {
    int width, height;

    vector<player> players;
    vector<block> blocks;

    game_state() {}
    game_state(int num_players, game_map gm);

    int block_index(point pos) {
	return pos.x * height + pos.y;
    }

    block get_block(point pos) {
	return blocks[block_index(pos)];
    }

    block get_block(int x, int y) {
	return get_block({x, y});
    }
};

#endif


#ifdef reflection
// tieto udaje pouziva marshal.cpp aby vedel ako tie struktury ukladat a nacitavat

reflection(point)
    member(x)
    member(y)
end()

reflection(block)
    member(type)
    member(owned_by)
    member(crossed_by)
end()

reflection(player)
    member(position)
    member(dir)
    member(alive)
    member(score)
end()

reflection(game_state)
    member(width)
    member(height)
    member(players)
    member(blocks)
end()

#endif
