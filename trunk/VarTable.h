#include "SymbolTable.h"
using namespace std;

class VarTable : public SymbolTable
{
public:
	void insertVar(string var);
	string getVarName(int index);
	int getVarIndex(string var);
	int getSize(); 
	bool containsVar(string var);
	set<string> getAllVar();
};
