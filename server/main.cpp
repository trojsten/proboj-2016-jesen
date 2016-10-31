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
    }
    else {
        fprintf(stderr, "main: prepisujem zaznamovy adresar: %s\n", zaznAdr.c_str());
    }
    
    string obsubor = zaznAdr+"/observation";
    fstream observationstream(obsubor.c_str(), fstream::out | fstream::trunc);
    checkOstream(observationstream, "observation");
	
    { // spermutujeme poradie hracov, dame im dobre mena, a farby
        string metainfo;
        
        random_shuffle(argv + 4, argv + argc);
        set<string> uzMena;
            
        for (int i=4; i<argc; i++) {
            string klientAdr(argv[i]);
            // meno klienta je cast za poslednym /, za ktorym nieco je
            int r = (int) klientAdr.size() - 1;
            while (r > 0 && !validchar(klientAdr[r - 1])) {
                r--;
            }
            if (r == 0 || (r < klientAdr.size() && klientAdr[r] != '/')) {
                r++;
            }
            int l = r - 1;
            while (l > 0 && klientAdr[l-1] != '/') {
                l--;
            }
            string meno = klientAdr.substr(l, r - l);
            bool dajNahodnuFarbu = false;
            while (uzMena.count(meno)) {
                dajNahodnuFarbu = true;
                meno += "+";
            }
            uzMena.insert(meno);
            string uvodneData = "hrac " + toString<int>(i - 4) + "\n";
            klienti.push_back(Klient(meno, uvodneData, klientAdr, zaznAdr));

            string riadok;
            if (dajNahodnuFarbu) {
                for (int i = 0; i < 3; i++) {
                    double cl = (9 + (double)(rand() % (2 * 9)) ) / (4 * 9);
                    riadok += toString<double>(cl) + " ";
                }
                riadok += "1.0";
            }
            else {
                string clsubor = klientAdr + "/color";
                fstream clstream(clsubor.c_str(), fstream::in);
                getline(clstream, riadok);
                clstream.close();
            }
            metainfo += meno + " " + riadok + "\n";
        }
        string metasubor = zaznAdr+"/meta";
		fstream metastream(metasubor.c_str(), fstream::out | fstream::trunc);
		metastream << metainfo << "end\n";
        metastream.close();
        
        for (unsigned k=0; k<klienti.size(); k++) {
            klienti[k].restartuj();
        }
    }

    // ABSENT: nacitame mapu

    // ABSENT: zakoduje pociatocny stav a posle ho
    // potom pocka chvilu --- cas na predpocitanie
    usleep(CAS_NA_INICIALIZACIU*1000ll);

    bool koncim = false;
    while (!koncim) {
        string resetAns;
        {
            stringstream temp;
            koduj(temp, stavAlt(stavHry));
            resetAns = temp.str();
        }
        while (gettime() - ltime < TAH_CAS) {
            for (unsigned k=0; k<klienti.size(); k++) {
            if (!klienti[k].zije() && (klienti[k].getMeno() != "observer")) {
                klienti[k].restartuj();
                klienti[k].posli(resetAns + "end\n");
                continue;
            }
            stringstream riadky(klienti[k].citaj(MAX_CITAJ));
            odpovedaj(k, riadky, resetAns);
            }
        }
        ltime = gettime();
            
        stringstream pokracovanieHistorie;
        koncim = odsimulujKolo(stavHry, kodpovede, pokracovanieHistorie);
        for (unsigned k=0; k<klienti.size(); k++) {
            kodpovede[k].clear();
        }
        historia += pokracovanieHistorie.str();

        observationstream << pokracovanieHistorie.str() << flush;
    }
	
    // cleanup
    observationstream.close();
    zabiKlientov();

    // ABSENT: (nizsie uvedene)
    // vypis casy umrtia jednotlivych hracov
    // alebo nieco ine, podla coho hodnotit
    ofstream rankstream((zaznAdr+"/rank").c_str());
    checkOstream(rankstream, zaznAdr+"/rank");
    rankstream.close();

    // +- info o dlzke hry
    loguj("");

    return 0;
}
