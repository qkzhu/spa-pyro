#include "PKB.h"
#include "VarTable.h"

/**
 * PKB Part
 */
void pkb_AddToASTList(AST* ast, int procIndex);
AST* getASTByProcdureIndex(int procIndex);
int getProcedureIndexByAST(AST* ast);
/**
 * AST Part
 */
AST* PKB::ast_CreateASTnode() {
	return new AST();
}

Node* PKB::ast_CreateNode(AST* ast, Node::NodeType nt, int statNum, int name) {
	return ast->createNode(nt, statNum, name);
}

list<int> ast_GetChild(AST* ast, int stmtNum) {
	return ast->getChild(stmtNum);
}

Node* ast_GetPreviousStatement(AST* ast, Node* d){
	return ast->getPreviousStatement(d);
}

Node* ast_GetFollowingStatement(AST* ast, Node* d){
	return ast->getFollowingStatement(d);
}

Node* ast_GetNodeByStatementNum(AST* ast, int index){
	return ast->getNodeByStatementNum(index);
}

void ast_AddChild(AST* ast, Node *d, Node *childNode){
	ast->addChild(d, childNode);
}

void ast_AddDown(AST* ast, Node *upperNode, Node *bottomNode){
	ast->addDown(upperNode, bottomNode);
}

void ast_AddFollow(AST* ast, Node *d, Node *followNode){
	ast->addFollow(d, followNode);
}

void ast_AddProcedure(AST* ast, Node *d){
	ast->addProcedure(d);
}
/**
 * VarTable Part
 */
void vTable_InsertVar(string);
string vTable_GetVarName(int);
int vTable_GetVarIndex(string);
int vTable_GetVarTableSize(); 
bool vTable_ContainsVar(string);
set<string> vTable_GetAllVar();
/**
 * ProcTable Part
 */
void pTable_InsertProc(string);
string pTable_GetProcName(int);
int pTable_GetProcIndex(string);
int pTable_GetProcTableSize();
bool pTable_ContainsProc(string);
set<string> pTable_GetAllProc();
/**
 * ConstantTable Part
 */
void cTable_AddConstant(int c);
set<int> cTable_GetAllConstants();
bool cTable_IsConstantExist(int c);
int cTable_GetNumOfConstant();