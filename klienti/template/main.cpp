#include <iostream>
#include <unistd.h>
#include <algorithm>

using namespace std;

#include "common.h"
#include "marshal.h"

int ja; // cislo hraca
stav S; // stav hry

void prikaz (int od, int kam, int jednotiek) {
	invAlt inva(-1, -1, -1, od, kam, jednotiek);
	cout << "invazia ";
	uloz(cout,inva);
	cout << "\n";
}

// main() zavola tuto funkciu, ked chce vediet, aky prikaz chceme vykonat
// co tato funkcia rozhodne pomocou toho, ako nastavi prikaz;
void zistiTah() {
  vector<int> moje, ine; 
  for (unsigned i=0; i<S.mesta.size(); i++) {//najdeme si vsetky nasa a vsetky cudzie mesta
		if (S.mesta[i].vlastnik == ja) {
			moje.push_back(i);
		}
		else {
			ine.push_back(i);
		}
	}
	if (ine.size() == 0) { //asi sme uz vyhrali
		return;
	}
	//cerr << "moje mesta a ich populacie:\n";
	int ciel = ine[ rand()%ine.size() ]; //zvolime si nahodny ciel
	for (unsigned i=0; i<moje.size(); i++) {
		int kto = moje[i];
		//cerr << kto << " " << S.mesta[kto].zistiPop() << "\n";
		int jednotiek = S.mesta[kto].zistiPop()/2;
		prikaz(kto, ciel, jednotiek);
	}
}

int main() {
  // v tejto funkcii su vseobecne veci, nemusite ju menit (ale mozte).

  unsigned seed = time(NULL) * getpid();
  srand(seed);
  fprintf(stderr, "START pid=%d, seed=%u\n", getpid(), seed); //aby ste vedeli restartovat s rovnakym randomom
  fprintf(stderr, "START pid=%d\n", getpid());

	string temp;
	cin >> temp;
	if (temp == "hrac") {
		cin >> ja;
	}
	else {
		cerr << "cakal som cislo hraca, dostal som ale toto: " << temp << "\n";
		return 1;
	}

	dekodujStav(cin,S);
	zistiTah();

  while (cin.good()) {
		bool este = true;
		while (cin.good() && este) {
			cout << "changeDesc\n";
			este = !dekodujStav(cin,S);
		}
		zistiTah();
	}

  return 0;
}
