#ifndef PKB_H
#define PKB_H

#include "AST.h"
#include "SymbolTable.h"
#include "ConstantTable.h"
#include "Modifies.h"
#include "Uses.h"
#include "Pattern.h"
#include "CFG.h"
#include <sstream>


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
		   ast(new AST()), cfg(new CFG()){}
	/**
	 * AST Part:9
	 */
	Node*		ast_CreateNode(Node::NodeType nt, int statNum, int name);
	void		ast_GetChild(vector<int>&, int stmtNum);	// For Query Evaluator
	Node*		ast_GetPreviousStatement(Node* d);
	int			ast_GetPreviousStatementNum(int stmtNum);   // For Query Evaluator
	Node*		ast_GetFollowingStatement(Node* d);
	int			ast_GetFollowingStatementNum(int stmtNum);   // For Query Evaluator
	Node*		ast_GetNodeByStatementNum(int index);
	void		ast_AddChild(Node *d, Node *childNode);   
	void		ast_AddDown(Node *upperNode, Node *bottomNode);
	void		ast_AddFollow(Node *d, Node *followNode);
	void		ast_AddProcedure(int procIndex, Node *d);
	int			ast_getParent(int child);			// For Query Evaluator
	Node*		ast_getParent(Node* n);
	int			ast_getStmtNum(Node* n);
	bool		ast_hasStmt(int stmtNum);   
	int			ast_getMaxStmtNum();				// For Query Evaluator 
	void		ast_GetAllAssign(vector<int>&);		// For Query Evaluator
	void		ast_GetAllWhile(vector<int>&);		// For Query Evaluator
	void		ast_GetAllIf(vector<int>&);			// For Query Evaluator
	void		ast_GetAllCall(vector<int>&);		// For Query Evaluator
	bool		ast_IsIf(int stmt);					// For Query Evaluator
	bool		ast_IsWhile(int stmt);				// For Query Evaluator
	bool		ast_IsAssign(int stmt);				// For Query Evaluator
	bool		ast_IsCall(int stmt);				// For Query Evaluator
	
	vector<Node*> ast_GetAllProc(); //For CFG
	vector<Node*> ast_GetDown(Node* n); //For CFG
	/**
	 * VarTable Part:5
	 */
	void			vTable_InsertVar(string var);
	string			vTable_GetVarName(int index);			// For Query Evaluator
	int				vTable_GetVarIndex(string var);			// For Query Evaluator
	int				vTable_GetVarTableSize();
	bool			vTable_IsVarNameExist(string var);		// For Query Evaluator
	bool			vTable_IsVarIndexExist(int varIndex);	// For Query Evaluator
	int				vTable_GetAllVar();						// For Query Evaluator
	/**
	 * ProcTable Part:7
	 */
	void			pTable_InsertProc(string procName);
	string			pTable_GetProcName(int procIndex);   // For Query Evaluator
	int				pTable_GetProcIndex(string procName);  // For Query Evaluator
	int				pTable_GetProcTableSize();
	bool			pTable_isProcNameExist(string procName); // For Query Evaluator
	bool			pTable_isProcIndexExist(int procIndex);  // For Query Evaluator
	int				pTable_GetAllProc();				 // For Query Evaluator
	void			pTable_AddCall(int procIndex1, int procIndex2);
	void			pTable_getCall(vector<int>&, int procIndex);			// For Query Evaluator
	void		 	pTable_getCalled(vector<int>&, int procIndex);		// For Query Evaluator
	void			pTable_getCall_(vector<int>&, int procIndex);
	void			pTable_getCalled_(vector<int>&, int procIndex);
	/**
	 * ConstantTable Part:3
	 */
	void		cTable_AddConstant(int c);
	void		cTable_GetAllConstants(vector<int>&);			// For Query Evaluator
	bool		cTable_IsConstantExist(int c);      // For Query Evaluator
	int			cTable_GetNumOfConstant();
	bool		cTable_hasCons(int c);			// For Query Evaluator
	/**
	 * UseTable Part: 4
	 */
	void			uTable_setUses(int stmtNum, int varIndex);
	void			uTable_setUsesPV(int procIndex, int varIndex);
	void		 	uTable_getUsedVar(vector<int>&, int stmtNum);         // For Query Evaluator
	void			uTable_getUsedVarPI(vector<int>&, int procIndex);     // For Query Evaluator
	void			uTable_getStmtUses(vector<int>&, int varIndex);       // For Query Evaluator 
	void			uTable_getProcUses(vector<int>&, int varIndex);       // For Query Evaluator  
	/**
	 * ModifyTable Part:4
	 */
	void		mTable_getModifiedVar(vector<int>&, int statNum);        // For Query Evaluator
	void		mTable_getModifiedVarPI(vector<int>&, int procIndex);    // For Query Evaluator
	void		mTable_getStmtModifies(vector<int>&, int varIndex);      // For Query Evaluator
	void		mTable_getProcModifies(vector<int>&, int procIndex);     // For Query Evaluator 
	void		mTable_setModify(int stmtNum, int varIndex);
	void		mTable_setModifyPV(int procIndex, int varIndex);

	/**
	 * Pattern
	 */
	bool patternAssign(int stmtNum, string patternLeft, string patternRight); //For Query Evaluator
	int condWhile(int stmtNum);
	int condIf(int stmtNum);


	/**
	 * CFG
	 */
	void cfg_getNext(vector<int>& theNext, int stmt);		//(for different nodes, insert a -1 inside.)
	void cfg_getNextUp(vector<int>& theNextUp, int stmt);   //(for different nodes, insert a -1 inside.)

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
	string intToString(int i);
	void printCFG();

private:
	AST*			ast;
	Node*			node;
	VarTable*		varTable;
	ProcTable*		procTable;
	ConstantTable*	constantTable;
	Uses*			useTable;
	Modifies*		modifyTable;
	CFG*			cfg;

	Pattern pattern;
	void convertSetToVector(set<int>&, vector<int>&);
	void convertSetToVector(set<string>&, vector<string>&);
};

#endif