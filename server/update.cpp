#include "update.h"

game_state *update_game_state(const game_state *gs, vector<player_command> commands) {
    game_state *new_gs = new game_state;
    *new_gs = *gs;

    for (int i = 0; i < (int)gs->players.size(); i++) {
	point new_position = gs->players[i].position;
	switch (commands[i].dir) {
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
	new_gs->players[i].position = new_position;
	new_gs->blocks[new_gs->block_index(new_position)].crossed_by = i;
    }

    for (int x = 0; x < new_gs->width; x++) {
	for (int y = 0; y < new_gs->height; y++) {
	    int i = new_gs->block_index({x, y});

	    // nothing serious could happend here
	    if (gs->blocks[i].crossed_by == -1) continue;

	    // closed a rectangle
	    if (gs->blocks[i].crossed_by == new_gs->blocks[i].crossed_by) {
		//TODO: fill the closed rectangle
	    }

	    // hit another player
	    if (gs->blocks[i].crossed_by != new_gs->blocks[i].crossed_by) {
		//TODO: kill the other player or both
	    }
	}
    }

    return new_gs;
}
