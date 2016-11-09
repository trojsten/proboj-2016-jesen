#include <fstream>
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

void game_map::load (fstream& mapstream) {
    /* format:
     * sirka vyska
     * 0 = empty
     * 1 = spawn
     */
    mapstream >> width >> height;
    squares.resize(width);
    for (int x = 0; x < width; x++) {
        squares[x].resize(height);
        for (int y = 0; y < height; y++) {
            int val;
            mapstream >> val;
            if (val == 0) {
                squares[x][y] = EMPTY;
            }
            else {
                squares[x][y] = SPAWN;
            }
        }
    }
}
