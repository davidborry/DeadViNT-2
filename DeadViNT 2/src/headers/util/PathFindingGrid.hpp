#ifndef PATHFINDINGGRID_HPP
#define PATHFINDINGGRID_HPP

#include <vector>

class Node{

public:
	bool isSolid() const;
	void setSolid(bool solid);

	void addNeighbour(Node* neighbour);
	void print();

private:
	std::vector<Node*> mNeighbours;
	bool mSolid = false;
};


class PathFindingGrid{

public:
	PathFindingGrid(int lines, int cols);

	void addNeighbours();
	bool isSolid(int x, int y) const;
	void setSolid(int x, int y, bool solid);
	void print();

private:
	int mLines;
	int mCols;

	Node** mNodes;
};

#endif