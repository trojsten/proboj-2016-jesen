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
    
    if(gs.get_block(x,y).type == WALL || gs.get_block(x,y).crossed_by == enemy) return -1; ///blokada
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

void random_dir(int x, int y){
	int random = rand()%4;
	cerr << random << endl;
	if(x+dx[random] >= 0 && x+dx[random] < gs.height && y+dy[random] >= 0 && y+dy[random] < gs.width && in_front(x+dx[random], y+dy[random]) != -1){
		dir = random;
		cerr << "true" << endl;
		switch (dir) {
            case 0:
                cerr << "cd UP";
                break;
            case 1:
                cerr << "cd RIGHT";
                break;
            case 2:
                cerr << "cd LEFT";
                break;
            case 3:
                cerr << "cd DOWN";
                break;
        }
        cerr << endl;
	}
	else random_dir(x,y);
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
		go_home(x,y);
        if(cesta.size() != 0 && pozicia > 4) {
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
