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

int find_direction(int x, int y){
    vector<int> moze;
    FOR(i, 4){
        cerr << "DIR " << S[i] << ":\n";
        if(x + DX[i] < 0 || y + DY[i] < 0 || x + DX[i] >= gs.width || y + DY[i] >= gs.height){
            continue;
        }
        block b = gs.get_block(x + DX[i], y + DY[i]);
        cerr << "Ja som " << ja << " and crossed_by: " << b.crossed_by << "Type: " << b.type << " and Wall = " << WALL << " and poz = [" << x + DX[i] << " ; " << y + DY[i] << endl;
        if(b.type != WALL && (b.crossed_by == -1 || b.crossed_by != ja)){
            moze.push_back(i);
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
