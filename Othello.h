#pragma once

#include<iostream>
#include<vector>
#include<set>
#include<time.h>

using namespace std;

class PlayRev {
public:
	int pos;
	int revset[8] = {};
	PlayRev();
};

class Othello {
public:
	static const int BLACK;
	static const int WHITE;
	static const int EMPTY;
	static const int DRAW;
	static const int PLAYING;
	static const int DIRSET[8][2];
	int turn;
	Othello();
	int _board[64] = {};
	int showboard();
	inline int &board(int pos) {
		return _board[pos];
	}
	inline int &board(int x, int y) {
		return _board[to1d(x, y)];
	}

	static inline int to1d(int x, int y) {
		return x * 8 + y;
	}

	static inline int to2dx(int pos) {
		return pos >> 3;
	}

	static inline int to2dy(int pos) {
		return pos % 8;
	}

	void updateplayset();

	int play(int pos);

	int play(PlayRev playrev);

	int randomplay();

	vector<PlayRev> playset;

	int boardstate;

};