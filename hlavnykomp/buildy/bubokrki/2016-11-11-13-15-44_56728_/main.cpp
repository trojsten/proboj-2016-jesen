#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>

#include "common.h"

using namespace std;

int ja = -1;

string chceck_wall(string dir_akt, int x, int y, int hranicaX, int hranicaY)
{
    if( (dir_akt == "RIGHT") and (x == hranicaX - 41) ) return "RIGHT";
    else if ( (dir_akt == "LEFT") and (x == 0) ) return "LEFT";
    else if ( (dir_akt == "DOWN") and (y == hranicaY - 1) ) return "DOWN";
    else if ( (dir_akt == "UP") and (y == 0) ) return "UP";
    else return "NONE";

}

int main() {
    srand(time(NULL));

    string str;
    cin >> str >> ja;

    cerr << ja << endl;

    game_state gs;
    cerr << "NACITAL SOM" << endl;
    nacitaj(cin, gs);
    cerr << "NACITAL SOM" << endl;
    
    //prvy direction je k najblizsiemu rohu

    int x = gs.players[ja].position.x;
    int y = gs.players[ja].position.y;
    
    cerr << "NACITAL SOM POZICIU" << endl;

    string dir_akt;
    string wall = "NONE";

    // RD RU LU LD
    /*
    map <string, string> opposite;
    opposite.insert( pair<string, string> ("RIGHT", "LEFT") );
    opposite.insert( pair<string, string> ("RIGHT", "LEFT") );
    opposite.insert( pair<string, string> ("RIGHT", "LEFT") );
    opposite.insert( pair<string, string> ("RIGHT", "LEFT") );
    */
    char*  dir_realization [] [6] = {   {"RIGHT", "DOWN", "LEFT", "DOWN", "RIGHT", "UP"},
                                        {"RIGHT", "UP", "LEFT", "UP", "RIGHT", "DOWN"},
                                        {"LEFT", "UP", "RIGHT", "UP", "LEFT", "DOWN"},
                                        {"LEFT", "DOWN", "RIGHT", "DOWN", "LEFT", "UP"},
                                    };

    //Defs 
    int dir_index_left;
    int dir_index_right = 5;
    int strana_stvorca = 3;
    int krokov_do_zmeny = strana_stvorca;
    int until_collided;
    
    //Prve urcenie smeru
    cerr << "INIT ENDED" << endl;
    
    if(x >= gs.width / 2) {
        dir_index_left = 0;
        if(y < gs.height / 2) dir_index_left += 1;
    }

    else {
        dir_index_left = 2;
        if(y >= gs.height / 2) dir_index_left += 1;
    }
    
    dir_akt = dir_realization[dir_index_left][dir_index_right];

    //Pocitanie optimalnej velkosti stvorca TODO
    // krokov_do_zmeny = strana_stvorca;
    
    cerr << "IDEM DO WHILE" << endl;

    while (true) {
        //cerr << "ROBIA SA VECI" << endl;
        //cerr << "NACITAL SOM" << endl;

        x = gs.players[ja].position.x;
        y = gs.players[ja].position.y;
        wall = "NONE";
        
        krokov_do_zmeny--;

        if(krokov_do_zmeny == 1){
            dir_index_right++;
            dir_index_right = dir_index_right % 6;
            dir_akt = dir_realization[dir_index_left][dir_index_right];
            krokov_do_zmeny = strana_stvorca;
        }

        wall = chceck_wall(dir_akt, x, y, gs.width, gs.height);
        if(wall != "NONE"){
            cerr << "STENAAAA" << endl;
            krokov_do_zmeny = strana_stvorca;
            dir_index_right++;
            dir_index_right = dir_index_right % 6;
            dir_akt = dir_realization[dir_index_left][dir_index_right];

        }
        cout << "cd " << dir_akt << endl;
        

        nacitaj(cin, gs);
    }   
}
