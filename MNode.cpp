#include"MNode.h"

MNode::MNode(Othello *othello){
	this->othello = othello;
	children = vector<MNode*>(othello->playset.size(), (MNode*)0);
	parent = 0;
	vn = an = 0;
	//fullexpanded = 0;
}

int MNode::TreePolicy() {
	const double r = 2;
	double ucbval = -2, tval;
	int ucbi = -1;
	for (int i = 0; i < children.size(); i++) {
		MNode *tnode = children[i];
		if (!tnode) return i;
		//if (tnode->fullexpanded) continue;
		if (!tnode->vn) return i;
		tval = (double)tnode->an / tnode->vn + r * sqrt(log((double)vn / tnode->vn));
		if (tval > ucbval) { ucbval = tval; ucbi = i; }
	}
	//if (ucbi == -1) fullexpanded = 1;
	return ucbi;
}

int MNode::SearchOnce() {
	MNode *tnode = 0;
	MNode *nnode = this;
	int n;
	while (nnode) {
		tnode = nnode;
		n = tnode->TreePolicy();
		if (n == -1) {
			/*if (tnode->parent) {
				nnode = tnode->parent;
			}else*/ return 1;
		}
		else {
			nnode = tnode->children[n];
		}
	}

	Othello *tothello = new Othello(*(tnode->othello));
	tothello->play(tothello->playset[n]);

	nnode = new MNode(tothello);
	nnode->parent = tnode;
	tnode->children[n] = nnode;

	int simuv = tnode->DefaultPolicy();
	if (simuv == othello->turn) {
		nnode->BackPropagation(1);
	} else if(simuv == -(othello->turn)) {
		nnode->BackPropagation(0);
	} else {
		nnode->BackPropagation(0.5);
	}
	return 0;
}

MNode* MNode::SearchAndPlay() {
	const double timelimit = 10;
	time_t s_time, t_time;
	time(&s_time);
	int i = 0;
	int searchend;
	while(1){
		searchend = SearchOnce();
		if (searchend) { std::cout << "Search End" << std::endl; break; }
		i++;
		time(&t_time);
		if (difftime(t_time, s_time) > timelimit) break;
	}
	int n = -1;
	double maxucb = -2;
	double tucb;
	for (int i = 0; i < children.size(); i++) {
		if (!children[i]) continue;
		tucb = (double)children[i]->an / children[i]->vn;
		if (tucb > maxucb){
			maxucb = tucb;
			n = i;
		}
	}
	if (n < 0) {
		std::cout << "Search Not Enough!" << std::endl;
		return this;
	}
	std::cout << "Play " << Othello::to2dx(othello->playset[n].pos) << " " << Othello::to2dy(othello->playset[n].pos) << " "
		<< "\tWith " << i << " searches" << std::endl;
//	showtree(0, 2);
	return Play(n);
}

int MNode::DefaultPolicy() {
	Othello tothello(*othello);
	while (tothello.boardstate == Othello::PLAYING) {
		tothello.randomplay();
	}
	return tothello.boardstate;
}

void MNode::BackPropagation(double val) {
	for (MNode *tnode = this; tnode; tnode = tnode->parent) {
		tnode->vn++; tnode->an += val;
	}
}

MNode* MNode::Play(int n) {
	MNode *nnode = children[n];
	if (!nnode) {
		Othello *tothello = new Othello(*othello);
		tothello->play(tothello->playset[n]);
		nnode = new MNode(tothello);
	}
	nnode->parent = 0;
	this->children[n] = 0;
	freetree();
	return nnode;
}

MNode* MNode::Play(int x, int y) {
	int n = -1;
	for (int i = 0; i < othello->playset.size(); i++) {
		if (Othello::to1d(x, y) == othello->playset[i].pos) { n = i; break; }
	}
	if (n >= 0) return Play(n);
	else {
		std::cout << "Invalid play" << std::endl;
		return this;
	}
}

MNode::~MNode() {
	delete othello;
}

void MNode::freetree() {
	for (int i = 0; i < children.size(); i++) {
		if(children[i]) children[i]->freetree();
	}
	delete this;
}


void MNode::showtree(int level) {
	for (int i = 0; i < level; i++) {
		std::cout << "  ";
	}
	std::cout << "|| " << an << " / " << vn << std::endl;
	for (int i = 0; i < children.size(); i++) {
		if(children[i]) children[i]->showtree(level + 1);
	}
}

void MNode::showtree(int level, int targetlevel) {
	
	for (int i = 0; i < level; i++) {
		std::cout << "  ";
	}
	std::cout << "|| " << an << " / " << vn << std::endl;
	if (level >= targetlevel) return;
	for (int i = 0; i < children.size(); i++) {
		for (int i = 0; i < level + 1; i++) {
			std::cout << "  ";
		}
		std::cout << "x= " <<  Othello::to2dx(othello->playset[i].pos)<< " y= " << Othello::to2dy(othello->playset[i].pos) << std::endl;
		if (children[i]) children[i]->showtree(level + 1, targetlevel);
	}
}