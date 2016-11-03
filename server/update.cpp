#include "update.h"

void update_game_state(game_state *gs, vector<player_command> commands) {
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
    }
}
