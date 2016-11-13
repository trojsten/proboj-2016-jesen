#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <queue>

#include "common.h"

using namespace std;

int ja = -1;

const double STRACH = 100;
const double AGRESIVITA = 10;
const double ODSEBA = -100;
const double KAMEN = 5;
const double SPEED = -1000;
const double NAVNADA = 1000;
const double NUM_NAVNAD = 5;
const double MIN_DIST = 2;
const double MAX_DIST = 5;
const double NEPRECHODNE = -10;

int dx[]={-1, 0, 1, 0};
int dy[]={0, 1, 0, -1};


vector<vector<int> > distance(vector<vector<int> >&vstup)
//0-oddrbe ta
//1-neoddrbe ta
//2-domcek
{
    int a, b, c, na, nb;
    int r = vstup.size(), s=vstup[0].size();
    queue<int> Q;
    
    vector<vector<int> > dist(r, vector<int>(s, -1));
    
    for(int i=0;i<r;++i)
    {
        for(int j=0;j<s;++j)
        {
            if(vstup[i][j] == 2)
            {
                Q.push(i);
                Q.push(j);
                Q.push(0);
                dist[i][j]=0;
            }
        }
    }
    
    while(!Q.empty())
    {
        a=Q.front();Q.pop();
        b=Q.front();Q.pop();
        c=Q.front();Q.pop();
        
            for(int i=0;i<4;++i)
            {
                na = a+dx[i];
                nb = b+dy[i];
                if(na>=0 && nb>=0 && na<r && nb<s && vstup[na][nb] != 0 && dist[na][nb] == -1)
                {
                    Q.push(na);
                    Q.push(nb);
                    Q.push(c+1);
                    dist[na][nb]=c+1;
                }
            }
    }
    
    return dist;
}
void vytrat(vector<vector<double> >&vstup, double konst, int iter)
{
    queue<int>Q;
    double sum;
    int num, ni, nj, r = vstup.size(), s=vstup[0].size();
    vector<vector<double> >odpoved(r, vector<double>(s, 0));

    double konstPren = konst;
    vector<vector<int> > susedia(r,vector<int> (s,0));
    for(int f=0;f<iter;++f)
    {
        
        for(int i = 0;i<r;++i)
        {
            for(int j = 0;j<s;++j)
            {
                num = 0;
                sum = 0.0;
                for(int k = 0;k<4;++k)
                {
                    ni = i+dx[k];
                    nj = j+dy[k];
                    if(ni>=0 && nj>=0 && ni<r && nj<s)
                    {
                        sum += vstup[ni][nj];
                        ++num;
                    }
                }
                susedia[i][j] = num;
                odpoved[i][j] += (sum);
            }
        }
        
        for(int i = 0;i<r;++i)
        {
            for(int j = 0;j<s;++j)
            {
                vstup[i][j] = (vstup[i][j]+konst*odpoved[i][j])/(konst*susedia[i][j]+1);
                odpoved[i][j]=0;
            }
        }
    }
    
}

int main() {
    srand(time(NULL));

    string str;
    cin >> str >> ja;

 //   cerr << ja << endl;

    game_state gs;
	set<pair<int,int> > navnady;
    while (true) {
		nacitaj(cin, gs);
		vector<vector<double> > ohodnotenePolicka(gs.width + 2, vector<double> (gs.height + 2, 0));
		int currentLength = 0;

		int x = gs.players[ja].position.x;
		int y = gs.players[ja].position.y;

		
		if(navnady.size() <= NUM_NAVNAD){
			vector<vector<int> > G(gs.width + 2,vector<int> (gs.height + 2));
			for(int i=0;i<gs.width + 2; ++i){
				G[i][0] = 0;
				G[i][gs.height + 1] = 0;
			}
			for(int i=0;i<gs.height + 2; ++i){
				G[0][i] = 0;
				G[gs.height + 1][i] = 0;
			}
			for (int x = 0; x < gs.width; x++) {
				for (int y = 0; y < gs.height; y++) {
					if (gs.get_block(x, y).type == WALL) G[x + 1][y + 1] = 0;
					else if (gs.get_block(x, y).owned_by == ja){
						G[x + 1][y + 1] = 2;
					}
					else G[x + 1][y + 1] = 1;
				}
			}
			vector<vector<int> > D = distance(G);
			vector<pair<int,int> > PP;
			for (int x = 0; x < gs.width + 2; x++) {
				for (int y = 0; y < gs.height + 2; y++) {
					if(D[x][y] != -1){
						if(D[x][y] >= MIN_DIST && D[x][y] <= MAX_DIST){
							PP.push_back({x,y});
						}
					}
				}
			}
			random_shuffle(PP.begin(),PP.end());
			for(int i=0;i<PP.size() && navnady.size() <= NUM_NAVNAD; ++i){
				navnady.insert(PP[i]);
			}
		}


		for(int i=0;i<gs.width + 2; ++i){
			ohodnotenePolicka[i][0] = NEPRECHODNE;
			ohodnotenePolicka[i][gs.height + 1] = NEPRECHODNE;
		}
		for(int i=0;i<gs.height + 2; ++i){
			ohodnotenePolicka[0][i] = NEPRECHODNE;
			ohodnotenePolicka[gs.height + 1][i] = NEPRECHODNE;
		}

		for (int x = 0; x < gs.width; x++) {
			for (int y = 0; y < gs.height; y++) {
				char c = '.';
				if (gs.get_block(x, y).type == WALL) ohodnotenePolicka[x + 1][y + 1] = NEPRECHODNE;
				else if (gs.get_block(x, y).crossed_by == ja){
					ohodnotenePolicka[x + 1][y + 1] += ODSEBA;
					++currentLength;
				}
				else if (gs.get_block(x, y).crossed_by != -1) ohodnotenePolicka[x + 1][y + 1] += AGRESIVITA;
				else ohodnotenePolicka[x + 1][y + 1] = 0;
				
			}
		}
		for (int x = 0; x < gs.width; x++) {
			for (int y = 0; y < gs.height; y++) {
				if (gs.get_block(x, y).owned_by == ja) ohodnotenePolicka[x + 1][y + 1] += currentLength*STRACH;
			}
		}

		for(auto p: gs.players){
			ohodnotenePolicka[p.position.x + 1][p.position.y + 1] += -STRACH*currentLength*10;
		}

		for(auto p: navnady){
			if(gs.get_block(p.first - 1, p.second - 1).owned_by == ja) navnady.erase(navnady.find({p.first, p.second}));
			ohodnotenePolicka[p.first][p.second] += NAVNADA;
		}
		if(ja==0){
			for(auto p: ohodnotenePolicka){
				for (auto q: p){
					cerr << q << " ";
				}
				cerr << "\n";
			}
			cerr << "--------------------------\n";
		}
		vytrat(ohodnotenePolicka, 0.1, 15);
		vector<pair<double, string> > dirs;
		if (x > 0 && gs.blocks[gs.block_index({x - 1, y})].crossed_by != ja && gs.get_block(x - 1, y).type != WALL){
			dirs.push_back({ohodnotenePolicka[x][y+1],"LEFT"});
		}
		if (x < gs.width - 1 && gs.blocks[gs.block_index({x + 1, y})].crossed_by != ja && gs.get_block(x + 1, y).type != WALL){
			dirs.push_back({ohodnotenePolicka[x + 2][y + 1], "RIGHT"});
		}
		if (y > 0 && gs.blocks[gs.block_index({x, y - 1})].crossed_by != ja && gs.get_block(x, y - 1).type != WALL){
			dirs.push_back({ohodnotenePolicka[x+1][y], "UP"});
		}
		
		if (y < gs.height - 1 && gs.blocks[gs.block_index({x, y + 1})].crossed_by != ja && gs.get_block(x, y + 1).type != WALL){
			dirs.push_back({ohodnotenePolicka[x+1][y+2],"DOWN"});
		}
		sort(dirs.begin(),dirs.end());
		if (dirs.size() > 0) {
			cout << "cd " << dirs[dirs.size() - 1].second << endl;
		}
    }
}
