#include "SymbolTable.h"
using namespace std;

class ProcTable : public SymbolTable
{
public:
	void insertProc(string procName);
	string getProcName(int index);
	int getProcIndex(string procName);
	int getSize(); 
	bool containsProc(string procName);
	set<string> getAllProc();
};


