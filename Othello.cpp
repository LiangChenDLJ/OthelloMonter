#include "Othello.h"

PlayRev::PlayRev() {

}

const int Othello::BLACK = -1;
const int Othello::WHITE = 1;
const int Othello::EMPTY = 0;
const int Othello::DRAW = 3;
const int Othello::PLAYING = 0;
const int Othello::DIRSET[8][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 },{ 0,1 },{ 1,-1 },{ 1,0 },{ 1,1 } };

Othello::Othello() {
	// initialize the board
	boardstate = PLAYING;
	turn = BLACK;
	board(3, 3) = board(4, 4) = WHITE;
	board(3, 4) = board(4, 3) = BLACK;
	updateplayset();
}

int Othello::randomplay() {
	if (boardstate != PLAYING) return boardstate;
	return play(playset[rand() % playset.size()]);
}

int Othello::play(PlayRev playrev) {
	// uppdate the positon
	int pos = playrev.pos;
	board(pos) = turn;
	
	// reverse chesses accordingly
	for (int j = 0; j < 8; j++) {
		int posoff = to1d(DIRSET[j][0], DIRSET[j][1]);
		for (int k = 0; k < playrev.revset[j]; k++) {
			board(pos + posoff * (k + 1)) *= -1;
		}
	}
	
	//reverse the turn
	turn = -turn;
	
	updateplayset();
	// check which turn it is and if the game is over	
	if (playset.size() == 0) {
		turn = -turn;
		updateplayset();
		if (playset.size() == 0) {
			int res = 0;
			for (int i = 0; i < 64; i++) {
				res += board(i);
			}
			if (res * BLACK > 0) boardstate = BLACK;
			else if (res * WHITE > 0) boardstate = WHITE;
			else boardstate = DRAW;
		}
	}
	return boardstate;
}

int Othello::play(int pos) {
	for (int i = 0; i < playset.size(); i++) {
		if (playset[i].pos == pos) {
			board(pos) = turn;
			return play(playset[i]);
		}
	}
	cout << "Invalid play" << endl;
	return boardstate;
}


int Othello::showboard() {
	int tmpboard[64] = {};
	for (int i = 0; i < 64; i++) {
		tmpboard[i] = board(i);
	}
	for (int i = 0; i < playset.size(); i++) {
		tmpboard[playset[i].pos] = 2;
	}
	cout << "  01234567 " << endl;
	cout << " |--------|" << endl;
	for (int i = 0; i < 8; i++) {
		cout << i << "|";
		for (int j = 0; j < 8; j++) {
			int val = tmpboard[to1d(i, j)];
			if (val == BLACK) cout << "@";
			else if (val == WHITE) cout << "O";
			else if (val == 2) cout << "*";
			else cout << " ";
		}
		cout << "|" << i << endl;
	}
	cout << " |--------|" << endl;
	cout << "  01234567 " << endl;
	return 0;
}

void Othello::updateplayset() {
	int j, pos, dirx, diry, tposx, tposy, color, revnum, isvalid;
	int revset[8];
	PlayRev* playrev;
	playset.clear();
	// for each position check once
	for (pos = 0; pos < 64; pos++) {
		if (board(pos)) continue;
		isvalid = 0;
		memset(revset, 0, sizeof(revset));
		// check all 8 direction if chesses can be reversed
		for (j = 0; j < 8; j++) {
			dirx = DIRSET[j][0]; diry = DIRSET[j][1];
			tposx = to2dx(pos); tposy = to2dy(pos);
			revnum = 0;
			while (true) {
				tposx += dirx; tposy += diry;
				if (tposx < 0 || tposx >= 8 || tposy < 0 || tposy >= 8) break;
				color = board(tposx, tposy);
				if (color == EMPTY) break;
				else if (color == -turn) {
					revnum++;
				}
				else if (color == turn) { 
					if (revnum) {
						revset[j] = revnum; 
						isvalid = 1; 
					}
					break; 
				}
			}
		}
		// add a new playrev to the revset
		if (isvalid) {
			playset.push_back(PlayRev());
			playrev = &playset[playset.size()-1];
			playrev->pos = pos;
			for (j = 0; j < 8; j++) {
				playrev->revset[j] = revset[j];
			}
		}
	}
}

