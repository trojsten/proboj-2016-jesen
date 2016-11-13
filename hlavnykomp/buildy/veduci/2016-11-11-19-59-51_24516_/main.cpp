#include <iostream>
#include <queue>

#include "common.h"

using namespace std;

struct block_state {
	bool good;
	int enemy_distance;
	int my_area_distance;
	int my_snake_distance;
};

vector<block_state> calc_states(int me, game_state &gs) {
	vector<block_state> result(gs.width * gs.height, {true, 999, 999, 0});

	// good
	for (int i = 0; i < gs.blocks.size(); i++) {
		result[i].good = gs.blocks[i].type != WALL && gs.blocks[i].type != FAST_BONUS && gs.blocks[i].crossed_by != me;
	}

	queue<pair<point, int>> q;
	vector<bool> v(gs.width * gs.height);

	// enemy distance
	for (int i = 0; i < v.size(); i++) v[i] = false;
	for (int i = 0; i < gs.players.size(); i++) {
		if (i == me || !gs.players[i].alive) continue;
		q.push({gs.players[i].position, 0});
	}
	while (!q.empty()) {
		point p = q.front().first;
		int d = q.front().second;
		q.pop();

		int i = gs.block_index(p);

		if (p.x < 0 || p.x >= gs.width || p.y < 0 || p.y >= gs.height) continue;
		if (v[i]) continue;
		v[i] = true;

		result[i].enemy_distance = min(result[i].enemy_distance, d);

		q.push({{p.x - 1, p.y}, d + 1});
		q.push({{p.x + 1, p.y}, d + 1});
		q.push({{p.x, p.y - 1}, d + 1});
		q.push({{p.x, p.y + 1}, d + 1});
	}

	// my area distance
	for (int i = 0; i < v.size(); i++) v[i] = false;
	for (int x = 0; x < gs.width; x++) {
		for (int y = 0; y < gs.height; y++) {
			if (gs.get_block({x, y}).owned_by == me) {
				q.push({{x, y}, 0});
			}
		}
	}
	while (!q.empty()) {
		point p = q.front().first;
		int d = q.front().second;
		q.pop();

		int i = gs.block_index(p);

		if (p.x < 0 || p.x >= gs.width || p.y < 0 || p.y >= gs.height) continue;
		if (v[i]) continue;
		v[i] = true;

		result[i].my_area_distance = min(result[i].my_area_distance, d);

		q.push({{p.x - 1, p.y}, d + 1});
		q.push({{p.x + 1, p.y}, d + 1});
		q.push({{p.x, p.y - 1}, d + 1});
		q.push({{p.x, p.y + 1}, d + 1});
	}

	// my snake distance
	for (int i = 0; i < v.size(); i++) v[i] = false;
	for (int x = 0; x < gs.width; x++) {
		for (int y = 0; y < gs.height; y++) {
			if (gs.get_block({x, y}).crossed_by == me) {
				q.push({{x, y}, 0});
			}
		}
	}
	while (!q.empty()) {
		point p = q.front().first;
		int d = q.front().second;
		q.pop();

		int i = gs.block_index(p);

		if (p.x < 0 || p.x >= gs.width || p.y < 0 || p.y >= gs.height) continue;
		if (v[i]) continue;
		v[i] = true;

		result[i].my_snake_distance = max(result[i].my_snake_distance, d);

		q.push({{p.x - 1, p.y}, d + 1});
		q.push({{p.x + 1, p.y}, d + 1});
		q.push({{p.x, p.y - 1}, d + 1});
		q.push({{p.x, p.y + 1}, d + 1});
	}

	return result;
}

int main() {
	srand(time(NULL));

	string tmp;
	int me;

	cin >> tmp >> me;

	game_state gs;

	int phase = 0;

	while (true) {
		nacitaj(cin, gs);

		int x = gs.players[me].position.x;
		int y = gs.players[me].position.y;

		vector<block_state> b = calc_states(me, gs);

		int danger = min(1000 - gs.round, 64);
		for (int x = 0; x < gs.width; x++) {
			for (int y = 0; y < gs.height; y++) {
				int i = gs.block_index({x, y});
				if (gs.blocks[i].crossed_by == me) {
					danger = min(danger, b[i].enemy_distance);
				}
			}
		}

		cerr << danger << endl;

		block_state here = b[gs.block_index(gs.players[me].position)];

		vector<point> cands;
		vector<int> score;

		for (point p: vector<point>{{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}}) {
			if (p.x < 0 || p.x >= gs.width || p.y < 0 || p.y >= gs.height) continue;
			int i = gs.block_index(p);
			if (!b[i].good) continue;
			cands.push_back(p);
			score.push_back(0);
		}

		if (phase == 0) {
			cerr << "go out" << endl;
			// go out
			for (int p = 0; p < cands.size(); p++) {
				int i = gs.block_index(cands[p]);
				score[p] = b[i].my_area_distance * 999 + b[i].my_snake_distance;
			}

			if (!(here.my_snake_distance < 16 && here.my_area_distance < danger / 2)) phase++;
		} else if (phase == 1) {
			cerr << "circle" << endl;
			// circle
			for (int p = 0; p < cands.size(); p++) {
				int i = gs.block_index(cands[p]);
				score[p] = b[i].my_snake_distance * 999 + b[i].my_area_distance;
			}

			if (!(here.my_snake_distance < 16 && here.my_area_distance < danger)) phase++;
		} else {
			cerr << "go back" << endl;
			// go back
			for (int p = 0; p < cands.size(); p++) {
				int i = gs.block_index(cands[p]);
				score[p] = -b[i].my_area_distance * 999 + b[i].my_snake_distance;
			}

			if (here.my_area_distance == 0) phase = 0;
		}

		int best_score = -99999999;
		for (int p = 0; p < cands.size(); p++) {
			best_score = max(best_score, score[p]);
		}
		for (int p = 0; p < cands.size(); p++) {
			if (best_score > 0) {
				if (score[p] == best_score) {
					if (cands[p].x == x - 1) cout << "cd LEFT" << endl;
					if (cands[p].x == x + 1) cout << "cd RIGHT" << endl;
					if (cands[p].y == y - 1) cout << "cd UP" << endl;
					if (cands[p].y == y + 1) cout << "cd DOWN" << endl;
					break;
				}
			} else {
				p = rand() % cands.size();
				if (cands[p].x == x - 1) cout << "cd LEFT" << endl;
				if (cands[p].x == x + 1) cout << "cd RIGHT" << endl;
				if (cands[p].y == y - 1) cout << "cd UP" << endl;
				if (cands[p].y == y + 1) cout << "cd DOWN" << endl;
				break;
			}
		}
	}
}
