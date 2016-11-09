#include "update.h"
#include <iostream>

using namespace std;

void killPlayer (game_state& gs, int dier) {
    gs.players[dier].alive = false;
    for (int x = 0; x < gs.width; x++) {
        for (int y = 0; y < gs.height; y++) {
            int i = gs.block_index({x, y});
            if (gs.blocks[i].owned_by == dier) {
                gs.blocks[i].owned_by = -1;
            }
            if (gs.blocks[i].crossed_by == dier) {
                gs.blocks[i].crossed_by = -1;
            }
        }
    }
}

game_state update_game_state(game_state gs, vector<vector<player_command> > commands) {
    game_state new_gs = gs;

    // pre kazdeho z hracov zistime jeho novy smer
    for (unsigned i = 0; i < gs.players.size(); i++) {
        if (!gs.players[i].alive) {
            continue;
        }
        if (!commands[i].empty()) {
            // dovolime len otacanie o 90 stupnov
            direction new_dir = commands[i].back().dir;
            if (gs.players[i].dir == LEFT && new_dir == RIGHT) {
                continue;
            }
            if (gs.players[i].dir == RIGHT && new_dir == LEFT) {
                continue;
            }
            if (gs.players[i].dir == UP && new_dir == DOWN) {
                continue;
            }
            if (gs.players[i].dir == DOWN && new_dir == UP) {
                continue;
            }
            new_gs.players[i].dir = new_dir;
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
        
        // nezabil sa o stenu / okraj mapy?
        if (new_position.x < 0 || new_position.x >= new_gs.width) {
            killPlayer(new_gs, i);
        }
        if (new_position.y < 0 || new_position.y >= new_gs.height) {
            killPlayer(new_gs, i);
        }
        
        // nezabili sme prave niekoho?
        block curr = gs.blocks[gs.block_index(new_position)];
        if (curr.crossed_by != -1) {
            if (curr.owned_by != curr.crossed_by) {
                killPlayer(new_gs, curr.crossed_by);
            }
        }
    }
    
    // kazdy hrac obsadi svoje policko
    for (unsigned i = 0; i < new_gs.players.size(); i++) {
        if (!new_gs.players[i].alive) {
            continue;
        }
        point position = new_gs.players[i].position;
        new_gs.blocks[new_gs.block_index(position)].crossed_by = i;
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
