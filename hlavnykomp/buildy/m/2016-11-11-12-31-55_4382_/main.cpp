#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <queue>
#include <tuple>
#include <vector>

#include "common.h"
using namespace std;

typedef point point;

static point get_point(int x, int y) {
    point p;
    p.x = x;
    p.y = y;
    return p;
}

int INF = 1e9;

int dirs[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
string dir_msg[4] = {"LEFT", "UP", "RIGHT", "DOWN"};
int ja = -1;
game_state gs;

point get_point(int index) {
    return get_point(index / gs.height, index % gs.height);
}

int get_index(point pos) {
    return pos.x * gs.height + pos.y;
}

int game_size() {
    //return get_index(get_point(gs.width - 1, gs.height - 1));
    return gs.blocks.size();
}

bool boundaries(point pos) {
    return (pos.x >= 0 && pos.x < gs.width && pos.y >= 0 && pos.y < gs.height);
}

bool survive(point pos, int player) {
    int ind = get_index(pos);
    return (boundaries(pos) && gs.blocks[ind].crossed_by != player && gs.blocks[ind].type != WALL);
}

void game_plane(vector<int> &p) {
    p.clear();
    p.resize(game_size(), INF);
}

typedef tuple<int,int> bfsPozicia;
void bfs(vector<point> &poss, vector<int> &vzdialenosti, int start_time=0, int player=-2) {
    queue< tuple<int, int> > q;
    for (int i = 0; i < poss.size(); ++i)
        q.push(bfsPozicia(get_index(poss[i]), start_time));
    point pos, pos2;
    int ind, x, t;
    while (!q.empty()) {
        tie(x, t) = q.front();
        vzdialenosti[x] = t;
        q.pop();
        pos = get_point(x);
        for (int d = 0; d < 4; ++d) {
            pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
            ind = get_index(pos2);
            if (survive(pos2, player) && vzdialenosti[ind] == INF) {
                vzdialenosti[ind] = t + 1;
                q.push(bfsPozicia(ind, vzdialenosti[ind]));
            }
        }
    }
}

typedef tuple<int,int,int[2]> bfs_rev_pozicia; // ind time dir
typedef vector<bfs_rev_pozicia> path_list;
void bfsrev(vector<point> &poss, path_list &vzdialenosti, int start_time=0) {
//    queue<bfs_rev_pozicia> q;
//    for (int i = 0; i < poss.size(); ++i)
//        q.push({get_index(poss[i]), start_time, {0, 0}});
//    point pos, pos2;
//    int ind, ind2, x, t, dir[2];
//    while (!q.empty()) {
//        tie(ind, t, dir) = q.front();
//        vzdialenosti[ind] = make_tuple(-1, t, {dir[0], dir[1]});
//        q.pop();
//        pos = get_point(ind);
//        for (int d = 0; d < 4; ++d) {
//            pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
//            ind2 = get_index(pos2);
//            if (boundaries(pos2) && get<0>(vzdialenosti[ind2]) == INF && gs.blocks[ind2].type != WALL) {
//                get<0>(vzdialenosti[ind2]) = t + 1;
//                q.push(bfs_rev_pozicia(ind, get<0>(vzdialenosti[ind2]), {pos.x-pos2.x, pos.y-pos2.y});
//            }
//        }
//    }
}

int main() {
    srand(time(NULL));

    string str;
    cin >> str >> ja;

    cerr << ja << endl;

    int ind;
    point pos, pos2, pos_prev=get_point(-1, -1);
    vector<int> dist_enemy, dist_home, safe;
    vector<point> home, enemies;
    while (true) {
        cerr << "START!!! ROUND: " << gs.round << endl;
        nacitaj(cin, gs);
        cerr << "NACITANE" << endl;

        pos = gs.players[ja].position;
        int x = pos.x;
        int y = pos.y;

//        for (int x = 0; x < gs.width; x++) {
//            for (int y = 0; y < gs.height; y++) {
//                char c = '.';
//                if (gs.get_block(x, y).type == WALL)
//                    c = '#';
//                else if (gs.get_block(x, y).crossed_by == ja)
//                    c = 'A';
//                else if (gs.get_block(x, y).crossed_by != -1)
//                    c = 'B';
//                cerr << c << " ";
//            }
//            cerr << endl;
//        }
//        cerr << endl;

        enemies.clear();
        home.clear();
        for (int i = 0; i < game_size(); ++i) {
            if (gs.blocks[i].type == WALL)
                continue;
            else if (gs.blocks[i].owned_by == ja)
                home.push_back(get_point(i));
            else if (gs.blocks[i].crossed_by != ja && gs.blocks[i].crossed_by != -1)
                enemies.push_back(get_point(i));
        }
        cerr << "ROZDELENE" << endl << endl;

        game_plane(dist_enemy);
        game_plane(dist_home);
        game_plane(safe);

        bfs(enemies, dist_enemy);
        bfs(home, dist_home, 1, ja);
        for (int i = 0; i < game_size(); ++i)
            safe[i] = dist_enemy[i] > dist_home[i] || gs.blocks[i].owned_by == ja;

        cerr << x << " " << y << endl;
        for (int r = 0; r < gs.height; ++r) {
            for (int c = 0; c < gs.width; ++c) {
                ind = get_index(get_point(c, r));
                cerr << safe[ind] << " ";
            }
            cerr << endl;
        }
        cerr << endl << endl;


        vector<string> hybem;

        for (int d = 0; d < 4; ++d) {
            pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
            if (survive(pos2, ja) && safe[get_index(pos2)]) {
                hybem.push_back(dir_msg[d]);
            }
        }

        if (hybem.size() > 0) {
            string smer = hybem[rand() % hybem.size()];
            cout << "cd " << smer << endl;
            cerr << "HYBEM SA NA " << smer << endl;
        }
        else {
            cerr << "NEMOZEM SA HYBAT" << endl;
        }
    }
}
