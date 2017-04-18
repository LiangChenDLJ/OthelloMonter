#pragma once
#include<iostream>
#include<math.h>
#include<time.h>

#include"Othello.h"

class MNode {
public:
	MNode(Othello *othello);
	Othello *othello;
	int vn;
	double an;
	vector<MNode*> children;
	MNode *parent;
	MNode* SearchAndPlay();
	MNode* Play(int n);
	MNode* Play(int x, int y);
	void BackPropagation(double val);
	void freetree();
	//int fullexpanded;
	~MNode();

	//FOT TEST
	void showtree(int level, int tatrgetlevel);
	void showtree(int level);

private:
	int TreePolicy();
	int DefaultPolicy();
	int SearchOnce();
};
