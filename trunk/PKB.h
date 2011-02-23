#include "AST.h"
#include "SymbolTable.h"
#include "ConstantTable.h"
//#include "ModifyTable.h"
//#include "UseTable.h"


class PKB
{
public:

	///////////////////////////////////////REQUIRED BY HQ///////////////////////////////////////////
	//Newly added, not discussed yet
	//Didn't see QK'n API. In case of change, discuss together.
	bool hasCons(int);  //whether the simple code has this constant 
	bool hasStmt(int);
	bool hasProc(int);
	bool hasVar(int);


	std::vector<int> *getAllStmts();
	std::vector<int> *getAllAssign(); //get all assignment stmts
	std::vector<int> *getAllWhile();
	std::vector<int> *getAllIf();
	std::vector<int> *getAllCall();
	std::vector<int> *getAllProc();

	int getVarIndex(std::string var); //return the code for a variable                //Need changes
	int getProcIndex(std::string proc); //return the code for a procedure
	std::string getVarName(int key);	//return the variable name given its code
	std::string getProcName(int key); //return the procedure name given its code

	std::vector<int> *getCalls(int proc_index); 
	std::vector<int> *getCalled(int proc_index); 

	std::vector<int> *getStmtModifies(int varIndex);  //take a variable code and return all stmts that modify it 
	std::vector<int> *getProcModifies(int varIndex);  //return all procs that modify it
	std::vector<int> *getModifiedVar(int stmtNum); //take a stmt# and return the varName code that it modifies
	std::vector<int> *getModifiedVarPI(int procIndex); //No need to return vector, but for convenience, use vector

	std::vector<int> *getUsedVar(int stmtNum);  //return all varName code that this stmt uses
	std::vector<int> *getUsedVarPI(int procIndex); //return all varName code that this proc uses
	std::vector<int> *getStmtUses(int varIndex); //return all stmt# that uses this var in its code
	std::vector<int> *getProcUses(int varIndex); //return all procName in code that uses this var in its code

	int getFollowingStatement(int d);  //Get the follow node of given node d, with follow relation
	int getPreviousStatement(int d); //Get the previous node of given node d, with the follow relation


	int getParent(int child);
	std::vector<int> *getChild(int parent);  


	///////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 * PKB Part
	 */
	//init vTable, cTable, mTable, uTable, pTable
	PKB(): varTable(new VarTable()),
		   procTable(new ProcTable()),
		   constantTable(new ConstantTable()){}
	/**
	 * AST Part
	 */
	AST*		ast_CreateASTnode();
	Node*		ast_CreateNode(AST* ast, Node::NodeType nt, int statNum, int name);
	vector<int>	ast_GetChild(AST* ast, int stmtNum);
	Node*		ast_GetPreviousStatement(AST* ast, Node* d);
	Node*		ast_GetFollowingStatement(AST* ast, Node* d);
	Node*		ast_GetNodeByStatementNum(AST* ast, int index);
	void		ast_AddChild(AST* ast, Node *d, Node *childNode);
	void		ast_AddDown(AST* ast, Node *upperNode, Node *bottomNode);
	void		ast_AddFollow(AST* ast, Node *d, Node *followNode);
	void		ast_AddProcedure(AST* ast, int procIndex, Node *d);
	/**
	 * VarTable Part
	 */
	void		vTable_InsertVar(string var);
	string		vTable_GetVarName(int index);
	int			vTable_GetVarIndex(string var);
	int			vTable_GetVarTableSize(); 
	bool		vTable_ContainsVar(string var);
	set<string> vTable_GetAllVar();
	/**
	 * ProcTable Part
	 */
	void		pTable_InsertProc(string procName);
	string		pTable_GetProcName(int index);
	int			pTable_GetProcIndex(string procName);
	int			pTable_GetProcTableSize();
	bool		pTable_ContainsProc(string procName);
	set<string> pTable_GetAllProc();
	void		pTable_AddCall(int procIndex1, int procIndex2);
	/**
	 * ConstantTable Part
	 */
	void		cTable_AddConstant(int c);
	set<int>	cTable_GetAllConstants();
	bool		cTable_IsConstantExist(int c);
	int			cTable_GetNumOfConstant();

private:
	Node*			node;
	VarTable*		varTable;
	ProcTable*		procTable;
	ConstantTable*	constantTable;
	/*
	ModifyTable*	modifyTable;
	UseTable*		useTable;
	*/
};