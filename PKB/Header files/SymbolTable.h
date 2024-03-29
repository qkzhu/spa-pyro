#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <hash_map>
#include <string>
#include <utility>
#include <set>
#include <vector>
#include <queue>
#include <iostream>	//For Debugging

using namespace std;


//Delclarations
typedef hash_map<string,int> MapType; //for implementation use

class SymbolTable
{
public:
	void insertSymbol(string); // takes symbol name; assign symbol name to an index
	string getSymbolName(int); // takes in symbol index; return symbol name
	int getSymbolIndex(string);// takes in symbol name; return symbol index
	int getSymbolMapSize(); // return total number of elements in symbol map
	bool isNameExist(string name); // takes in symbol name; return bool isNameExist
	bool isIndexExist(int index); // takes in symbol index; return bool isIndexExist
	int getAllSymbol();  // return the largest index of symbol (varIndex/ procIndex)

	//For Debugging
	void printSymbolTable(std::string name);
private:
	hash_map<string, int> mSymbolMap;
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
	int getAllVar(); //return the largest varIndex

	//For Debugging
	void printVarTable();
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
	int getAllProc(); //return the largest procIndex

	/* For call Tables */
	void addCall(int procIndex1, int procIndex2);
	void addCall(int procIndex1, vector<int> procList);
	void addCalled(int procIndex1, int procIndex2);
	void addCalled(int procIndex1, vector<int> procList);
	void getCall(int procIndex, vector<int>& result);
	void getCall_(int procIndex, vector<int>& result);
	void getCalled(int procIndex, vector<int>& result);
	void getCalled_(int procIndex, vector<int>& result);

	//For Debugging
	void printProcTable();
	void printCallTable();		/* int calls set<int> */
	void printCalledTable();	/* int was called by set<int> */
	void printCall_Table();
	void printCalled_Table();
private:
	hash_map<int, set<int>> callTable;		/* int calls set<int> */
	hash_map<int, set<int>> calledTable;		/* int was called by set<int> */

	//these two will only be computed on demand.
	hash_map<int, vector<int>> call_Table;
	hash_map<int, vector<int>> called_Table;
};

#endif