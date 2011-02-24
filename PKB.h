#include "AST.h"
#include "SymbolTable.h"
#include "ConstantTable.h"
#include "Modifies.h"
#include "Uses.h"


class PKB
{
public:

	///////////////////////////////////////REQUIRED BY HQ///////////////////////////////////////////
	//Newly added, not discussed yet
	//Didn't see QK'n API. In case of change, discuss together.
	//bool hasCons(int c);  //whether the simple code has this constant 
	bool hasStmt(int stmtNum);

	vector<int> getAllStmts();
	vector<int> getAllAssign(); //get all assignment stmts
	vector<int> getAllWhile();
	vector<int> getAllIf();
	vector<int> getAllCall();
	vector<int> getAllProc();
	///////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 * PKB Part
	 */
	//init vTable, cTable, mTable, uTable, pTable
	PKB(): varTable(new VarTable()),
		   procTable(new ProcTable()),
		   constantTable(new ConstantTable()),
		   useTable(new Uses()),
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
	/**
	 * VarTable Part
	 */
	void			vTable_InsertVar(string var);
	string			vTable_GetVarName(int index);
	int				vTable_GetVarIndex(string var);
	int				vTable_GetVarTableSize(); 
	bool			vTable_IsVarNameExist(string var);
	bool			vTable_IsVarIndexExist(int varIndex);
	set<string>		vTable_GetAllVar();
	/**
	 * ProcTable Part
	 */
	void			pTable_InsertProc(string procName);
	string			pTable_GetProcName(int procIndex);
	int				pTable_GetProcIndex(string procName);
	int				pTable_GetProcTableSize();
	bool			pTable_isProcNameExist(string procName);
	bool			pTable_isProcIndexExist(int procIndex);
	set<string>		pTable_GetAllProc();
	void			pTable_AddCall(int procIndex1, int procIndex2);
	vector<int> 	pTable_getCall(int procIndex);
	vector<int> 	pTable_getCalled(int procIndex);
	/**
	 * ConstantTable Part
	 */
	void		cTable_AddConstant(int c);
	set<int>	cTable_GetAllConstants();
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