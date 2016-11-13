#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <stack>
#include <queue>
#include "common.h"

using namespace std;

string topleft[8] = {"cd UP", "cd UP","cd LEFT", "cd LEFT", "cd DOWN", "cd DOWN", "cd RIGHT", "cd UP"};
string topright[8] = {"cd UP", "cd UP","cd RIGHT", "cd RIGHT", "cd DOWN", "cd DOWN", "cd LEFT", "cd UP"};
string botleft[8] = {"cd DOWN", "cd DOWN","cd LEFT", "cd LEFT", "cd UP", "cd UP", "cd RIGHT", "cd DOWN"};
string botright[8] = {"cd DOWN", "cd DOWN","cd RIGHT", "cd RIGHT", "cd UP", "cd UP", "cd LEFT", "cd DOWN"};
string* current;

game_state gs;

bool checkwall(int x, int y, int w, int h){
    if ((x < 0) or (y < 0) or (x >= w) or (y >= h)){
        return true;
    }else if(gs.get_block(x,y).type == WALL){return true;}
    else return false;
}

bool checktopleft(int x, int y,int w, int h){
    if((checkwall(x-1,y-2,w,h) or checkwall(x,y-2,w,h) or checkwall(x-2,y,w,h) or checkwall(x-2, y-1,w,h) or checkwall(x-2,y-2,w,h)) == false){
        return true;         
    }else return false;
}
bool checktopright(int x, int y,int w, int h){
    if((checkwall(x+2,y-2,w,h) or checkwall(x,y-2,w,h) or checkwall(x+2,y,w,h) or checkwall(x+2, y-1,w,h) or checkwall(x+1,y-2,w,h)) == false){
        return true;         
    }else return false;
}
bool checkbotleft(int x, int y,int w, int h){
    if((checkwall(x-2,y,w,h) or checkwall(x,y+2,w,h) or checkwall(x-2,y+1,w,h) or checkwall(x-2, y+2,w,h) or checkwall(x-1,y+2,w,h)) == false){
        return true;         
    }else return false;
}
bool checkbotright(int x, int y,int w, int h){
    if((checkwall(x,y+2,w,h) or checkwall(x+1,y+2,w,h) or checkwall(x+2,y+2,w,h) or checkwall(x+2, y+1,w,h) or checkwall(x+2,y,w,h)) == false){
        return true;         
    }else return false;
}

string* findpath(int x, int y,int w, int h, int &cis){
    cerr<<11111<<endl;
    
    srand(time(NULL));
    while (true){       
    
    if(cis==0 and checktopleft(x,y,w,h)){return topleft; break;}
    if(cis==1 and checktopright(x,y,w,h)){return topright; break;}
    if(cis==2 and checkbotright(x,y,w,h)){return botright; break;}
    if(cis==3 and checkbotleft(x,y,w,h)){return botleft; break;}
    cis=(cis+1)%4;
    }
}
int ja = -1; //mne bude pridelene nejake cislo

int main() {
    srand(time(NULL));
    int p=0;
    string str;
    cin >> str >> ja;
    int cislo=0;
    cerr << ja << endl;
    while (true) {
 
	    nacitaj(cin, gs);

	    int x = gs.players[ja].position.x;
	    int y = gs.players[ja].position.y;  
        cerr<<p<<endl;
        p = p % 8;       
        if(p == 0){
            current = findpath(x,y,gs.width,gs.height,cislo);
        }  
        cout << current[p] << endl;
        p++;
   
    }
    return 0;
}
