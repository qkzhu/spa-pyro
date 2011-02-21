#include <set>

using namespace std;

class ConstantTable
{
public:
	void addConstant(int c);
	set<int> getAllConstants();
	bool isConstantExist(int c);
	int getNumOfConstant();
private:
	set<int> cSet;
};