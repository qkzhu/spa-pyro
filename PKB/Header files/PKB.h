#ifndef PKB_H
#define PKB_H

#include "AST.h"
#include "SymbolTable.h"
#include "ConstantTable.h"
#include "Modifies.h"
#include "Uses.h"


class PKB
{
public:
	/**
	 * PKB Part
	 */
	//init vTable, cTable, mTable, uTable, pTable
	PKB(): varTable(new VarTable()),
		   procTable(new ProcTable()),
		   constantTable(new ConstantTable()),
		   useTable(new Uses()), modifyTable(new Modifies()), 
		   ast(new AST()){}
	/**
	 * AST Part
	 */
	Node*		ast_CreateNode(Node::NodeType nt, int statNum, int name);
	vector<int> ast_GetChild(int stmtNum);
	Node*		ast_GetPreviousStatement(Node* d);
	int			ast_GetPreviousStatementNum(int stmtNum);
	Node*		ast_GetFollowingStatement(Node* d);
	int			ast_GetFollowingStatementNum(int stmtNum);
	Node*		ast_GetNodeByStatementNum(int index);
	void		ast_AddChild(Node *d, Node *childNode);
	void		ast_AddDown(Node *upperNode, Node *bottomNode);
	void		ast_AddFollow(Node *d, Node *followNode);
	void		ast_AddProcedure(int procIndex, Node *d);
	int			ast_getParent(int child);
	bool		ast_hasStmt(int stmtNum);
	int			ast_getMaxStmtNum();
	vector<int> ast_GetAllAssign();
	vector<int> ast_GetAllWhile();
	vector<int> ast_GetAllIf();
//	vector<int> ast_GetAllProc(); Similar function can be found at pTable_GetAllProc()
	vector<int>	ast_GetAllCall();
	/**
	 * VarTable Part
	 */
	void			vTable_InsertVar(string var);
	string			vTable_GetVarName(int index);
	int				vTable_GetVarIndex(string var);
	int				vTable_GetVarTableSize(); 
	bool			vTable_IsVarNameExist(string var);
	bool			vTable_IsVarIndexExist(int varIndex);
	vector<int>		vTable_GetAllVar();
	/**
	 * ProcTable Part
	 */
	void			pTable_InsertProc(string procName);
	string			pTable_GetProcName(int procIndex);
	int				pTable_GetProcIndex(string procName);
	int				pTable_GetProcTableSize();
	bool			pTable_isProcNameExist(string procName);
	bool			pTable_isProcIndexExist(int procIndex);
	vector<int>		pTable_GetAllProc();
	void			pTable_AddCall(int procIndex1, int procIndex2);
	vector<int> 	pTable_getCall(int procIndex);
	vector<int> 	pTable_getCalled(int procIndex);
	/**
	 * ConstantTable Part
	 */
	void		cTable_AddConstant(int c);
	vector<int>	cTable_GetAllConstants();
	bool		cTable_IsConstantExist(int c);
	int			cTable_GetNumOfConstant();
	bool		cTable_hasCons(int c);
	/**
	 * UseTable Part
	 */
	void			uTable_setUses(int stmtNum, int varIndex);
	void			uTable_setUsesPV(int procIndex, int varIndex);
	vector<int> 	uTable_getUsedVar(int stmtNum);
	vector<int> 	uTable_getUsedVarPI(int procIndex);
	vector<int> 	uTable_getStmtUses(int varIndex);
	vector<int> 	uTable_getProcUses(int varIndex);
	/**
	 * ModifyTable Part
	 */
	vector<int>  mTable_getModifiedVar(int statNum);
	vector<int>  mTable_getModifiedVarPI(int procIndex);
	vector<int>  mTable_getStmtModifies(int varIndex);
	vector<int>  mTable_getProcModifies(int procIndex);
	void		 mTable_setModify(int stmtNum, int varIndex);
	void		 mTable_setModifyPV(int procIndex, int varIndex);

	/*
		Debugging functions
	*/
	void printTree();

private:
	AST*			ast;
	Node*			node;
	VarTable*		varTable;
	ProcTable*		procTable;
	ConstantTable*	constantTable;
	Uses*			useTable;
	Modifies*		modifyTable;
	vector<int>		convertSetToVector(set<int>);
	vector<string>	convertSetToVector(set<string>);
};

#endif