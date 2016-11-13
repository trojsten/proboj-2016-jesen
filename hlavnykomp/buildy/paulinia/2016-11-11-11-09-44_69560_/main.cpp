#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>

#include "common.h"

using namespace std;

#define FOR(i, n) for(int i = 0; i < (int)n; i ++)
#define ii pair<int, int>
#define lli long long int
#define lili pair<lli, lli>
#define infinity 2000000999
#define inff 2000000000000000999

template<class T>
ostream &operator<<(ostream &out, vector<T> &v){
    FOR(i, v.size()){
        out << (i ? " " : "") << v[i];
    }
    out << endl;
    return out;
}

int ja = -1;

vector<string> S = {"LEFT", "UP", "RIGHT", "DOWN"};
int DX[] = {        -1,      0,    1,       0};
int DY[] = {        0,      -1,   0,       1};
game_state gs;

// imagine a magic
// and this is it
// magic constants:
int attack_const = 0; // idem ku enemy
int defense_const = 2; // idem ku mne

vector<vector<int> > state; // 0: UNDEFINED, 1: CONNECTED, -1: ISOLATED

int susedim_so_sebou(int x, int y){
    if(state[x][y]) return state[x][y];
    state[x][y] = -1;
    if(gs.get_block(x, y).owned_by == ja){
        state[x][y] = 1;
        return 1;
    }
    FOR(i, 4){
        if(x + DX[i] < 0 || y + DY[i] < 0 || x + DX[i] >= gs.width || y + DY[i] >= gs.height){
            continue;
        }
        if(gs.get_block(x + DX[i], y + DY[i]).type == WALL || gs.get_block(x + DX[i], y + DY[i]).crossed_by == ja) continue;
        if(susedim_so_sebou(x + DX[i], y + DY[i]) == 1){
            state[x][y] = 1;
            return 1;
        }
    }
    return -1;
}

int defense(int x, int y, int dir){
    
}

int find_direction(int x, int y){
    state.clear();
    state.resize(gs.width, vector<int>(gs.height, 0));
    vector<int> moze;
    FOR(i, 4){
        cerr << "DIR " << S[i] << ":\n";
        if(x + DX[i] < 0 || y + DY[i] < 0 || x + DX[i] >= gs.width || y + DY[i] >= gs.height){
            continue;
        }
        block b = gs.get_block(x + DX[i], y + DY[i]);
        cerr << "Ja som " << ja << " and crossed_by: " << b.crossed_by << "Type: " << b.type << " and Wall = " << WALL << " and poz = [" << x + DX[i] << " ; " << y + DY[i] << endl;
        if(b.type != WALL && b.type != FAST_BONUS && (b.crossed_by == -1 || b.crossed_by != ja)){
            if(susedim_so_sebou(x + DX[i], y + DY[i]) == 1) moze.push_back(i);
        }
    }
    if(moze.size()){
        int gener = rand() % moze.size();
        cerr << "Generated " << gener << " from: " << moze << "-> " << S[moze[gener]] << endl;
        return moze[gener];
    }
    return rand() % 4;
}

int main() {
    string str;
    cin >> str >> ja;

    cerr << ja << endl;

    srand(time(NULL) + ja);


    while (true) {
        nacitaj(cin, gs);

        int x = gs.players[ja].position.x;
        int y = gs.players[ja].position.y;
        
        auto res = S[find_direction(x, y)];
        cerr << "RES = " << res << endl;
        
        cout << "cd " << res << endl;
    }
}
