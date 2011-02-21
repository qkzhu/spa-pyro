#include "Node.h"
#include "AST.h"
#include "VarTable.h"
#include "ProcTable.h"

//#include "ModifyTable.h"
//#include "UseTable.h"
#include "ConstantTable.h"

class PKB
{
public:
	/**
	 * AST Part
	 */
	Node*		node_CreateNode(Node::NodeType nt, int statNum, int name);
	list<int>	node_GetChild(int stmtNum);
	Node*		node_GetPreviousStatement(Node* d);
	Node*		node_GetFollowingStatement(Node* d);
	Node*		node_GetNodeByStatementNum(int index);
	void ast_AddChild(Node *d, Node *childNode);
	void ast_AddDown(Node *upperNode, Node *bottomNode);
	void ast_AddFollow(Node *d, Node *followNode);
	void ast_AddProcedure(Node *d);
	AST* ast_CreateASTnode();
	/**
	 * VarTable Part
	 */
	void		vTable_InsertVar(string);
	string		vTable_GetVarName(int);
	int			vTable_GetVarIndex(string);
	int			vTable_GetVarTableSize(); 
	bool		vTable_ContainsVar(string);
	set<string> vTable_GetAllVar();
	/**
	 * ProcTable Part
	 */
	void		pTable_InsertProc(string);
	string		pTable_GetProcName(int);
	int			pTable_GetProcIndex(string);
	int			pTable_GetProcTableSize();
	bool		pTable_ContainsProc(string);
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
	vector<AST*>	astList;
	VarTable*		varTable;
	ProcTable*		procTable;
	/*
	ModifyTable*	modifyTable;
	UseTable*		useTable;
	ConstantTable*	constantTable;
	*/
};