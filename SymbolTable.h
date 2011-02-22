#include <map>
#include <string>
#include <utility>
#include <set>
#include <vector>

using namespace std;


//Delclarations
typedef map<string,int> MapType; //for implementation use

class SymbolTable
{
public:
	void insertSymbol(string);
	string getSymbolName(int);
	int getSymbolIndex(string);
	int getSymbolMapSize();
	bool containsSymbol(string);
	set<string> getAllSymbol();
private:
	map<string, int> mSymbolMap;
};

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

class ProcTable : public SymbolTable
{
public:
	void insertProc(string);
	string getProcName(int);
	int getProcIndex(string);
	int getSize(); 
	bool containsProc(string);
	set<string> getAllProc();

	/* For call Tables */
	void addCall(int procIndex1, int procIndex2);
	void addCall(int procIndex1, vector<int> procList);
	void addCalled(int procIndex1, int procIndex2);
	void addCalled(int procIndex1, vector<int> procList);
	vector<int> getCall(int procIndex);
	vector<int> getCalled(int procIndex);

private:
	map<int, set<int>> callTable;		/* int calls set<int> */
	map<int, set<int>> calledTable;		/* int was called by set<int> */
};