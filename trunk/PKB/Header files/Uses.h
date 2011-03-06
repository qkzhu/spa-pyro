#ifndef USES_H
#define USES_H

#include <set>
#include <map>
#include "SymbolTable.h"	//For debugging
using namespace std;

class Uses
{
public:
	set<int> getUsedVar(int);					// takes in stmtNum; return varIndex
	set<int> getUsedVarPI(int);					// takes in procIndex; return varIndex
	set<int> getStmtUses(int);					// takes in varIndex; return stmt#
	set<int> getProcUses(int);					// takes in varIndex; return procIndex<int>
	void setUses(int stmtNum, int varIndex);	// stmtNum, varIndex
	void setUsesPV(int procIndex, int varIndex);// procIndex, varIndex

	//For Debugging
	void printStmtUseMap(VarTable* varTable);						// map each stmt# to a set of used varIndex
	void printProcUseMap(ProcTable* pTable, VarTable* varTable);	// map each ProcIndex to a set of used varIndex
	void printVarStmtMap(VarTable* varTable);						// map each used varIndex to a set of stmt#
	void printVarProcMap(ProcTable* pTable, VarTable* varTable);	// map each used varIndex to a set of procIndex
private:
	map<int, set<int>> mStmtUseMap; // map each stmt# to a set of used varIndex
	map<int, set<int>> mProcUseMap; // map each ProcIndex to a set of used varIndex
	map<int, set<int>> mVarStmtMap; // map each used varIndex to a set of stmt#
	map<int, set<int>> mVarProcMap; // map each used varIndex to a set of procIndex
};

#endif