// Proboj_rozsirovanie_uzemia.cpp : Defines the entry point for the console application.
//

#include "common.h"

#include <bits/stdc++.h>
using namespace std;

#define CHODVON 0
#define CHODSPAT 1
#define KDEVON 2
#define VON 3

int GRA(int ja, string s){
	cerr << "Presiel som" << endl;
	cout << "cd " << s << endl; 
  cerr.precision(4);
  srand(time(NULL));
  cerr << ja << endl;
  game_state gs;
	int retardovane_dlhy_nazov_lebo_mozem = 4;
  while (retardovane_dlhy_nazov_lebo_mozem){
		cerr << "som while cykle" << endl;
		retardovane_dlhy_nazov_lebo_mozem--;
    cerr << ja << endl;
    nacitaj(cin, gs);
    int x = gs.players[ja].position.x;
		int y = gs.players[ja].position.y;
    vector<vector<double>> gravity;
    gravity.resize(gs.width);
    for(int i = 0; i < gravity.size(); i++){
      gravity[i].resize(gs.height);
    }
    for(int i = 0; i < gravity.size(); i++){
      for(int j = 0; j < gravity[i].size(); j++){
        if(gs.get_block(i, j).type == WALL){
          gravity[i][j] = -20;
        }
        else if(gs.get_block(i,j).crossed_by != -1 && gs.get_block(i, j).crossed_by != ja){
          gravity[i][j] = 10;
        }
        else if(gs.get_block(i,j).crossed_by == ja){
          gravity[i][j] = 8;
        }
        else{
          gravity[i][j] = 5;
        }
      }
    }
		for(int i = 0; i < gravity.size(); i++){
      for(int j = 0; j < gravity[i].size(); j++){
        if(gs.get_block(i, j).owned_by == ja){
          gravity[i][j] += 10;
        }
      }
    }
    for(int i = 0; i < gs.players.size(); i++){
      if(i != ja){
				if(gs.players[i].alive == 1){
					//cerr << gs.players[i].position.x << " " << gs.players[i].position.y << endl;
        	gravity[gs.players[i].position.x][gs.players[i].position.y] = -50;
	      }
			}
    }/*
    for(int i = 0; i < gravity.size(); i++){
      for(int j = 0; j < gravity[i].size(); j++){
        cerr << gravity[i][j] << " ";
      }
      cerr << endl;
    }
    cerr << endl;*/
    for(int i = 0; i < 4; i++){
      for(int a = 0; a < gravity.size(); a++){
        for(int b = 0; b < gravity[a].size(); b++){
					if(i%2 == 1){
						a = gravity.size()-a-1;
						b = gravity[a].size()-b-1;
					}
          if(gravity[a][b] == -20 || gravity[a][b] == -50 || gravity[a][b] == 15){
						if(i%2 == 1){
							a = gravity.size()-a-1;
							b = gravity[a].size()-b-1;
						}
            continue;
          }
					
          double sucet = gravity[a][b];
          double pocet = 1;
          if(a > 0){
            if(gravity[a-1][b] != double(-20)){
              pocet += 1;
              sucet += gravity[a-1][b];
            }
          }
          if(b > 0){
            if(gravity[a][b-1] != double(-20)){
              pocet += 1;
              sucet += gravity[a][b-1];
            }
          }
          if(a < gravity.size()-1){
            if(gravity[a+1][b] != double(-20)){
              pocet += 1;
              sucet += gravity[a+1][b];
            }
          }
          if(b < gravity[a].size()-1){
            if(gravity[a][b+1] != double(-20)){
              pocet += 1;
              sucet += gravity[a][b+1];
            }
          }
          gravity[a][b] = sucet / double(pocet);
					if(i%2 == 1){
						a = gravity.size()-a-1;
						b = gravity[a].size()-b-1;
					}
        }
      }
    }
		cerr << "nezdrbal som psa" << endl;/*
    for(int i = 0; i < gravity.size(); i++){
      for(int j = 0; j < gravity[i].size(); j++){
        cerr << gravity[i][j] << " ";
      }
      cerr << endl;
    }
    cerr << endl;*/
		queue<pair<int, int>> hladaj;
		vector<vector<bool>> presiel(gs.width);
    for(int i = 0; i < gravity.size(); i++){
      presiel[i].resize(gs.height, 0);
    }
		hladaj.push(make_pair(gs.players[ja].position.x,gs.players[ja].position.y));
		while(!hladaj.empty()){
			pair<int, int> akt;
			akt = hladaj.front();
			hladaj.pop();
			double rozdiel = double(abs(akt.first - gs.players[ja].position.x) + abs(akt.second - +gs.players[ja].position.y));
			if(rozdiel-8 > 0 || presiel[akt.first][akt.second] == 1){
				continue;
			}
			presiel[akt.first][akt.second] = 1;
			//cerr << "ich bin wichtig und frohlich" << endl;
			double val = gravity[gs.players[ja].position.x][gs.players[ja].position.y];
			if(akt.first > 0 && presiel[akt.first-1][akt.second] != 1){
				if(!((gravity[akt.first-1][akt.second]+val)/2 < val-0.1)){// ich rozdiel policka a zaciatku je mensi ako 2 pri vah.priemere
					hladaj.push(make_pair(akt.first-1, akt.second));
				}
			}
			if(akt.second > 0 && presiel[akt.first][akt.second-1] != 1){
				if(!((gravity[akt.first][akt.second-1]+val)/2 < val-0.1)){// ich rozdiel policka a zaciatku je mensi ako 2 pri vah.priemere
					hladaj.push(make_pair(akt.first, akt.second-1));
				}
			}
			if(akt.first < gs.height-1 && presiel[akt.first+1][akt.second] != 1){
				if(!((gravity[akt.first+1][akt.second]+val)/2 < val-0.1)){// ich rozdiel policka a zaciatku je mensi ako 2 pri vah.priemere
					hladaj.push(make_pair(akt.first+1, akt.second));
				}
			}
			if(akt.second < gs.width-1 && presiel[akt.first][akt.second+1] != 1){
				if(!((gravity[akt.first][akt.second+1]+val)/2 < val-0.1)){// ich rozdiel policka a zaciatku je mensi ako 2 pri vah.priemere
					hladaj.push(make_pair(akt.first, akt.second+1));
				}
			}
		}
		vector<pair<double, string>> dir(4);
		for(int i = 0; i < presiel.size(); i++){
			for(int j = 0; j < presiel[i].size(); j++){
				cerr << presiel[i][j] << " ";
			}
			cerr << endl;
		}
		
		dir[0].second = "LEFT";
		dir[1].second = "RIGHT";
		dir[2].second = "UP";
		dir[3].second = "DOWN";
		for(int i = gs.players[ja].position.x; i < gs.width; i++){
			for(int j = 0; j < gs.height; j++){
				if(presiel[i][j] == 1){
					dir[0].first += 1;//hybe sa vlavo
				}
			}
		}
		for(int i = gs.players[ja].position.x; i > 0; i--){
			for(int j = 0; j < gs.height; j++){
				if(presiel[i][j] == 1){
					dir[1].first += 1;//hybe sa vpravo
				}
			}
		}
		for(int i = 0; i < gs.width; i++){
			for(int j = gs.players[ja].position.y; j < gs.height; j++){
				if(presiel[i][j] == 1){
					dir[2].first += gravity[i][j];//hybe sa dole
				}
			}
		}
		for(int i = 0; i < gs.width; i++){
			for(int j = gs.players[ja].position.y; j > 0; j--){
				if(presiel[i][j] == 1){
					dir[3].first += 1;//hybe sa hore
				}
			}
		}
		sort(dir.begin(), dir.end());
		for(int i = 0; i < dir.size(); i++){
			cerr << dir[i].first << " ";
		}
		cerr << endl;
		cerr << x << " " << y << endl;
		cerr << endl << endl;
		vector<string> dirs;
		for(int i = 3; i > -1; i--){
			if (dir[i].second == "LEFT" && x > 0 && gs.blocks[gs.block_index({x - 1, y})].crossed_by != ja && gravity[x-1][y] != -20)
   	  	dirs.push_back("LEFT");
			if (dir[i].second == "RIGHT" && x < gs.width - 1 && gs.blocks[gs.block_index({x + 1, y})].crossed_by != ja && gravity[x+1][y] != -20)
      	dirs.push_back("RIGHT");
			if (dir[i].second == "UP" && y > 0 && gs.blocks[gs.block_index({x, y - 1})].crossed_by != ja && gravity[x][y-1] != -20)
     	  dirs.push_back("UP");
			if (dir[i].second == "DOWN" && y < gs.height - 1 && gs.blocks[gs.block_index({x, y + 1})].crossed_by != ja && gravity[x][y+1] != -20)
  	    dirs.push_back("DOWN");
		}
		if(retardovane_dlhy_nazov_lebo_mozem == 0){
			if(dirs[0] == "LEFT"){
				return 3;
			}
			if(dirs[0] == "RIGHT"){
				return 1;
			}
			if(dirs[0] == "UP"){
				return 0;
			}
			if(dirs[0] == "DOWN"){
				return 2;
			}
		}
		if (dirs.size() > 0) {
 	  	cout << "cd " << dirs[0] << endl;
		}
  }
}



int vzdialenostBodu(point hrac, point bod) {return max(hrac.x - bod.x, bod.x - hrac.x) + max(hrac.y - bod.y, bod.y - hrac.y);}

//string obidStenu(point hrac, point pohyb, game_state gs, int ja)
//{
//    int x_direct = pohyb.x - hrac.x;
//    int y_direct = pohyb.y - hrac.y;
//    while (gs.get_block(pohyb).type == WALL || gs.get_block(pohyb).crossed_by == ja)
//    {
//        pohyb.x += x_direct;
//        pohyb.y += y_direct;
//    }
//    point miesto;
//    miesto.x = pohyb.x;
//    miesto.y = pohyb.y;
//    while (true) //dolava
//    {
//        if (!(gs.get_block(miesto.x - x_direct, miesto.y - y_direct).type == WALL || gs.get_block(miesto.x - x_direct, miesto.y - y_direct).crossed_by == ja))
//        {
//           miesto.x -= x_direct;
//            miesto.y -= y_direct;
//        }
//        else if (!(gs.get_block(miesto.x - y_direct, miesto.y - x_direct).type == WALL || gs.get_block(miesto.x - y_direct, miesto.y - x_direct).crossed_by == ja))
//        {
//            miesto.y -= x_direct;
//            miesto.x -= y_direct;
//        }
//    }
//}

point from_index(game_state gs, int i) {
    return {i / gs.width, i % gs.width};
}

int main()
{
    int ja;
    string str;
    cin >> str >> ja;
    cerr << ja << endl;
    game_state gs;


    int cestaDomov = 0;
    int I = 0;
    point vysiel;
    string smery[4] = { "UP","RIGHT","DOWN","LEFT" };
    int smer;
    int smerVon = 0;
    point bodVon;
    int maxCestaDomov = gs.height / 5;
    int objective = 2;
    int x_move;
    int y_move;
		int nearestPlayer = 500;
    while (true)
    {
				bool volajFunkciu = false;
				nearestPlayer = 500;
        nacitaj(cin, gs);
        for(int i = 0; i<gs.height*gs.width; i++)
        {
            if(gs.get_block(from_index(gs, i)).owned_by == ja && vzdialenostBodu(gs.players[ja].position, from_index(gs, i)) < vzdialenostBodu(gs.players[ja].position, bodVon)) bodVon = from_index(gs,i);
        }
				for (int i = 0; i < gs.players.size(); i++)
				{
						if (gs.players[i].alive) nearestPlayer = min(nearestPlayer, vzdialenostBodu(gs.players[ja].position, gs.players[i].position));
				}
        switch (objective)
        {
        case 0:
            if (nearestPlayer <= cestaDomov - 1)
            {
                srand(time(NULL));
                smer = (smerVon + (rand()%2 + 1)*2 - 1) % 4;
                objective = CHODSPAT;
                break;
            }
            else if (cestaDomov >= maxCestaDomov)
            {
                smer = (smerVon + (rand()%2 + 1)*2 - 1) % 4;
                objective = CHODSPAT;
                break;
            }
            else
            {
                smer = smerVon;
                for(int x = -1; x < 2; x += 2)
                {
                    if(gs.get_block(gs.players[ja].position.x + x, gs.players[ja].position.y).type == STONE_BONUS) smer = (1 + x) % 4;
                }
                for(int y = -1; y < 2; y += 2)
                {
                    if(gs.get_block(gs.players[ja].position.x, gs.players[ja].position.y + y).type == STONE_BONUS) smer = (4 + y) % 4;
                }
                cestaDomov++;
            }
            break;
        case 1:
            if (gs.get_block(gs.players[ja].position).owned_by == ja) objective = KDEVON;
            else if (bodVon.x < gs.players[ja].position.x) smer = 3;
            else if (bodVon.x > gs.players[ja].position.x) smer = 1;
            else if (bodVon.y < gs.players[ja].position.y) smer = 0;
            else smer = 2;
          	break;
        case 2: {
          	pair<int, int> vacsiMensiX{ 0,0 };
          	pair<int, int> vacsiMensiY{ 0,0 };
	          for (int i = 0; i < gs.players.size(); i++)
	          {
			            if (gs.players[i].alive && gs.players[i].position.x > gs.players[ja].position.x) vacsiMensiX.first++;
	              else vacsiMensiX.second++;
	              if (gs.players[i].alive && gs.players[i].position.y > gs.players[ja].position.y) vacsiMensiY.first++;
	              else vacsiMensiY.second++;
	          }
	          srand(time(NULL));
	          vacsiMensiX.first += rand()%5;
	          srand(time(NULL)+1);
	          vacsiMensiX.second += rand()%5;
	          srand(time(NULL)+2);
	          vacsiMensiY.first += rand()%5;
	          srand(time(NULL)+3);
	          vacsiMensiY.second += rand()%5;
	          if (vacsiMensiX.first > vacsiMensiX.second) x_move = -1;
	          else x_move = 1;
	          if (vacsiMensiY.first > vacsiMensiY.second) y_move = -1;
	          else y_move = 1;
	          objective = VON;
						if (nearestPlayer < 5 || gs.players[ja].position.x == 0 || gs.players[ja].position.x == gs.width - 1 || gs.players[ja].position.y == 0 || gs.players[ja].position.y == gs.height-1)
						{
								volajFunkciu = true;
								objective = CHODSPAT;
						}
        }
        case 3:
            srand(time(NULL));
            if (!rand()%5)
            {
                smer = 2 - y_move;
                smerVon = 2 - y_move;
                bodVon = gs.players[ja].position;
                objective = CHODVON;
            }
            else if (gs.get_block(gs.players[ja].position.x + x_move, gs.players[ja].position.y).owned_by == ja) smer = (3 - x_move) % 4;
            else if(gs.get_block(gs.players[ja].position.x, gs.players[ja].position.y + y_move).owned_by == ja) smer = 2 - y_move;
            else
            {
                smer = 2 - y_move;
                smerVon = 2 - y_move;
                bodVon = gs.players[ja].position;
                objective = CHODVON;
            }
        default:
            break;
        }
        cerr << smer << " " << smerVon << "\n";
        while(true)
        {
            bool hlava = false;
            point ciel = gs.players[ja].position;
            switch (smer)
            {
            case 0:
                ciel.y -= 1;
                break;
            case 1:
                ciel.x += 1;
                break;
            case 2:
                ciel.y += 1;
                break;
            case 3:
                ciel.x -= 1;
                break;
            default:
                break;
            }
            for(int i = 0; i<gs.players.size(); i++)
            {
                if(gs.players[i].alive && gs.players[i].position == ciel) hlava = true;
            }
            if (gs.get_block(ciel).type == WALL || (gs.get_block(ciel).type == FAST_BONUS) || gs.get_block(ciel).crossed_by == ja || ciel.x == -1 || ciel.x == gs.width || ciel.y == -1 || ciel.y == gs.height || hlava)
                {
                    srand(time(NULL));
                    smer = (smer + (rand()%2 + 1)*2 - 1) % 4;
                    objective = CHODSPAT;
                }
            
            else break;
        }
				if(volajFunkciu)
				{
						smer = GRA(ja, smery[smer]);
				}
        cout << "cd " << smery[smer] << "\n";
        cerr << objective << "..." << I << "\n\n";
        I++;
    }
    return 0;
}

