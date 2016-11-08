#include "mapa.h"

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
