#include <iostream>
#include <string>

#include "common.h"

using namespace std;

int main() {
    game_state gs;

    string tmp;
    int hrac;
    cin >> tmp >> hrac;

    while (true) {
	nacitaj(cin, gs);
        cout << "cd " << "LEFT" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "LEFT" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "LEFT" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "LEFT" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "LEFT" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "UP" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "UP" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "UP" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "UP" << endl;
	nacitaj(cin, gs);
        cout << "cd " << "UP" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "RIGHT" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "RIGHT" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "RIGHT" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "RIGHT" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "RIGHT" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "DOWN" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "DOWN" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "DOWN" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "DOWN" << endl;
	nacitaj(cin, gs);
	cout << "cd " << "DOWN" << endl;
    }
}
