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
	bool argumentNoCorrect;
	vector <int> attrCond; 
};

struct Declaration{
	vector<int> declaration;
};