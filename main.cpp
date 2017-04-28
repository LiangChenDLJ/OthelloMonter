#include<iostream>
#include<stdlib.h>
#include<time.h>

#include"Othello.h"
#include"MNode.h"


using namespace std;

int main() {
	char c;
	int a, b;
	Othello* othello = new Othello();
	othello->showboard();
	MNode *mtree=  new MNode(othello);
	const int COMPUTER = Othello::WHITE;
	srand(time(0));
	while (mtree->othello->boardstate == Othello::PLAYING) {
		if (mtree->othello->turn == COMPUTER) {
			mtree = mtree->SearchAndPlay();
		}
		else {
			/*
			srand(time(0));
			mtree = mtree->Play(rand() % mtree->othello->playset.size());
			*/
			std::cin >> a;
			if (a == -1) {
				srand(time(0));
				mtree = mtree->Play(rand() % mtree->othello->playset.size());
			}
			else {
				std::cin >> b;
				mtree = mtree->Play(a, b);
			}
		}
		mtree->othello->showboard();
	}
	if (mtree->othello->boardstate == Othello::WHITE) {
		std::cout << "WINNER IS WHITE!" << std::endl;
	}
	else if (mtree->othello->boardstate == Othello::BLACK) {
		std::cout << "WINNER IS BLACK!" << std::endl;
	}else {
		std::cout << "DRAW!" << std::endl;
	}
	std::cin >> c;
	/*
	while (1) {
		cin >> c;
		if (c == 'p') {
			othello->randomplay(); othello->showboard();
			othello = new Othello(*othello);
			mtree->freetree();
			mtree = new MNode(othello);
		}
		else if (c == 's'){
			mtree->SearchOnce();
			mtree->showtree(0);
		}
	}
	*/
}
