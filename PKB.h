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
	//bool hasProc(int procIndex);
	//bool hasVar(int varIndex);


	vector<int> getAllStmts();
	vector<int> getAllAssign(); //get all assignment stmts
	vector<int> getAllWhile();
	vector<int> getAllIf();
	vector<int> getAllCall();
	vector<int> getAllProc();

	//int getVarIndex(string var); //return the code for a variable                //Need changes
	//int getProcIndex(string proc); //return the code for a procedure
	//string getVarName(int key);	//return the variable name given its code
	//string getProcName(int key); //return the procedure name given its code

	//vector<int> *getCalls(int proc_index);
	//vector<int> *getCalled(int proc_index);

	//vector<int> *getStmtModifies(int varIndex);  //take a variable code and return all stmts that modify it 
	//vector<int> *getProcModifies(int varIndex);  //return all procs that modify it
	//vector<int> *getModifiedVar(int stmtNum); //take a stmt# and return the varName code that it modifies
	//vector<int> *getModifiedVarPI(int procIndex); //No need to return vector, but for convenience, use vector

	//vector<int> *getUsedVar(int stmtNum);  //return all varName code that this stmt uses
	//vector<int> *getUsedVarPI(int procIndex); //return all varName code that this proc uses
	//vector<int> *getStmtUses(int varIndex); //return all stmt# that uses this var in its code
	//vector<int> *getProcUses(int varIndex); //return all procName in code that uses this var in its code

	int getFollowingStatement(int d);  //Get the follow node of given node d, with follow relation
	int getPreviousStatement(int d); //Get the previous node of given node d, with the follow relation


	//int getParent(int child);
	//vector<int> *getChild(int parent);  


	///////////////////////////////////////////////////////////////////////////////////////////////


	/**
	 * PKB Part
	 */
	//init vTable, cTable, mTable, uTable, pTable
	PKB(): varTable(new VarTable()),
		   procTable(new ProcTable()),
		   constantTable(new ConstantTable()),
		   useTable(new Uses()){}
	/**
	 * AST Part
	 */
	AST*		ast_CreateASTnode();
	Node*		ast_CreateNode(AST* ast, Node::NodeType nt, int statNum, int name);
	vector<int> ast_GetChild(AST* ast, int stmtNum);
	Node*		ast_GetPreviousStatement(AST* ast, Node* d);
	int			ast_GetPreviousStatementNum(AST* ast, int stmtNum);
	Node*		ast_GetFollowingStatement(AST* ast, Node* d);
	int			ast_GetFollowingStatementNum(AST* ast, int stmtNum);
	Node*		ast_GetNodeByStatementNum(AST* ast, int index);
	void		ast_AddChild(AST* ast, Node *d, Node *childNode);
	void		ast_AddDown(AST* ast, Node *upperNode, Node *bottomNode);
	void		ast_AddFollow(AST* ast, Node *d, Node *followNode);
	void		ast_AddProcedure(AST* ast, int procIndex, Node *d);
	int			ast_getParent(AST* ast, int child);
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
	Node*			node;
	VarTable*		varTable;
	ProcTable*		procTable;
	ConstantTable*	constantTable;
	Uses*			useTable;
	Modifies*		modifyTable;
	vector<int>		convertSetToVector(set<int>);
	vector<string>	convertSetToVector(set<string>);
};