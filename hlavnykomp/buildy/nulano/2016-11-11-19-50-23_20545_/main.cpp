#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <queue>

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

int dfs(int x, int y, int lim, const int direct, bool empty=true);

int bfs(int x, int y) {
	vector<vector<int>> vis(gs.width);
	for (int i = 0; i < gs.width; i++) vis[i].resize(gs.height,-1);
	queue<point> q;
	point p;p.x=x; p.y=y;
auto pole = gs.get_block(x,y);
static int m=0;
if (pole.owned_by != ja)m++;
else m=0;
	q.push(p);
while (!q.empty()) {
	p = q.front(); q.pop();
 pole = gs.get_block(p.x,p.y);
    if (((p.x!=x||p.y!=y)&&pole.crossed_by == ja) || pole.type == WALL || pole.type == FAST_BONUS) continue;
    if (m<=2 && pole.owned_by != ja) {
break;
}
    if (m>2 && (pole.owned_by == ja || pole.type == STONE_BONUS)) {
break;
}
	for (int i = 0,j=rand()%4; i < 4; i++) {
	switch((i+j)%4) {
case 0:
	if (p.x>1) {p.x--;
	if (vis[p.x][p.y]==-1){q.push(p);
	 vis[p.x][p.y]=LEFT;}p.x++;}
break;
case 1:
	if (p.x<gs.width-2) {p.x++;
	if (vis[p.x][p.y]==-1){q.push(p);
	 vis[p.x][p.y]=RIGHT;}p.x--;}
break;
case 2:
	if (p.x>1) {p.y--;
	if (vis[p.x][p.y]==-1){q.push(p);
	 vis[p.x][p.y]=UP;}p.y++;}
break;
case 3:
	if (p.x<gs.height-2) {p.y++;
	if (vis[p.x][p.y]==-1){q.push(p);
	 vis[p.x][p.y]=DOWN;}p.y--;}
break;
}
}
p.x=-1;
}
if (p.x<0)
return -1;
int g = 0;
while (p.x!=x||p.y!=y) {

switch (g=vis[p.x][p.y]){
case LEFT:
p.x++; break;
case RIGHT:
p.x--; break;
case UP:
p.y++; break;
case DOWN:
p.y--; break;


}



}
return g;

}



int main() {
    string str;
    cin >> str >> ja;

    cerr << ja << " " << __TIME__ << endl;

    srand(time(NULL)+ja*1000);


    while (true) {
	nacitaj(cin, gs);

	int x = gs.players[ja].position.x;
	int y = gs.players[ja].position.y;
/*
	for (int x = 0; x < gs.width; x++) {
	    for (int y = 0; y < gs.height; y++) {
		char c = '.';
		if (gs.get_block(x,y).owned_by == ja) c = ',';
		if (gs.get_block(x, y).type == WALL) c = '#';
		else if (gs.get_block(x, y).crossed_by == ja) c = 'A';
		else if (gs.get_block(x, y).crossed_by != -1) c = 'B';
		if (gs.players[ja].position.x==x &&gs.players[ja].position.y==y) c = '$';
		cerr << c << " ";
	    }
	    cerr << endl;
	}
	cerr << endl;
	cerr << "--" << endl;        
*/

        int dir = bfs(gs.players[ja].position.x,gs.players[ja].position.y);
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
        
	cerr << "RANDOM!!!!!!!!!!!!!"<<endl;

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
