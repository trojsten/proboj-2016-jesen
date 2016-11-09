#include <fstream>
#include "mapa.h"

#define chyba(...) (fprintf(stderr, __VA_ARGS__), false)

game_map::game_map(int width, int height) {
    this->width = width;
    this->height = height;
    this->squares.resize(width);
    for (int x = 0; x < width; x++) {
	this->squares[x].resize(height);
	for (int y = 0; y < height; y++) {
	    this->squares[x][y] = EMPTY;
	}
    }
}

bool game_map::load(string filename) {
  FILE *in = fopen(filename.c_str(), "r");
  if (!in) return chyba("neviem citat '%s'\n", filename.c_str());

  if (fgetc(in) != 'P') return chyba("'%s' ma zly format, chcem raw PPM\n", filename.c_str());
  if (fgetc(in) != '6') return chyba("'%s' ma zly format, chcem raw PPM\n", filename.c_str());

  // podporujeme komentare len medzi headerom a zvyskom (aj ked PPM standard umoznuje skoro kdekolvek)
  char c;
  fscanf(in, " %c", &c);
  while (c == '#') {
    while (c != '\n') c = fgetc(in);
    fscanf(in, " %c", &c);
  }
  ungetc(c, in);

  unsigned w, h, maxval;
  fscanf(in, "%u%u%u", &w, &h, &maxval);
  fgetc(in);
  if (maxval != 255) return chyba("'%s' ma zlu farebnu hlbku, podporujem len 24bpp\n", filename.c_str());

  width = w;
  height = h;
  squares.clear();
  squares.resize(w);
  for (unsigned x = 0; x < w; x++) {
      squares[x].resize(h);
  }

  for (unsigned y = 0; y < h; y++) {
    for (unsigned x = 0; x < w; x++) {
      int r = fgetc(in);
      int g = fgetc(in);
      int b = fgetc(in);
      if (r == EOF || g == EOF || b == EOF) return chyba("necakany EOF pri citani '%s'\n", filename.c_str());
      
      if (r == 255 && g == 255 && b == 255) squares[x][y] = EMPTY;
      else if (r == 0 && g == 255 && b == 0) squares[x][y] = SPAWN;
      else return chyba("zla farba %d,%d,%d na pozicii %d,%d v '%s'\n", r, g, b, x, y, filename.c_str());
    }
  }

  fclose(in);
  return true;
}
