#include "SymbolTable.h"
using namespace std;

class ProcTable : public SymbolTable
{
public:
	void InsertProc(string);
	string GetProcName(int);
	int GetProcIndex(string);
	int GetSize(); 
	bool ContainsProc(string);
	set<string> GetAllProc();
};


