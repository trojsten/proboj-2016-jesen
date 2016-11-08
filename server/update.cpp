#include "update.h"
#include <iostream>

using namespace std;

game_state update_game_state(game_state gs, vector<vector<player_command>> commands) {
    game_state new_gs = gs;

    for (unsigned i = 0; i < gs.players.size(); i++) {
	for (player_command cmd: commands[i]) {
	    new_gs.players[i].dir = cmd.dir;
	}

	point new_position = new_gs.players[i].position;
	switch (new_gs.players[i].dir) {
	case LEFT:
	    new_position.x -= 1;
	    break;
	case RIGHT:
	    new_position.x += 1;
	    break;
	case UP:
	    new_position.y -= 1;
	    break;
	case DOWN:
	    new_position.y += 1;
	    break;
	}

	if (new_position.x < 0) new_position.x = 0;
	if (new_position.x >= gs.width) new_position.x = gs.width - 1;
	if (new_position.y < 0) new_position.y = 0;
	if (new_position.y >= gs.height) new_position.y = gs.height - 1;

	cout << "pos " << new_position.x << " " << new_position.y << " " << gs.block_index(new_position) << endl;

	new_gs.players[i].position = new_position;
	new_gs.blocks[new_gs.block_index(new_position)].crossed_by = i;
    }

    return new_gs;
}
