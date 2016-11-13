#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifdef __MINGW32__
#include <sys/time.h>
#include "../../server/common.h"
#else
#include "common.h"
#endif

using namespace std;

int ja = -1;
game_state gs;


string dirtostr(int dir) {
    switch (dir) {
        case LEFT: return "LEFT";
        case UP: return "UP";
        case RIGHT: return "RIGHT";
        case DOWN: return "DOWN";
        case -1: return "INVALID";
    }
}


int dfs(int x, int y, int lim, const int direct, bool empty=true) {
	if (x<0||y<0||x>=gs.width||y>=gs.height) return -1;
    int dir = direct;
    auto pole = gs.get_block(x,y);
    if (direct != -1 &&(pole.crossed_by == ja || pole.type == WALL || pole.type == FAST_BONUS || lim < 0)) return -1;
    if (!empty && (pole.owned_by == ja || pole.type == STONE_BONUS)) return dir;
    if (empty && !(pole.owned_by == ja || pole.type == STONE_BONUS)) empty=false;
    if (dir == -1) dir = gs.players[ja].dir;
    int d;
    for (int i = 0; i < 4; i++) {
        switch (dir) {
            case UP:
                d = dfs(x-1,y,lim-1,LEFT,empty);
                dir = RIGHT;
                break;
            case RIGHT:
                d = dfs(x,y-1,lim-1,UP,empty);
                dir = DOWN;
                break;
            case DOWN:
                d = dfs(x+1,y,lim-1,RIGHT,empty);
                dir = LEFT;
                break;
            case LEFT:
                d = dfs(x,y+1,lim-1,DOWN,empty);
                dir = UP;
                break;
        }
        if (d != -1) {
            cerr << "dfs( " << x << ", " << y << ", lim=" << lim << ", dir=" << dirtostr(direct) << ") = " << dirtostr(d) << endl;
            return d;
        }
    }
    cerr << "dfs( " << x << ", " << y << ", lim=" << lim << ", dir=" << dirtostr(direct) << ") = " << dirtostr(-1) << endl;
    return -1;
}


int main() {
    string str;
    cin >> str >> ja;

    cerr << ja << endl;

    srand(time(NULL)+ja*1000);


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

	cerr << "--" << endl;        

        int dir = dfs(gs.players[ja].position.x,gs.players[ja].position.y,6,-1);
        switch (dir) {
            case LEFT:
                cout << "cd LEFT\n";
                cerr << "cd LEFT\n";
                continue;
            case UP:
                cout << "cd UP\n";
                cerr << "cd UP\n";
                continue;
            case RIGHT:
                cout << "cd RIGHT\n";
                cerr << "cd RIGHT\n";
                continue;
            case DOWN:
                cout << "cd DOWN\n";
                cerr << "cd DOWN\n";
                continue;
        }
        
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

#pragma clang diagnostic pop
