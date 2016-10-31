#ifndef UPDATE_H
#define UPDATE_H

// herne konstanty

// o temnych rytieroch
#define CAS_SMRT 45 // sekund
#define CYKLUS_TEMNEJ_KADENCIE 4 // sekund
#define POVODNY_CYKLUS_TEMNYCH 1 // sekund
#define POMALOST_TEMNYCH 100 // tahov
#define JEDNOTIEK_TEMNYCH 50
#define UTOK_TEMNYCH 9

#define VYTRVALOST_VOJAKOV 95 // zo 100
#define RYCHLOST_JEDNOTIEK 2.0
#define POMALOST_RASTU 1

// hlavne server-side konstanty
#define TAH_CAS 20 // v milisekundach
#define CAS_NA_INICIALIZACIU 2000 // ms

#include <string>
#include <sstream>
#include <ostream>
#include <vector>

#include "common.h"

void inicializuj (unsigned pocHrac, stav& stavHry, bool absend) ;

void ulozUmrtia (vector<int>& V) ;

bool odsimulujKolo (stav& stavHry, const vector<string>& odpovede, stringstream& pokrac) ;

#endif
