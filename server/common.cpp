#include "common.h"
#include <algorithm>

game_state::game_state(int num_players, game_map gm) {
    this->width = gm.width;
    this->height = gm.height;
    this->blocks.resize(this->width * this->height);

    for (int x = 0; x < this->width; x++) {
	for (int y = 0; y < this->height; y++) {
	    int i = this->block_index({x, y});
	    this->blocks[i] = {gm.squares[x][y], -1, -1};
	}
    }

    vector<pair<int, int>> spawns;
    for (int x = 0; x < gm.width; x++) {
	for (int y = 0; y < gm.height; y++) {
	    if (gm.squares[x][y] == SPAWN) {
		spawns.push_back({x, y});
	    }
	}
    }

    random_shuffle(spawns.begin(), spawns.end());
    for (int i = 0; i < num_players; i++) {
	point pos = {spawns[i].first, spawns[i].second};
	this->players.push_back(player{pos, UP, true, 0, 0});
	this->blocks[this->block_index(pos)].crossed_by = i;

	for (int x = -1; x <= 1; x++) {
	    for (int y = -1; y <= 1; y++) {
		this->blocks[this->block_index({pos.x + x, pos.y + y})].owned_by = i;
	    }
	}
    }
}
