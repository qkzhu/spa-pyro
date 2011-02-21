#include "AST.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "ConstantTable.h"
//#include "ModifyTable.h"
//#include "UseTable.h"


class PKB
{
public:
	/**
	 * PKB Part
	 */
	//init vTable, cTable, mTable, uTable, pTable
	PKB(): varTable(new VarTable()),
		   procTable(new ProcTable()),
		   constantTable(new ConstantTable()){}
	//PKB():varTable(new VarTable()){}
	void pkb_AddToASTList(AST* ast, int procIndex);
	AST* getASTByProcdureIndex(int procIndex);
	/**
	 * AST Part
	 */
	AST*		ast_CreateASTnode();
	Node*		ast_CreateNode(AST* ast, Node::NodeType nt, int statNum, int name);
	list<int>	ast_GetChild(AST* ast, int stmtNum);
	Node*		ast_GetPreviousStatement(AST* ast, Node* d);
	Node*		ast_GetFollowingStatement(AST* ast, Node* d);
	Node*		ast_GetNodeByStatementNum(AST* ast, int index);
	void		ast_AddChild(AST* ast, Node *d, Node *childNode);
	void		ast_AddDown(AST* ast, Node *upperNode, Node *bottomNode);
	void		ast_AddFollow(AST* ast, Node *d, Node *followNode);
	void		ast_AddProcedure(AST* ast, Node *d);
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
	/**
	 * ConstantTable Part
	 */
	void		cTable_AddConstant(int c);
	set<int>	cTable_GetAllConstants();
	bool		cTable_IsConstantExist(int c);
	int			cTable_GetNumOfConstant();

private:
	Node*			node;
	map<int, AST*>	astList;
	VarTable*		varTable;
	ProcTable*		procTable;
	ConstantTable*	constantTable;
	/*
	ModifyTable*	modifyTable;
	UseTable*		useTable;
	*/
};