#include "SymbolTable.h"
using namespace std;

class ProcTable : public SymbolTable
{
public:
	void insertProc(string);
	string getProcName(int);
	int getProcIndex(string);
	int getSize(); 
	bool containsProc(string);
	set<string> getAllProc();
};


