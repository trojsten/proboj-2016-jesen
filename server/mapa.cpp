#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "common.h"
#include "marshal.h"

void nacitajMapu (string filename, stav& S, const int pocHrac) {
	fstream subor(filename.c_str(), fstream::in);
	if (subor.fail()) {
		fprintf(stderr, "nacitanie mapy: neviem otvorit subor: %s\n", filename.c_str());
		exit(EXIT_FAILURE);
	}
	stavAlt salt;
	nacitaj(subor, salt);
	subor.close();

	vector<int> starty;
	for (int i=0; i<(int)salt.mesta.size(); i++) {
		salt.mesta[i].id = i;
		if (salt.mesta[i].vlastnik >= 0) {
			starty.push_back(i);
		}
		salt.mesta[i].vlastnik = -1;
	}
	random_shuffle(starty.begin(), starty.end());
	for (int i=0; i<pocHrac; i++) {
		int kto = starty[i];
		salt.mesta[kto].vlastnik = i;
	}
	S = stav(salt);
}
