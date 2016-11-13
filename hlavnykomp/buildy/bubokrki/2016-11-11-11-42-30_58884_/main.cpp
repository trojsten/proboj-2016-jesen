#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>

#include "common.h"

using namespace std;

int ja = -1;

int main() {
    srand(time(NULL));

    string str;
    cin >> str >> ja;

    cerr << ja << endl;

    game_state gs;
    cerr << "NACITAL SOM" << endl;
    nacitaj(cin, gs);
    cerr << "NACITAL SOM" << endl;
    
    //prvy direction je k najblizsiemu rohu

    int x = gs.players[ja].position.x;
    int y = gs.players[ja].position.y;
    
    cerr << "NACITAL SOM POZICIU" << endl;

    string dir_akt;

    // RD RU LD LU
    char*  dir_realization [] [6] = {    {"RIGHT", "DOWN", "LEFT", "DOWN", "RIGHT", "UP"},
                                                    {"RIGHT", "UP", "LEFT", "UP", "RIGHT", "DOWN"},
                                                    {"LEFT", "DOWN", "RIGHT", "DOWN", "LEFT", "UP"},
                                                    {"LEFT", "UP", "RIGHT", "UP", "LEFT", "DOWN"}
                                               };

    //Prve urcenie smeru 
    int dir_index_left;
    int dir_index_right = 5;
    int strana_stvorca = 3;
    int krokov_do_zmeny = strana_stvorca;

    cerr << "INIT ENDED" << endl;
    
    if(x >= gs.width / 2) dir_index_left = 0;
    else dir_index_left = 2;

    if(y < gs.height / 2) dir_index_left += 1;

    //Pocitanie optimalnej velkosti stvorca TODO
    // krokov_do_zmeny = strana_stvorca;
    
    cerr << "IDEM DO WHILE" << endl;

    while (true) {
        cerr << "ROBIA SA VECI" << endl;
        cerr << "NACITAL SOM" << endl;
        /*
	for (int x = 0; x < gs.width; x++) {
	    for (int y = 0; y < gs.height; y++) {
		char c = '.';
		if (gs.get_block(x, y).type == WALL) c = '#';
		else if (gs.get_block(x, y).crossed_by == ja) c = 'A';
		else if (gs.get_block(x, y).crossed_by != -1) c = 'B';
		cerr << c << " ";
	    }
	    cerr << endl;
	}
	cerr << endl;

	vector<string> dirs;
	if (x > 0 && gs.blocks[gs.block_index({x - 1, y})].crossed_by != ja) dirs.push_back("LEFT");
	if (x < gs.width - 1 && gs.blocks[gs.block_index({x + 1, y})].crossed_by != ja) dirs.push_back("RIGHT");
	if (y > 0 && gs.blocks[gs.block_index({x, y - 1})].crossed_by != ja) dirs.push_back("UP");
	if (y < gs.height - 1 && gs.blocks[gs.block_index({x, y + 1})].crossed_by != ja) dirs.push_back("DOWN");

	if (dirs.size() > 0) {
	    cout << "cd " << dirs[rand() % dirs.size()] << endl;
	}*/
        if(krokov_do_zmeny > 1) {
            dir_akt = dir_realization[dir_index_left][dir_index_right];
            krokov_do_zmeny--;
        }
        else {
            dir_index_right++;
            dir_index_right = dir_index_right % 6;
            dir_akt = dir_realization[dir_index_left][dir_index_right];
            krokov_do_zmeny = strana_stvorca;
        }

        cout << "cd " << dir_akt << endl;

        nacitaj(cin, gs);
    }   
}
