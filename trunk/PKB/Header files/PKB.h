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
	 * AST Part:9
	 */
	Node*		ast_CreateNode(Node::NodeType nt, int statNum, int name);
	vector<int> ast_GetChild(int stmtNum);   // For Query Evaluator
	Node*		ast_GetPreviousStatement(Node* d);
	int			ast_GetPreviousStatementNum(int stmtNum);   // For Query Evaluator
	Node*		ast_GetFollowingStatement(Node* d);
	int			ast_GetFollowingStatementNum(int stmtNum);   // For Query Evaluator
	Node*		ast_GetNodeByStatementNum(int index);
	void		ast_AddChild(Node *d, Node *childNode);   
	void		ast_AddDown(Node *upperNode, Node *bottomNode);
	void		ast_AddFollow(Node *d, Node *followNode);
	void		ast_AddProcedure(int procIndex, Node *d);
	int			ast_getParent(int child);    // For Query Evaluator
	Node*		ast_getParent(Node* n);
	int			ast_getStmtNum(Node* n);
	bool		ast_hasStmt(int stmtNum);   
	int			ast_getMaxStmtNum();  // For Query Evaluator 
	vector<int> ast_GetAllAssign();   // For Query Evaluator
	vector<int> ast_GetAllWhile();	// For Query Evaluator
	vector<int> ast_GetAllIf();	// For Query Evaluator
	vector<int>	ast_GetAllCall();  // For Query Evaluator
	/**
	 * VarTable Part:5
	 */
	void			vTable_InsertVar(string var);
	string			vTable_GetVarName(int index);   // For Query Evaluator
	int				vTable_GetVarIndex(string var);  // For Query Evaluator
	int				vTable_GetVarTableSize(); 
	bool			vTable_IsVarNameExist(string var);  // For Query Evaluator
	bool			vTable_IsVarIndexExist(int varIndex);  // For Query Evaluator
	vector<int>		vTable_GetAllVar();   // For Query Evaluator
	/**
	 * ProcTable Part:7
	 */
	void			pTable_InsertProc(string procName);
	string			pTable_GetProcName(int procIndex);   // For Query Evaluator
	int				pTable_GetProcIndex(string procName);  // For Query Evaluator
	int				pTable_GetProcTableSize();
	bool			pTable_isProcNameExist(string procName); // For Query Evaluator
	bool			pTable_isProcIndexExist(int procIndex);  // For Query Evaluator
	vector<int>		pTable_GetAllProc();				 // For Query Evaluator
	void			pTable_AddCall(int procIndex1, int procIndex2);
	vector<int> 	pTable_getCall(int procIndex);			// For Query Evaluator
	vector<int> 	pTable_getCalled(int procIndex);		// For Query Evaluator
	vector<int>		pTable_getCall_(int procIndex);
	vector<int>		pTable_getCalled_(int procIndex);
	/**
	 * ConstantTable Part:3
	 */
	void		cTable_AddConstant(int c);
	vector<int>	cTable_GetAllConstants();			// For Query Evaluator
	bool		cTable_IsConstantExist(int c);      // For Query Evaluator
	int			cTable_GetNumOfConstant();
	bool		cTable_hasCons(int c);			// For Query Evaluator
	/**
	 * UseTable Part: 4
	 */
	void			uTable_setUses(int stmtNum, int varIndex);
	void			uTable_setUsesPV(int procIndex, int varIndex);
	vector<int> 	uTable_getUsedVar(int stmtNum);         // For Query Evaluator
	vector<int> 	uTable_getUsedVarPI(int procIndex);     // For Query Evaluator
	vector<int> 	uTable_getStmtUses(int varIndex);       // For Query Evaluator 
	vector<int> 	uTable_getProcUses(int varIndex);       // For Query Evaluator  
	/**
	 * ModifyTable Part:4
	 */
	vector<int>  mTable_getModifiedVar(int statNum);        // For Query Evaluator
	vector<int>  mTable_getModifiedVarPI(int procIndex);    // For Query Evaluator
	vector<int>  mTable_getStmtModifies(int varIndex);      // For Query Evaluator
	vector<int>  mTable_getProcModifies(int procIndex);     // For Query Evaluator 
	void		 mTable_setModify(int stmtNum, int varIndex);
	void		 mTable_setModifyPV(int procIndex, int varIndex);

	/*
		Debugging functions
	*/
	void printTree();
	AST* getAST();
	VarTable* getVarTable();
	ProcTable* getProcTable();
	ConstantTable* getConstantTable();
	Uses* getUseTable();
	Modifies* getModifyTable();
	void printVectorInt(vector<int>);

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