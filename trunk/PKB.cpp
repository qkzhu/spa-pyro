#include "PKB.h"


/**
 * AST Part
 */
AST* PKB::ast_CreateASTnode() {
	return new AST();
}

Node* PKB::ast_CreateNode(AST* ast, Node::NodeType nt, int statNum, int name) {
	return ast->createNode(nt, statNum, name);
}

vector<int> ast_GetChild(AST* ast, int stmtNum) {
	return ast->getChild(stmtNum);
}

Node* PKB::ast_GetPreviousStatement(AST* ast, Node* d){
	return ast->getPreviousStatement(d);
}

Node* PKB::ast_GetFollowingStatement(AST* ast, Node* d){
	return ast->getFollowingStatement(d);
}

Node* PKB::ast_GetNodeByStatementNum(AST* ast, int index){
	return ast->getNodeByStatementNum(index);
}

void PKB::ast_AddChild(AST* ast, Node *d, Node *childNode){
	ast->addChild(d, childNode);
}

void PKB::ast_AddDown(AST* ast, Node *upperNode, Node *bottomNode){
	ast->addDown(upperNode, bottomNode);
}

void PKB::ast_AddFollow(AST* ast, Node *d, Node *followNode){
	ast->addFollow(d, followNode);
}

void PKB::ast_AddProcedure(AST* ast, int procIndex, Node *d){
	ast->addProcedure(procIndex, d);
}
/**
 * VarTable Part
 */
void PKB::vTable_InsertVar(string var){
	varTable->insertVar(var);
}

string PKB::vTable_GetVarName(int index){
	return varTable->getVarName(index);
}

int PKB::vTable_GetVarIndex(string var){
	return varTable->getVarIndex(var);
}

int PKB::vTable_GetVarTableSize(){
	return varTable->getSize();
}

bool PKB::vTable_ContainsVar(string var){
	return varTable->containsVar(var);
}

set<string> PKB::vTable_GetAllVar(){
	return varTable->getAllVar();
}
/**
 * ProcTable Part
 */
void PKB::pTable_InsertProc(string procName){
	procTable->insertProc(procName);
}

string PKB::pTable_GetProcName(int index){
	return procTable->getProcName(index);
}

int PKB::pTable_GetProcIndex(string procName){
	return procTable->getProcIndex(procName);
}

int PKB::pTable_GetProcTableSize(){
	return procTable->getSize();
}

bool PKB::pTable_ContainsProc(string procName){
	return procTable->containsProc(procName);
}

set<string> PKB::pTable_GetAllProc(){
	return procTable->getAllProc();
}
void PKB::pTable_AddCall(int procIndex1, int procIndex2){
	procTable->addCall(procIndex1, procIndex2);
}
/**
 * ConstantTable Part
 */
void PKB::cTable_AddConstant(int c){
	constantTable->addConstant(c);
}

set<int> PKB::cTable_GetAllConstants(){
	return constantTable->getAllConstants();
}

bool PKB::cTable_IsConstantExist(int c){
	return constantTable->isConstantExist(c);
}

int PKB::cTable_GetNumOfConstant(){
	return constantTable->getNumOfConstant();
}