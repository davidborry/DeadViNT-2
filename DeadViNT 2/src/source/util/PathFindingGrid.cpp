#include "../../headers/util/PathFindingGrid.hpp"

namespace std
{
	//namespace tr1
	//{
	// Specializations for unordered containers

	template <>
	struct hash<Node> : public unary_function<Node, size_t>
	{
		size_t operator()(const Node& value) const
		{
			return 0;
		}
	};

	//} // namespace tr1

	template <>
	struct equal_to<Node> : public unary_function<Node, bool>
	{
		bool operator()(const Node& x, const Node& y) const
		{
			return false;
		}
	};

}


Node::Node(int x, int y):
x(x),
y(y)
{
}

Node::Node():
x(0),
y(0)
{}

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

int Node::getX() const{
	return x;
}

int Node::getY() const{
	return y;
}

void Node::print(){
	if (isSolid())
		printf("X ");
	else
		printf("_ ");

//	printf("%i ", mNeighbours.size());

//	printf("%i,%i ", x, y);
}

std::vector<Node*> Node::getNeighbours(){
	return mNeighbours;
}

PathFindingGrid::PathFindingGrid(int lines, int cols):
mLines(lines),
mCols(cols)
{
	mNodes = new Node*[mLines];

	for (int i = 0; i < mLines; i++){
		mNodes[i] = new Node[mCols];
		for (int j = 0; j < mCols; j++)
			mNodes[i][j] = Node(i, j);
	}

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

Node* PathFindingGrid::getNode(int x, int y){
	return &mNodes[x][y];
}

std::vector<PathFindingGrid::Position> PathFindingGrid::getPath(PathFindingGrid::Position start, PathFindingGrid::Position end){
	std::vector<PathFindingGrid::Position> path;
	std::unordered_map<Node*, Node*> cameFrom;

	std::queue<Node*> frontier;
	frontier.push(&mNodes[start.y][start.x]);

	Node* startNode = &mNodes[start.y][start.x];
	cameFrom[startNode] = startNode;

	while (!frontier.empty()){
		Node* current = frontier.front();
		frontier.pop();


		if (current->getX() == end.y && current->getY() == end.x)
			break;
		
		for (Node* next : current->getNeighbours()){
			if (!cameFrom.count(next) && !next->isSolid()){
				frontier.push(next);
				cameFrom[next] = current;
			}
		}
	}


	Node* current = &mNodes[end.y][end.x];

	if (current != NULL)
	path.push_back({ current->getY(), current->getX() });
	while (current != startNode) {
		current = cameFrom[current];
		if (current == NULL)
			break;
		path.push_back({ current->getY(), current->getX() });
	}

	path.push_back(start); // optional
	std::reverse(path.begin(), path.end());
	return path;
	
	
	
}