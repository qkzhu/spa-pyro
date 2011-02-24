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
	void insertSymbol(string); // takes symbol name; assign symbol name to an index
	string getSymbolName(int); // takes in symbol index; return symbol name
	int getSymbolIndex(string);// takes in symbol name; return symbol index
	int getSymbolMapSize(); // return total number of elements in symbol map
	bool isNameExist(string name); // takes in symbol name; return bool isNameExist
	bool isIndexExist(int index); // takes in symbol index; return bool isIndexExist
	set<string> getAllSymbol();
private:
	map<string, int> mSymbolMap;
};

class VarTable : public SymbolTable
{
public:
	void insertVar(string);  // assign varName to an index
	string getVarName(int);  // get varName using index
	int getVarIndex(string); // get varIndex usign varName
	int getSize(); // total no. of variables in VarTable
	bool isVarNameExist(string name);
	bool isVarIndexExist(int index);
	set<string> getAllVar();
};

class ProcTable : public SymbolTable
{
public:
	void insertProc(string);  // assign procName to an index
	string getProcName(int);  // get procName using index
	int getProcIndex(string); // get procIndex using procName
	int getSize();  // total no. of procedures in ProcTable
	bool isProcNameExist(string name);
	bool isProcIndexExist(int index);
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