#include <set>
#include <map>
using namespace std;

class Modifies
{
public:
	set<int> getModifiedVar(int);	//takes in a statNum; return varIndex
	set<int> getModifiedVarPI(int); // takes in a procIndex; return varIndex
	set<int> getStmtModifies(int);	// takes in a varIndex
	set<int> getProcModifies(int);	// takes in a varIndex; return procIndex
	void setModify(int stmtNum, int varIndex);
	void setModifyPV(int procIndex, int varIndex);

private:
	map<int, set<int>> mStmtModMap; // map each stmt# to a set of modified varIndex
	map<int, set<int>> mProcModMap; // map each ProcIndex to a set of modified varIndex
	map<int, set<int>> mVarStmtMap; // map each modified varIndex to a set of stmt#
	map<int, set<int>> mVarProcMap; // map each modified varIndex to a set of procIndex
};