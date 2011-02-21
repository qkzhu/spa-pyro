#include "SymbolTable.h"
using namespace std;

class VarTable : public SymbolTable
{
public:
	void InsertVar(string);
	string GetVarName(int);
	int GetVarIndex(string);
	int GetSize(); 
	bool ContainsVar(string);
	set<string> GetAllVar();
};


