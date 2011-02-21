#include "SymbolTable.h"
using namespace std;

class VarTable : public SymbolTable
{
public:
	void insertVar(string);
	string getVarName(int);
	int getVarIndex(string);
	int getSize(); 
	bool containsVar(string);
	set<string> getAllVar();
};


