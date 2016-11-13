#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

#ifdef __MINGW32__
#include <sys/time.h>
#include "../../server/common.h"
#else
#include "common.h"
#endif

using namespace std;

int ja = -1;

int main() {
    string str;
    cin >> str >> ja;

    cerr << ja << endl;

    srand(time(NULL)+ja*1000);

    game_state gs;

    while (true) {
	nacitaj(cin, gs);

	int x = gs.players[ja].position.x;
	int y = gs.players[ja].position.y;

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
	}
    }
}
