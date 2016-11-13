// Proboj_rozsirovanie_uzemia.cpp : Defines the entry point for the console application.
//

#include "common.h"

#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

#define CHODVON 0
#define CHODSPAT 1
#define KDEVON 2
#define VON 3

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
    int I;
    point vysiel;
    string smery[4] = { "UP","RIGHT","DOWN","LEFT" };
    int smer;
    int smerVon = 0;
    point bodVon;
    int maxCestaDomov = gs.height / 5;
    int objective = 2;
    int x_move;
    int y_move;
    while (true)
    {
        nacitaj(cin, gs);
        for(int i = 0; i<gs.height*gs.width; i++)
        {
            if(gs.get_block(from_index(gs, i)).owned_by == ja && vzdialenostBodu(gs.players[ja].position, from_index(gs, i)) < vzdialenostBodu(gs.players[ja].position, bodVon)) bodVon = from_index(gs,i);
        }
        switch (objective)
        {
        case 0:
            for (int i = 0; i < gs.players.size(); i++)
            {
                if (gs.players[i].alive && (vzdialenostBodu(vysiel, gs.players[i].position), vzdialenostBodu(gs.players[ja].position, gs.players[i].position)) <= cestaDomov - 1)
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
                    for(int x = -1; x < 2; i += 2)
                    {
                        if(gs.get_block(gs.players[ja].position.x + x, gs.players[ja].position.y).type == STONE_BONUS) smer = (1 + x) % 4;
                    }
                    for(int y = -1; y < 2; y += 2)
                    {
                        if(gs.get_block(gs.players[ja].position.x, gs.players[ja].position.y + y).type == STONE_BONUS) smer = (4 + y) % 4;
                    }
                    cestaDomov++;
                }
            }
            break;
        case 1:
            if (gs.get_block(gs.players[ja].position).owned_by == ja) objective = KDEVON;
            if (bodVon.x < gs.players[ja].position.x) smer = 3;
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
        cout << "cd " << smery[smer] << "\n";
        cerr << objective << "..." << I << "\n\n";
        I++;
    }
    return 0;
}

