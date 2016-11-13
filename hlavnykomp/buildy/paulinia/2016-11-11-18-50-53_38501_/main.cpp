#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <queue>
#include <cmath>

#include "common.h"

using namespace std;

#define FOR(i, n) for(int i = 0; i < (int)n; i ++)
#define ii pair<int, int>
#define lli long long int
#define lili pair<lli, lli>
#define infinity 2000000999
#define inff 2000000000000000999
// #define DBG cout    cer    

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
const long double attack_const = 2; // idem ku nepriatelovmu telu
const long double defense_const = 1; // idem daleko od nepriatelovej hlavy
const long double pud_sebazachovy = 0.5; // idem ku mojmu uzemiu
const long double rozsirovanie_uzemia = 9; // idem prec z mojho uzemia
const int mutation_probab = 100;
const int max_safe_len = 15;
const int min_safe_dist = 5;

int len = 1;

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
    vector<vector<int> > bolo(gs.height, vector<int>(gs.width, -1));
    queue<ii> F;
    F.push({x + DX[dir], y + DY[dir]});
    bolo[x + DX[dir]][y + DY[dir]] = 0;
    while(F.size()){
        int X = F.front().first;
        int Y = F.front().second;
        if(gs.get_block(X, Y).owned_by == ja || gs.get_block(X, Y).type == STONE_BONUS){
            return bolo[X][Y] + 1;
        }
        F.pop();
        FOR(i, 4){
            if(X + DX[i] < 0 || Y + DY[i] < 0 || X + DX[i] >= gs.width || Y + DY[i] >= gs.height) continue;
            if(gs.get_block(X + DX[i], Y + DY[i]).type == WALL || gs.get_block(X + DX[i], Y + DY[i]).type == FAST_BONUS || gs.get_block(X + DX[i], Y + DY[i]).crossed_by == ja) continue;
            
            if(bolo[X + DX[i]][Y + DY[i]] == -1){
                bolo[X + DX[i]][Y + DY[i]] = bolo[X][Y] + 1;
                F.push({X + DX[i], Y + DY[i]});
            }
            
        }
    }
    return infinity;
}

int greedy_factor(int x, int y, int dir){
    vector<vector<int> > bolo(gs.height, vector<int>(gs.width, -1));
    queue<ii> Q;
    bolo[x + DX[dir]][y + DY[dir]] = 0;
    Q.push({x + DX[dir], y + DY[dir]});
    while(Q.size()){
        int X = Q.front().first;
        int Y = Q.front().second;
        Q.pop();
        block b = gs.get_block(X, Y);
        if(b.owned_by != ja){
            return bolo[X][Y] + 1;
        }
        FOR(i, 4){
            if(X + DX[i] < 0 || Y + DY[i] < 0 || X + DX[i] >= gs.width || Y + DY[i] >= gs.height) continue;
            if(gs.get_block(X + DX[i], Y + DY[i]).type == WALL || gs.get_block(X + DX[i], Y + DY[i]).type == FAST_BONUS || gs.get_block(X + DX[i], Y + DY[i]).crossed_by == ja) continue;
            
            if(bolo[X + DX[i]][Y + DY[i]] == -1){
                bolo[X + DX[i]][Y + DY[i]] = bolo[X][Y] + 1;
                Q.push({X + DX[i], Y + DY[i]});
            }
        }
    }
    return infinity;
}

int be_aware(int x, int y, int dir){
    vector<vector<int> > bolo(gs.height, vector<int>(gs.width, -1));
    queue<ii> Q;
    int l = 0;
    for(player P : gs.players){
        if(!P.alive){
            l ++;
            continue;
        }
        if(l == ja){
            l ++;
            continue;
        }
        bolo[P.position.x][P.position.y] = 0;
        Q.push({P.position.x, P.position.y});
        l ++;
    }
    
    while(Q.size()){
        int X = Q.front().first;
        int Y = Q.front().second;
        Q.pop();
        if(gs.get_block(X, Y).crossed_by == ja) return bolo[X][Y] + 1;
        FOR(i, 4){
            if(X + DX[i] < 0 || Y + DY[i] < 0 || X + DX[i] >= gs.width || Y + DY[i] >= gs.height) continue;
            if(gs.get_block(X + DX[i], Y + DY[i]).type == WALL) continue;
            
            if(bolo[X + DX[i]][Y + DY[i]] == -1){
                bolo[X + DX[i]][Y + DY[i]] = bolo[X][Y] + 1;
                Q.push({X + DX[i], Y + DY[i]});
            }
        }
    }
    return infinity;
}

int be_dangerous(int x, int y, int dir){
    vector<vector<int> > bolo(gs.height, vector<int>(gs.width, -1));
    queue<ii> Q;
    bolo[x + DX[dir]][y + DY[dir]] = 0;
    Q.push({x + DX[dir], y + DY[dir]});
    while(Q.size()){
        int X = Q.front().first;
        int Y = Q.front().second;
        Q.pop();
        block b = gs.get_block(X, Y);
        if(b.crossed_by != ja && b.crossed_by != -1){
            return bolo[X][Y] + 1;
        }
        
        FOR(i, 4){
            if(X + DX[i] < 0 || Y + DY[i] < 0 || X + DX[i] >= gs.width || Y + DY[i] >= gs.height) continue;
            if(gs.get_block(X + DX[i], Y + DY[i]).type == WALL || gs.get_block(X + DX[i], Y + DY[i]).type == FAST_BONUS || gs.get_block(X + DX[i], Y + DY[i]).crossed_by == ja) continue;
            
            if(bolo[X + DX[i]][Y + DY[i]] == -1){
                bolo[X + DX[i]][Y + DY[i]] = bolo[X][Y] + 1;
                Q.push({X + DX[i], Y + DY[i]});
            }
        }
    }
    return infinity;
}

long double compute_score(long double PS, long double RU, long double A, long double D){ //IMAGINE a MAGIC
    cerr << len << " > " << max_safe_len << " ? " << D << " < " << min_safe_dist << endl;
    if(len > max_safe_len){
        return pow(PS, -pud_sebazachovy);
    }
    else if(D < min_safe_dist){
        cerr << "[" << D << " < " << min_safe_dist << "]";
        return -1.0/D + pow(PS, -pud_sebazachovy);
    }
    else{
        cerr << "res = " << pow(PS, - pud_sebazachovy) << " + " << pow(A, -attack_const) << " + " << pow(RU, -(rozsirovanie_uzemia)) << endl;
        return pow(PS, - pud_sebazachovy) + pow(A, -attack_const) + pow(RU, -(rozsirovanie_uzemia));
    }
}

int compute_direction(vector<int> moze, vector<int> PS, vector<int> RU, vector<int> A, int D){
    int minit = rand() % PS.size();
    FOR(i, PS.size()){
        cerr << "DIR " << S[moze[i]] << " A PS[i] = " << PS[i] << " a RU[i] = " <<  RU[i] << " A[i] = " << A[i] << " D = " << D 
             << " score: " << compute_score(PS[i], RU[i], A[minit], D) << endl;
        if(compute_score(PS[i], RU[i], A[i], D) > compute_score(PS[minit], RU[minit], A[minit], D)) minit = i;
    }
    if(rand() % mutation_probab == 0) return moze[rand() % moze.size()];
    cerr << "minit = " << minit << " and score: " << compute_score(PS[minit], RU[minit], A[minit], D);
    return moze[minit];
}

int find_direction(int x, int y){
    state.clear();
    state.resize(gs.width, vector<int>(gs.height, 0));
    len = 0;
    FOR(i, gs.width){
        FOR(j, gs.height){
            len = len + (gs.get_block(i, j).crossed_by == ja ? 1 : 0);
        }
    }
    cerr << "LEN = " << len << endl;
    vector<int> moze;
    FOR(i, 4){
        if(x + DX[i] < 0 || y + DY[i] < 0 || x + DX[i] >= gs.width || y + DY[i] >= gs.height){
            continue;
        }
        block b = gs.get_block(x + DX[i], y + DY[i]);
        if(b.type != WALL && b.type != FAST_BONUS && (b.crossed_by == -1 || b.crossed_by != ja)){
            if(susedim_so_sebou(x + DX[i], y + DY[i]) == 1) moze.push_back(i);
        }
    }
    cerr << "WTF" << endl;
    if(moze.size()){
        vector<int> PS;
        vector<int> RU;
        vector<int> A;
        int D = be_aware(x, y, 0);
        FOR(i, moze.size()){
            PS.push_back(defense(x, y, moze[i]));
            RU.push_back(greedy_factor(x, y, moze[i]));
            A.push_back(be_dangerous(x, y, moze[i]));
        }
        cerr << A;
        int res = compute_direction(moze, PS, RU, A, D);
        return res;
    }
    return rand() % 4;
}

int main() {
    string str;
    cin >> str >> ja;

    cerr << "Ja som " << ja << endl;

    srand(time(NULL) + ja);


    while (true) {
        nacitaj(cin, gs);

        int x = gs.players[ja].position.x;
        int y = gs.players[ja].position.y;
        
        auto res = S[find_direction(x, y)];
        
        cerr << gs.round << " -> " << res << endl;
        
        cout << "cd " << res << endl;
    }
}
