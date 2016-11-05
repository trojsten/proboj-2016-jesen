#include <vector>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <algorithm>

using namespace std;

#include "util.h"
#include "common.h"
#include "update.h"
#include "klient.h"
#include "mapa.h"
#include "marshal.h"

#define MAX_CITAJ 1024

vector<Klient> klienti;

/* Komment z minuleho proboja. Mozno to chceme lepsie.
// tato trapna funkcia existuje len kvoli inicializujSignaly()
// btw, takto sa signal handling nerobi
// len sa s tym nechceme babrat.
*/
void zabiKlientov() {
    fprintf(stderr, "ukoncujem klientov\n");
    for (unsigned i=0; i<klienti.size(); i++) {
	klienti[i].zabi();
    }
}

template<class T> void checkOstream(T& s, string filename) {
    if (s.fail()) {
	fprintf(stderr, "neviem zapisovat do %s\n", filename.c_str());
	zabiKlientov();
	exit(1);
    }
}

bool validchar (char ch) {
    if ((ch >= '0' && ch <= '9') || (ch == '/') || (ch == '-') || (ch == '_')) {
	return false;
    }
    return true;
}

string last_valid_substr(string s) {
    int r = (int)s.size() - 1;
    while (r > 0 && !validchar(s[r - 1])) {
	r--;
    }
    if (r == 0 || (r < (int)s.size() &&s[r] != '/')) {
	r++;
    }
    int l = r - 1;
    while (l > 0 &&s[l - 1] != '/') {
	l--;
    }
    string last = s.substr(l, r - l);
    return last;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
	fprintf(stderr, "usage: %s <zaznamovy-adresar> <mapa> <adresare-klienta-1> <adresar-klienta-2> ...\n", argv[0]);
	return 0;
    }
  
    unsigned int seed = time(NULL) * getpid();
    srand(seed);
    loguj("startujem server, seed je %u", seed);
    inicializujSignaly(zabiKlientov);

    string zaznAdr(argv[1]);
    if (!jeAdresar(zaznAdr)) {
	if (mkdir(zaznAdr.c_str(), 0777)) {
	    fprintf(stderr, "main/mkdir: %s: %s\n", zaznAdr.c_str(), strerror(errno));
	    exit(EXIT_FAILURE);
	}
    } else {
	fprintf(stderr, "main: prepisujem zaznamovy adresar: %s\n", zaznAdr.c_str());
    }
    
    string obsubor = zaznAdr+"/observation";
    fstream observationstream(obsubor.c_str(), fstream::out | fstream::trunc);
    checkOstream(observationstream, "observation");
	
    { // spermutujeme poradie hracov, dame im dobre mena, a farby
	// a vytvorime ich, kazdemu nastavime uvodne data, ...
	
	random_shuffle(argv + 3, argv + argc);
	set<string> uzMena;

	int pocet_hracov = argc - 3;
	observationstream << pocet_hracov << endl;
	
	for (int i = 3; i < argc; i++) {
	    string klientAdr(argv[i]);
	    string meno = last_valid_substr(klientAdr);

	    // meno klienta je cast za poslednym /, za ktorym nieco je
	    bool dajNahodnuFarbu = false;
	    while (uzMena.count(meno)) {
		dajNahodnuFarbu = true;
		meno += "+";
	    }
	    uzMena.insert(meno);
	    // posleme klientovi, kolkaty v poradi je
	    string uvodneData = "hrac " + to_string(i - 4) + "\n";

	    klienti.push_back(Klient(meno, uvodneData, klientAdr, zaznAdr));

	    string farba;
	    if (dajNahodnuFarbu) {
		for (int i = 0; i < 3; i++) {
		    double cl = (9 + (double)(rand() % (2 * 9)) ) / (4 * 9);
		    farba += to_string(cl) + " ";
		}
		farba += "1.0";
	    } else {
		string clsubor = klientAdr + "/color";
		fstream clstream(clsubor.c_str(), fstream::in);
		getline(clstream, farba);
		clstream.close();
	    }

	    observationstream << meno << " " << farba << endl;
	}
	
	for (unsigned k=0; k<klienti.size(); k++) {
	    klienti[k].restartuj();
	}
    }

    // ABSENT: nacitame mapu

    // ABSENT: zakoduje pociatocny stav a posle ho
    // potom pocka chvilu --- cas na predpocitanie
    usleep(1000 * 1000ll);

    // PRIKLAD toho, ako moze prebiehat komunikacia
    // medzi serverom a klientom
    long long lasttime = gettime();

    bool koncim = true;
    while (!koncim) {
	while (gettime() - lasttime < 1000) {
	    // fetchujeme spravy klientov, ale este nesimulujeme kolo
	    for (unsigned k = 0; k < klienti.size(); k++) {
		if (!klienti[k].zije()) {
		    klienti[k].restartuj();
		    // klientovi posleme relevantne data
		    // klienti[k].posli("blablabla");
		    continue;
		}
		// nacitame to co nam poslal klient a nieco s tym...
		// stringstream riadky(klienti[k].citaj(MAX_CITAJ));
	    }
	}
	lasttime = gettime();

	// odsimulujeme kolo

	// dame do zaznamu co treba
	// observationstream << "blablabla" << flush;
    }
	
    // cleanup
    observationstream.close();
    zabiKlientov();

    // ABSENT: (nizsie uvedene)
    // vypis casy umrtia jednotlivych hracov
    // alebo nieco ine, podla coho hodnotit
    ofstream rankstream((zaznAdr+"/rank").c_str());
    checkOstream(rankstream, zaznAdr+"/rank");
    // tu by nieco malo byt
    rankstream.close();

    // +- info o dlzke hry
    loguj("");

    return 0;
}
