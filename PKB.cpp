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

int PKB::ast_getParent(AST* ast, int child){
	return ast->getParent(child);
}

vector<int>* PKB::ast_getChild(AST* ast, int parent){
	return &(ast->getChild(parent));
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

bool PKB::vTable_IsVarNameExist(string varName){
	return varTable->isVarNameExist(varName);
}

bool PKB::vTable_IsVarIndexExist(int varIndex){
	return varTable->isVarIndexExist(varIndex);
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

bool PKB::pTable_isProcNameExist(string procName){
	return procTable->isProcNameExist(procName);
}

bool PKB::pTable_isProcIndexExist(int procIndex){
	return procTable->isProcIndexExist(procIndex);
}

set<string> PKB::pTable_GetAllProc(){
	return procTable->getAllProc();
}

void PKB::pTable_AddCall(int procIndex1, int procIndex2){
	procTable->addCall(procIndex1, procIndex2);
}

vector<int> PKB::pTable_getCall(int procIndex){
	return procTable->getCall(procIndex);
}

vector<int> PKB::pTable_getCalled(int procIndex){
	return procTable->getCalled(procIndex);
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

bool PKB::cTable_hasCons(int c){
	return constantTable->isConstantExist(c);
}

/**
 * UseTable Part
 */
void PKB::uTable_setUses(int stmtNum, int varIndex){
	useTable->setUses(stmtNum, varIndex);
}//end uTable_setUses

void PKB::uTable_setUsesPV(int procIndex, int varIndex){
	useTable->setUsesPV(procIndex, varIndex);
}//end uTable_setUsesPV

vector<int>* PKB::uTable_getUsedVar(int stmtNum) {
	set<int> tmp = useTable->getUsedVar(stmtNum);
	vector<int>* result;

	for(set<int>::iterator it = tmp.begin(); it != tmp.end(); it++)
		result->push_back(*it);

	return result;
}//end uTable_getUsedVar

vector<int>* PKB::uTable_getUsedVarPI(int procIndex){
	set<int> tmp = useTable->getUsedVarPI(procIndex);
	vector<int>* result;

	for(set<int>::iterator it = tmp.begin(); it != tmp.end(); it++)
		result->push_back(*it);

	return result;
}//end uTable_getUsedVarPI

vector<int>* PKB::uTable_getStmtUses(int varIndex){
	set<int> tmp = useTable->getStmtUses(varIndex);
	vector<int>* result;

	for(set<int>::iterator it = tmp.begin(); it != tmp.end(); it++)
		result->push_back(*it);

	return result;
}//end uTable_getStmtUses

vector<int> *PKB::uTable_getProcUses(int varIndex){
	set<int> tmp = useTable->getProcUses(varIndex);
	vector<int>* result;

	for(set<int>::iterator it = tmp.begin(); it != tmp.end(); it++)
		result->push_back(*it);

	return result;
}//end uTable_getProcUses