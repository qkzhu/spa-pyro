#include <iostream>
#include <vector>

class Driver{
public:
	Driver();
	int getFollowingStatement(int);
	int getPreviousStatement(int);

	int getProgSize();

	std::vector<int> *getAllStatements();
};