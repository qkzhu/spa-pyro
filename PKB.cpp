#include "PKB.h"


/**
 * AST Part
 */
AST* PKB::ast_CreateASTnode() {
	return new AST();
}

Node* PKB::ast_CreateNode(Node::NodeType nt, int statNum, int name) {
	return ast->createNode(nt, statNum, name);
}

vector<int> PKB::ast_GetChild(int stmtNum) {
	return ast->getChild(stmtNum);
}

Node* PKB::ast_GetPreviousStatement(Node* d){
	return ast->getPreviousStatement(d);
}

int PKB::ast_GetPreviousStatementNum(int stmtNum){
	Node* tmpNode = ast->getNodeByStatementNum(stmtNum);
	return ast_GetPreviousStatement(tmpNode)->stmtNum;
}

Node* PKB::ast_GetFollowingStatement(Node* d){
	return ast->getFollowingStatement(d);
}

int PKB::ast_GetFollowingStatementNum(int stmtNum){
	Node * tmpNode = ast->getNodeByStatementNum(stmtNum);
	return ast->getFollowingStatement(tmpNode)->stmtNum;
}

Node* PKB::ast_GetNodeByStatementNum(int index){
	return ast->getNodeByStatementNum(index);
}

void PKB::ast_AddChild(Node *d, Node *childNode){
	ast->addChild(d, childNode);
}

void PKB::ast_AddDown(Node *upperNode, Node *bottomNode){
	ast->addDown(upperNode, bottomNode);
}

void PKB::ast_AddFollow(Node *d, Node *followNode){
	ast->addFollow(d, followNode);
}

void PKB::ast_AddProcedure(int procIndex, Node *d){
	ast->addProcedure(procIndex, d);
}

int PKB::ast_getParent(int child){
	return ast->getParent(child);
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

vector<int> PKB::uTable_getUsedVar(int stmtNum) {
	return convertSetToVector(useTable->getUsedVar(stmtNum));
}//end uTable_getUsedVar

vector<int> PKB::uTable_getUsedVarPI(int procIndex){
	return convertSetToVector(useTable->getUsedVarPI(procIndex));
}//end uTable_getUsedVarPI

vector<int> PKB::uTable_getStmtUses(int varIndex){
	return convertSetToVector(useTable->getStmtUses(varIndex));
}//end uTable_getStmtUses

vector<int> PKB::uTable_getProcUses(int varIndex){
	return convertSetToVector(useTable->getProcUses(varIndex));
}//end uTable_getProcUses

/**
 * ModifyTable Part
 */
vector<int> PKB::mTable_getModifiedVar(int statNum){
	return convertSetToVector(modifyTable->getModifiedVar(statNum));
}

vector<int> PKB::mTable_getModifiedVarPI(int procIndex){
	return convertSetToVector(modifyTable->getModifiedVarPI(procIndex));
}

vector<int> PKB::mTable_getStmtModifies(int varIndex){
	return convertSetToVector(modifyTable->getStmtModifies(varIndex));
}

vector<int> PKB::mTable_getProcModifies(int procIndex){
	return convertSetToVector(modifyTable->getProcModifies(procIndex));
}

void PKB::mTable_setModify(int stmtNum, int varIndex){
	modifyTable->setModify(stmtNum, varIndex);
}

void PKB::mTable_setModifyPV(int procIndex, int varIndex){
	modifyTable->setModifyPV(procIndex, varIndex);
}



/**
 * PKB Part
 */
vector<int> convertSetToVector(set<int> setInt){
	vector<int> result;

	for(set<int>::iterator it = setInt.begin(); it != setInt.end(); it++)
		result.push_back(*it);

	return result;
}

vector<string> convertSetToVector(set<string> setString){
	vector<string> result;

	for(set<string>::iterator it = setString.begin(); it != setString.end(); it++)
		result.push_back(*it);

	return result;
}