#include <string>
using namespace std;

#include "klient.h"
#include "util.h"

Klient::Klient () {}

Klient::Klient (string _meno, string _uvodneData, string adresar, string execCommand, string zaznamovyAdresar)
    : meno(_meno), uvodneData(_uvodneData)
{
    vector<string> command;
    command.push_back(execCommand);
    proces.setProperties(command, adresar, zaznamovyAdresar + "/" + meno + ".log");
}

Klient::Klient (string _meno, string _uvodneData, string adresar, string zaznamovyAdresar)
    : Klient(_meno, _uvodneData, adresar, "./hrac", zaznamovyAdresar) {}

void Klient::restartuj () {
    // este nemame policy na to, ako casto mozno resetovat klienta
    // moze byt problem s prilis castym resetovanim
    precitane.clear();
    proces.restartuj();

    posli(uvodneData);
}

string Klient::citaj (unsigned cap) {
    string nove = proces.read(cap);
    int i = nove.size();
    while (i > 0 && nove[i-1] != '\n') {
        // vraciame iba hotovu odpoved, uzavretu znakom noveho riadku
        i--;
    }
    if (i == 0) {
        precitane += nove;
        return "";
    }
    string res = precitane + nove.substr(0,i);
    precitane = nove.substr(i);
    return res;
}

void Klient::posli (string data) {
    proces.write(data);
}

void Klient::zabi () {
    proces.zabi();
}

bool Klient::zije () {
    return proces.zije();
}
