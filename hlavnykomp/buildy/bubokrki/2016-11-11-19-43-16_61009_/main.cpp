#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "common.h"

using namespace std;

int ja = -1;
int dir = 0;
game_state gs;

vector <vector <int> > mapa;
vector <pair <pair <int,int>,int > > bfs; 
vector <vector <bool> > navstivene;
int dx[] = {0,1,-1,0};
int dy[] = {-1,0,0,1};

//ja -> home | type=0 people
//enemy -> ja | type=1 enemy
//ja -> enemy | type=2 enemy
inline int in_front(int x, int y, bool type = false, int people = ja, int enemy = ja, int direction = 4) {
    switch(direction) {
        case 0:
            y -= 1;
            break;
        case 1:
            y += 1;
            break;
        case 2:
            x += 1;
            break;
        case 3:
            x -= 1;
            break;
    }
     
    //cerr << gs.get_block(x,y).owned_by << endl;
    //cerr << "Pozicia: " << x << " " << y <<" Ja: "<< ja << endl;
    
    if(gs.get_block(x,y).type == WALL || gs.get_block(x,y).type == FAST_BONUS || gs.get_block(x,y).crossed_by == enemy) return -1; ///blokada
	///ciele
	else if(gs.get_block(x,y).owned_by == people && !type) return 0; ///people -> home
	else if(gs.get_block(x,y).crossed_by == people && type) return 0; ///enemy -> ja
    else if(gs.get_block(x,y).crossed_by != -1) return 1; 
    return 2;
}
vector <int> cesta;
void go_back(int j) {
    cesta.clear();
    while(bfs[j].second!= -1) {
        int x = bfs[j].first.first;
        int y = bfs[j].first.second;
        j = bfs[j].second;
        int x1 = bfs[j].first.first;
        int y1 = bfs[j].first.second;
        if(x1 != x) {
            if(x1 < x) cesta.push_back(1);
            else cesta.push_back(2); 
        } else {
            if(y1 < y) cesta.push_back(3);
            else cesta.push_back(0);
        
        }
    }
}

void go_home(int x, int y, bool type = false, int people = ja, int enemy = ja) {
    for (int i = 0;i < navstivene.size(); i++) {
        for(int j = 0; j < navstivene[i].size(); j++) {
            if(i == 0 || i == navstivene.size()-1 || j == 0 || j == navstivene[i].size()-1) navstivene[i][j] = 1;
            else navstivene[i][j] = 0;
        }
    }
    bfs.clear();
    bfs.push_back({{x,y},-1});
    for (int j = 0; j < (int)bfs.size();j++){
        //cerr << "Cyklim sa" << endl;
        //cerr << in_front(bfs[j].first.first,bfs[j].first.second) << endl;
        if(in_front(bfs[j].first.first,bfs[j].first.second, type, people, enemy) == 0) {
            go_back(j);
            return;
        } else { 
            //cerr << "Dostal som sa sem :)" << endl;
            navstivene[bfs[j].first.first+1][bfs[j].first.second +1] = 1;
            for (int i = 0; i < 4; i++) {
                //cerr << "Navratova hodnota:" << in_front(bfs[j].first.first+dx[i],bfs[j].first.second+dy[i]) << endl;
                if((!navstivene[bfs[j].first.first+dx[i]+1][bfs[j].first.second+dy[i]+1]) && in_front(bfs[j].first.first+dx[i],bfs[j].first.second+dy[i], type, people, enemy) != -1){
                    bfs.push_back({{bfs[j].first.first+dx[i],bfs[j].first.second+dy[i]},j});
                }
            }
        }
    }
}

int find_me() {
    for (int i = 0;i < navstivene.size(); i++) {
        for(int j = 0; j < navstivene[i].size(); j++) {
            if(i == 0 || i == navstivene.size()-1 || j == 0 || j == navstivene[i].size()-1) navstivene[i][j] = 1;
            else navstivene[i][j] = 0;
        }
    }
    bfs.clear();
    for(int i = 0; i < gs.players.size(); i++) if(i !=ja && gs.players[i].alive) bfs.push_back({{gs.players[i].position.x, gs.players[i].position.y},0});
    for (int j = 0; j < (int)bfs.size();j++){
        //cerr << "Cyklim sa" << endl;
        //cerr << in_front(bfs[j].first.first,bfs[j].first.second) << endl;
        if(in_front(bfs[j].first.first,bfs[j].first.second, true, ja, -1) == 0) {
            return bfs[j].second;
        } else { 
            //cerr << "Dostal som sa sem :)" << endl;
            navstivene[bfs[j].first.first+1][bfs[j].first.second +1] = 1;
            for (int i = 0; i < 4; i++) {
                //cerr << "Navratova hodnota:" << in_front(bfs[j].first.first+dx[i],bfs[j].first.second+dy[i]) << endl;
                if((!navstivene[bfs[j].first.first+dx[i]+1][bfs[j].first.second+dy[i]+1]) && in_front(bfs[j].first.first+dx[i],bfs[j].first.second+dy[i], true, ja, -1) != -1){
                    bfs.push_back({{bfs[j].first.first+dx[i],bfs[j].first.second+dy[i]},bfs[j].second++});
                }
            }
        }
    }
}

void random_dir(int x, int y){
	vector <int> A;
	for(int i = 0; i < 4; i++) if(x+dx[i] >= 0 && x+dx[i] < gs.height && y+dy[i] >= 0 && y+dy[i] < gs.width && in_front(x+dx[i], y+dy[i]) != -1) A.push_back(i);
	dir = A[rand()%A.size()];
	cerr << dir << endl;
}

int main() {
    srand(time(NULL));

    string str;
    cin >> str >> ja;

    cerr << ja << endl;

    nacitaj(cin, gs);
    mapa.resize(gs.width+2);
    navstivene.resize(gs.width+2);
    for (int i = 0; i < gs.width +2; i++) {
        mapa[i].resize(gs.height+2);
        navstivene[i].resize(gs.height+2);
    }
    int pozicia = 0;
    while (true) {
		
		int x = gs.players[ja].position.x;
		int y = gs.players[ja].position.y;
		random_dir(x, y);
		//dir = 0;

		for (int i = 0; i < gs.width; i++) {
			for (int j = 0; j < gs.height; j++) {
				char c = '.';
				if(i == x && y == j) c = 'a';
				else if (gs.get_block(i, j).type == WALL) c = '#';
				else if (gs.get_block(i, j).crossed_by == ja) c = 'A';
				else if (gs.get_block(i, j).crossed_by != -1) c = 'B';
				for(int q = 0; q < gs.players.size(); q++) if(gs.players[q].position.x == i && gs.players[q].position.y == j) if(q !=ja) c = 'b';
				cerr << c << " ";
			}
			cerr << endl;
		}
		cerr << endl;
        
        /*int min_enemy = 9876543;
        for(int i = 0; i < gs.players.size(); i++){
			cerr << i << endl;
			if(i != ja) go_home(gs.players[i].position.x, gs.players[i].position.y, true, ja, i); 
			if(cesta.size() < min_enemy){
				min_enemy = cesta.size();
			}
		}*/
		int minimum = find_me();
		cerr << "minimum " << minimum << '!' <<endl;
		go_home(x,y);
		if(cesta.size() != 0 && cesta.size()+3 > minimum) {// if(cesta.size()+3 > minimum) {//
            dir = cesta.back();
        }
        if(cesta.empty()) pozicia = -1;
        /*for (int i = 0; i < navstivene.size(); i++) {
            for (int j = 0; j < navstivene[i].size(); j++) {
                cerr << navstivene[i][j] << " ";
            }
            cerr << endl;
        }*/
        cerr << endl;
        for (int i = cesta.size()-1; i >= 0; i--) {
            cerr << cesta[i] << " ";
        }
        cerr << endl << endl << endl;
        
        switch (dir) {
            case 0:
                cout << "cd UP";
                break;
            case 1:
                cout << "cd RIGHT";
                break;
            case 2:
                cout << "cd LEFT";
                break;
            case 3:
                cout << "cd DOWN";
                break;
        }
        cout << endl;
		nacitaj(cin, gs);
        pozicia++;
    }
}
