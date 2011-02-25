#include <vector>

using namespace std;

struct Select{
	vector<int> tuple;
};

struct SuchThat{
	bool argumentNoCorrect;
	vector<int> relCond;
};

struct With{
	int length;
	vector <int> attrCond; 
};

struct Declaration{
	vector<int> declaration;
};