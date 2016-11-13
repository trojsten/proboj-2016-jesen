#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <queue>
#include <tuple>
#include <vector>

#include "common.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
using namespace std;


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
    return (boundaries(pos) && gs.blocks[ind].crossed_by != player &&
            gs.blocks[ind].type != WALL && gs.blocks[ind].type != FAST_BONUS);
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
        q.pop();
        if(vzdialenosti[x]!= INF)
            continue;
        vzdialenosti[x] = t;
        pos = get_point(x);
        for (int d = 0; d < 4; ++d) {
            pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
            if(survive(pos2, player))
                q.push(bfsPozicia(get_index(pos2), t + 1));
        }
    }
}

typedef tuple<int,int> bfsPozicia;
void bfs_home(vector<point> &poss, vector<int> &vzdialenosti,
              vector<int> dist_enemies, vector<int> dist_home, int min_dist_from_player,
              int start_time=0, int player=-2) {
    queue< tuple<int, int> > q;
    for (int i = 0; i < poss.size(); ++i)
        q.push(bfsPozicia(get_index(poss[i]), start_time));
    point pos, pos2;
    int ind, x, t;
    while (!q.empty()) {
        tie(x, t) = q.front();
        q.pop();
        if(vzdialenosti[x]!= INF)
            continue;
        vzdialenosti[x] = t;
        pos = get_point(x);
        for (int d = 0; d < 4; ++d) {
            pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
            ind = get_index(pos2);
            if(survive(pos2, player) &&
                    dist_home[ind] < dist_enemies[ind] &&
                    dist_home[ind] < min_dist_from_player)
                q.push(bfsPozicia(ind, t + 1));
        }
    }
}

int main() {
    srand(time(NULL));

    string str;
    cin >> str >> ja;
    cerr << ja << endl;

    int ourind, ind;
    string dir_prev=dir_msg[1];
    point pos, pos2, pos_prev=get_point(-1, -1);
    vector<int> dist_enemy, dist_etail, dist_home, dist_safe, safe, dist_outer;
    vector<point> home, enemies, etails, not_homo;
    while (true) {
        nacitaj(cin, gs);
        cerr << "START!!! ROUND: " << gs.round << endl;

        pos_prev = pos;
        pos = gs.players[ja].position;
        ourind = get_index(pos);
        cerr<<"POSITION: " << pos.x <<" "<<pos.y<<endl;

        enemies.clear();
        etails.clear();
        home.clear();
        not_homo.clear();
        for (int i = 0; i < game_size(); ++i) {
            if (gs.blocks[i].type == WALL || gs.blocks[i].type == FAST_BONUS)
                continue;
            else {
                pos2 = get_point(i);
                if (gs.blocks[i].owned_by == ja)
                    home.push_back(pos2);
                else if (gs.blocks[i].owned_by != ja)
                    not_homo.push_back(pos2);

                if (gs.blocks[i].crossed_by != ja && gs.blocks[i].crossed_by != -1)
                    etails.push_back(pos2);
            }
        }
        for (int i = 0; i < gs.players.size(); ++i)
            if (i != ja && gs.players[i].alive)
                enemies.push_back(gs.players[i].position);

        game_plane(dist_enemy);
        game_plane(dist_etail);
        game_plane(dist_home);
        game_plane(dist_safe);
        game_plane(safe);
        game_plane(dist_outer);

        int min_distance_from_player = INF;

        bfs(enemies, dist_enemy, -1);
        bfs(etails, dist_etail, 0, ja);
        bfs(home, dist_home, 0, ja);
        bfs(not_homo, dist_outer, 0, ja);
        min_distance_from_player = min(min_distance_from_player, dist_enemy[get_index(pos)]);
        for (int i = 0; i < game_size(); ++i) {
            if (gs.blocks[i].crossed_by == ja) {
                min_distance_from_player = min(min_distance_from_player, dist_enemy[i]);
            }

        }
        bfs_home(home, dist_safe, dist_enemy, dist_home, min_distance_from_player, 0, ja);

        for (int i = 0; i < game_size(); ++i) {
            safe[i] = survive(get_point(i), ja) && (dist_safe[i] != INF || gs.blocks[i].owned_by == ja) &&
                      dist_home[i] < min_distance_from_player;
        }

//        cerr << "Dist from player:" << min_distance_from_player << endl;
//        cerr << "Dist from homer:" << dist_home[get_index(pos)] << endl;
//        for (int r = 0; r < gs.height; ++r) {
//            for (int c = 0; c < gs.width; ++c) {
//                ind = get_index(get_point(c, r));
//                cerr << safe[ind] << " ";
//            }
//            cerr << endl;
//        }
//        cerr << endl << endl;
//
//        for (int r = 0; r < gs.height; ++r) {
//            for (int c = 0; c < gs.width; ++c) {
//                ind = get_index(get_point(c, r));
//                cerr << dist_enemy[ind] << " ";
//            }
//            cerr << endl;
//        }
//        cerr << endl << endl;

//        for (int r = 0; r < gs.height; ++r) {
//            for (int c = 0; c < gs.width; ++c) {
//                ind = get_index(get_point(c, r));
//                cerr << dist_outer[ind] << " ";(
//            }
//            cerr << endl;
//        }
//        cerr << endl << endl;

        vector<string> hybem;
        vector<string> zijemenebezpecne;

        for (int d = 0; d < 4; ++d) {
            pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
            if (survive(pos2, ja) && safe[get_index(pos2)]) {
                hybem.push_back(dir_msg[d]);
            }
            if (survive(pos2, ja))
                zijemenebezpecne.push_back(dir_msg[d]);
        }
        string smer="";
        if (hybem.size() > 0) {
            int min_dist_to_attack = 3;
            if (dist_etail[ourind] < min_dist_to_attack && min_distance_from_player > min_dist_to_attack * 1.5) {
                int mini = INF;
                for (int d = 0; d < 4; ++d) {
                    pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
                    if (survive(pos2, ja) && dist_etail[get_index(pos2)] < mini) {
                        mini = dist_etail[get_index(pos2)];
                        smer = dir_msg[d];
                    }
                }
            }
            else if (gs.blocks[ourind].owned_by == ja && rand() % 4 && dist_enemy[ourind] > 3) {
                int mini = INF;
                for (int d = 0; d < 4; ++d) {
                    pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
                    if (survive(pos2, ja) && dist_outer[get_index(pos2)] < mini) {
                        mini = dist_outer[get_index(pos2)];
                        smer = dir_msg[d];
                    }
                }
            }
            else {
                int siz = hybem.size();
                for (int d = 0; d < siz; ++d) {
                    if (hybem[d] == dir_prev) {
                        hybem.push_back(dir_prev);
                        hybem.push_back(dir_prev);
                    }
                }
                smer = hybem[rand() % hybem.size()];
            }
            cerr << "HYBEM SA NA " << smer << endl;
        } else {
            int mini = INF;
            for (int d = 0; d < 4; ++d) {
                pos2 = get_point(pos.x + dirs[d][0], pos.y + dirs[d][1]);
                if (survive(pos2, ja) && dist_home[get_index(pos2)] < mini) {
                    mini = dist_home[get_index(pos2)];
                    smer = dir_msg[d];
                }
            }
            if (smer == "")
                smer = zijemenebezpecne[rand() % zijemenebezpecne.size()];
            cerr << "NEMOZEM SA HYBAT -> " << smer << endl;

        }
        if (smer == "")
            smer = dir_prev;
        cout << "cd " << (dir_prev = smer) << endl;
        cerr << endl;
        cout.flush();
        cerr.flush();
    }
}
#pragma clang diagnostic pop