#include "update.h"
#include <iostream>

using namespace std;

game_state kill_player(game_state gs, int dier) {
    // delete all crossed blocks
    gs.players[dier].alive = false;
    for (unsigned i = 0; i < gs.blocks.size(); i++) {
	if (gs.blocks[i].crossed_by == dier) {
	    gs.blocks[i].crossed_by = -1;
	}
    }
    return gs;
}

game_state own_territory(game_state gs, int player) {
    // mark all crossed blocks as owned
    for (unsigned i = 0; i < gs.blocks.size(); i++) {
	if (gs.blocks[i].crossed_by == player) {
	    gs.blocks[i].owned_by = player;
	    gs.blocks[i].crossed_by = -1;
	}
    }

    vector<bool> visited(gs.blocks.size(), false);
    for (int x = 0; x < gs.width; x++) {
	for (int y = 0; y < gs.height; y++) {
	    int i = gs.block_index({x, y});

	    if (visited[i]) continue;

	    bool should_own = true;

	    vector<pair<int, int>> stack;
	    vector<bool> tested(gs.blocks.size(), false);
	    stack.push_back({x, y});

	    while (!stack.empty()) {
		int x = stack.back().first;
		int y = stack.back().second;
		int i = gs.block_index({x, y});
		stack.pop_back();

		if (gs.blocks[i].owned_by == player) continue;

		if (tested[i]) continue;
		tested[i] = true;
		
		if (x == 0 || y == 0 || x == gs.width - 1 || y == gs.height - 1) {
		    should_own = false;
		    break;
		}

		stack.push_back({x - 1, y});
		stack.push_back({x + 1, y});
		stack.push_back({x, y - 1});
		stack.push_back({x, y + 1});
	    }

	    if (should_own) {
		for (unsigned i = 0; i < gs.blocks.size(); i++) {
		    if (tested[i]) {
			gs.blocks[i].owned_by = player;
		    }
		}
	    }
	}
    }

    return gs;
}

game_state update_game_state(game_state gs, vector<vector<player_command> > commands) {
    game_state new_gs = gs;

    // pre kazdeho z hracov zistime jeho novy smer
    for (unsigned i = 0; i < gs.players.size(); i++) {
        if (!gs.players[i].alive) {
            continue;
        }
        if (!commands[i].empty()) {
            new_gs.players[i].dir = commands[i].back().dir;
        }
    }
    
    // zistime novu poziciu kazdeho hraca
    for (unsigned i = 0; i < gs.players.size(); i++) {
        if (!gs.players[i].alive) {
            continue;
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
        new_gs.players[i].position = new_position;
    }

    // zistime, ci sa nezrazili
    for (unsigned i = 0; i < new_gs.players.size(); i++) {
        // nezabil sa o stenu / okraj mapy?
        if (new_gs.players[i].position.x < 0 || new_gs.players[i].position.x >= new_gs.width) {
            new_gs = kill_player(new_gs, i);
        }
        else
        if (new_gs.players[i].position.y < 0 || new_gs.players[i].position.y >= new_gs.height) {
            new_gs = kill_player(new_gs, i);
        }
        else {
            // nezabili sme prave niekoho?
            block curr = gs.blocks[gs.block_index(new_gs.players[i].position)];
            if (curr.crossed_by != -1) {
                if (curr.owned_by != curr.crossed_by) {
                    kill_player(new_gs, curr.crossed_by);
                }
            }
        }
    }
    
    // kazdy hrac obsadi svoje policko
    for (unsigned i = 0; i < new_gs.players.size(); i++) {
        if (!new_gs.players[i].alive) continue;
	int pos_index = new_gs.block_index(new_gs.players[i].position);
        new_gs.blocks[pos_index].crossed_by = i;
    }

    // zistime, ci hrac neobsadil uzemie
    for (unsigned i = 0; i < new_gs.players.size(); i++) {
	if (!new_gs.players[i].alive) continue;
	int pos_index = new_gs.block_index(new_gs.players[i].position);
	if (new_gs.blocks[pos_index].owned_by == (int)i) {
	    new_gs = own_territory(new_gs, (int)i);
	}
    }

    /*
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

        // cout << "pos " << new_position.x << " " << new_position.y << " " << gs.block_index(new_position) << endl;

        new_gs.players[i].position = new_position;
        new_gs.blocks[new_gs.block_index(new_position)].crossed_by = i;
    }
    */

    return new_gs;
}
