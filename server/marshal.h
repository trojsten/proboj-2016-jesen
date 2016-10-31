#ifndef MARSHAL_H
#define MARSHAL_H

#include <iostream>
#include <vector>
#include <deque>
#include <set>

#include "common.h"

template<class T> void uloz (std::ostream& out, const T& in) ;
template<class T> void nacitaj (std::istream& in, T& out) ;

template<class T> void uloz(std::ostream& out, const std::vector<T>& in) {
  int size = in.size();
  uloz(out, size);
  for (int i = 0; i < size; i++) {
    uloz(out, in[i]);
  }
}
template<class T> void nacitaj(std::istream& in, std::vector<T>& out) {
  int size;
  nacitaj(in, size);
  if (in.fail()) return;
  out.clear();
  out.reserve(std::min(std::max(size, 0), 100000));
  for (int i = 0; i < size; i++) {
    out.resize(i+1);
    nacitaj(in, out[i]);
  }
}

template<class T> void koduj (std::ostream& out, const T& in) {
	out << in.nazovtyp() << " ";
	uloz(out, in);
	out << "\n";
}

bool dekodujStav (std::istream& in, stav& out) ;

#endif

