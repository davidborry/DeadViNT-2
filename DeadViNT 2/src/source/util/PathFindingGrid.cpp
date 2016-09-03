#include "../../headers/util/PathFindingGrid.hpp"


bool Node::isSolid() const{
	return mSolid;
}

void Node::setSolid(bool solid){
	mSolid = solid;
}

void Node::addNeighbour(Node* neighbour){
	printf("%i", neighbour->isSolid());
	mNeighbours.push_back(neighbour);
}

void Node::print(){
	if (isSolid())
		printf("X ");
	else
		printf("_ ");

	//printf("%i ", mNeighbours.size());
}

PathFindingGrid::PathFindingGrid(int lines, int cols):
mLines(lines),
mCols(cols)
{
	mNodes = new Node*[mLines];

	for (int i = 0; i < mLines; i++)
		mNodes[i] = new Node[mCols];

	addNeighbours();
}

void PathFindingGrid::setSolid(int x, int y, bool solid){
	printf("%i,%i\n", mLines, mCols);
	mNodes[y][x].setSolid(solid);
}

void PathFindingGrid::print(){
	for (int i = 0; i < mLines; i++){
		for (int j = 0; j < mCols; j++)
			mNodes[i][j].print();
		printf("\n");
	}
	
}

void PathFindingGrid::addNeighbours(){
	for (int i = 0; i < mLines; i++)
		for (int j = 0; j < mCols; j++){

			if (i-1 >= 0)
				mNodes[i][j].addNeighbour(&mNodes[i - 1][j]);

			if (i+1 < mLines)
				mNodes[i][j].addNeighbour(&mNodes[i + 1][j]);


			if (j - 1 >= 0){
				mNodes[i][j].addNeighbour(&mNodes[i][j - 1]);

				if (i - 1 >=0)
					mNodes[i][j].addNeighbour(&mNodes[i - 1][j - 1]);

				if (i+1 < mLines)
					mNodes[i][j].addNeighbour(&mNodes[i + 1][j - 1]);
			}
			
			if (j + 1 < mCols){
				mNodes[i][j].addNeighbour(&mNodes[i][j + 1]);

				if (i - 1 >= 0)
					mNodes[i][j].addNeighbour(&mNodes[i - 1][j + 1]);

				if (i + 1 < mLines)
					mNodes[i][j].addNeighbour(&mNodes[i + 1][j + 1]);
			}
			
		}
}