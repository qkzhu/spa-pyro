#include "PKB.h"


/**
 * AST Part
 */
Node* PKB::ast_CreateNode(Node::NodeType nt, int statNum, int name) {
	return ast->createNode(nt, statNum, name);
}

void PKB::ast_GetChild(vector<int>& result, int stmtNum) {
	ast->getChild(stmtNum, result);
}

Node* PKB::ast_GetPreviousStatement(Node* d){
	return ast->getPreviousStatement(d);
}

int PKB::ast_GetPreviousStatementNum(int stmtNum){
	Node* currNode = ast->getNodeByStatementNum(stmtNum);
	if(currNode == NULL) return -1;
	else{
		Node* previousNode = ast->getPreviousStatement(currNode);
		if(previousNode == NULL) return -1;
		else return previousNode->stmtNum;
	}
}

Node* PKB::ast_GetFollowingStatement(Node* d){
	return ast->getFollowingStatement(d);
}

int PKB::ast_GetFollowingStatementNum(int stmtNum){
	Node * currNode = ast->getNodeByStatementNum(stmtNum);
	if(currNode == NULL) return -1;
	else {
		Node* followNode = ast->getFollowingStatement(currNode);
		if(followNode == NULL) return -1;
		else return followNode->stmtNum;
	}
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

Node *PKB::ast_getParent(Node* n)
{
	return ast->getParent(n);
}

int PKB::ast_getStmtNum(Node* n)
{
	return n->stmtNum;
}

bool PKB::ast_hasStmt(int stmtNum){
	if(stmtNum <= 0) return false;

	int maxStmt = ast->getMaxStmtNum();

	if(maxStmt >= stmtNum) return true;
	else return false;
}

int PKB::ast_getMaxStmtNum(){
	return ast->getMaxStmtNum();
}


void PKB::ast_GetAllAssign(vector<int>& result){
	ast->getAllAssign(result);
}

void PKB::ast_GetAllWhile(vector<int>& result){
	ast->getAllWhile(result);
}

void PKB::ast_GetAllIf(vector<int>& result){
	ast->getAllIf(result);
}

void PKB::ast_GetAllCall(vector<int>& result){
	ast->getAllCall(result);
}

vector<Node*> PKB::ast_GetAllProc(){
	return ast->getAllProc();
}

vector<Node*> PKB::ast_GetDown(Node* n){
	return ast->getAllDown(n);
}

bool PKB::ast_IsIf(int stmt){
	return ast->isIf(stmt);
}

bool PKB::ast_IsWhile(int stmt){
	return ast->isWhile(stmt);
}

bool PKB::ast_IsAssign(int stmt){
	return ast->isAssign(stmt);
}

bool PKB::ast_IsCall(int stmt){
	return ast->isCall(stmt);
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

int PKB::vTable_GetAllVar(){
	/*vector<string> tmp = convertSetToVector(varTable->getAllVar());
	vector<int> result;
	int index;
	for(index = 0; index < (int)tmp.size(); index++)
		result.push_back(vTable_GetVarIndex(tmp[index]));
	if(result.size() == 0) result.push_back(-1);

	return result;*/
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

int PKB::pTable_GetAllProc(){
	/*vector<string> tmp = convertSetToVector(procTable->getAllProc());
	vector<int> result;
	int index;
	for(index = 0; index < (int)tmp.size(); index++)
		result.push_back(pTable_GetProcIndex(tmp[index]));

	if(result.size() == 0) result.push_back(-1);
	return result;*/
	return procTable->getAllProc();
}

void PKB::pTable_AddCall(int procIndex1, int procIndex2){
	procTable->addCall(procIndex1, procIndex2);
}

void PKB::pTable_getCall(vector<int>& result, int procIndex){
	procTable->getCall(procIndex, result);
}

void PKB::pTable_getCalled(vector<int>& result, int procIndex){
	procTable->getCalled(procIndex, result);
}

void PKB::pTable_getCall_(vector<int>& result, int procIndex) {
	procTable->getCall_(procIndex, result);
}
void PKB::pTable_getCalled_(vector<int>& result, int procIndex) {
	procTable->getCalled_(procIndex, result);
}

/**
 * ConstantTable Part
 */
void PKB::cTable_AddConstant(int c){
	constantTable->addConstant(c);
}

void PKB::cTable_GetAllConstants(vector<int>& result){
	convertSetToVector(constantTable->getAllConstants(), result);

	if(result.size() == 0) result.push_back(-1);
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

void PKB::uTable_getUsedVar(vector<int>& result, int stmtNum) {
	convertSetToVector(useTable->getUsedVar(stmtNum), result);
}//end uTable_getUsedVar

void PKB::uTable_getUsedVarPI(vector<int>& result, int procIndex){
	convertSetToVector(useTable->getUsedVarPI(procIndex), result);
}//end uTable_getUsedVarPI

void PKB::uTable_getStmtUses(vector<int>& result, int varIndex){
	convertSetToVector(useTable->getStmtUses(varIndex), result);
}//end uTable_getStmtUses

void PKB::uTable_getProcUses(vector<int>& result, int varIndex){
	convertSetToVector(useTable->getProcUses(varIndex), result);
}//end uTable_getProcUses

/**
 * ModifyTable Part
 */
void PKB::mTable_getModifiedVar(vector<int>& result, int statNum){
	convertSetToVector(modifyTable->getModifiedVar(statNum), result);
}

void PKB::mTable_getModifiedVarPI(vector<int>& result, int procIndex){
	convertSetToVector(modifyTable->getModifiedVarPI(procIndex), result);
}

void PKB::mTable_getStmtModifies(vector<int>& result, int varIndex){
	return convertSetToVector(modifyTable->getStmtModifies(varIndex), result);
}

void PKB::mTable_getProcModifies(vector<int>& result, int procIndex){
	return convertSetToVector(modifyTable->getProcModifies(procIndex), result);
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
void PKB::convertSetToVector(set<int>& setInt, vector<int>& result){

	for(set<int>::iterator it = setInt.begin(); it != setInt.end(); it++)
		result.push_back(*it);
}

void PKB::convertSetToVector(set<string>& setString, vector<string>& result){
	for(set<string>::iterator it = setString.begin(); it != setString.end(); it++)
		result.push_back(*it);
}

/*
 *   Pattern
 */

bool PKB::patternAssign(int stmtNum, string patternLeft, string patternRight)
{
	return pattern.patternAssign(stmtNum, patternLeft, patternRight, *ast, *varTable);
}

//For Debugging
void PKB::printTree()
{
	ast->printTree(*procTable, *varTable);
}

AST* PKB::getAST(){
	return this->ast;
}

VarTable* PKB::getVarTable(){
	return this->varTable;
}

ProcTable* PKB::getProcTable(){
	return this->procTable;
}

ConstantTable* PKB::getConstantTable(){
	return this->constantTable;
}

Uses* PKB::getUseTable(){
	return this->useTable;
}

Modifies* PKB::getModifyTable(){
	return this->modifyTable;
}

void PKB::printVectorInt(vector<int> vec){
	if(vec.size() == 0) return;

	for(vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
	{
		cout<<*it<<", ";
	}
	cout<<endl;
}

string PKB::intToString(int i)
{
   stringstream ss;//create a stringstream
   ss << i;//add number to the stream
   return ss.str();
}


/*
 *   CFG
 */
void PKB::cfg_getNext(vector<int>& theNext, int stmt)
{
	int tm = ast_getMaxStmtNum();
	if(stmt > tm) theNext.push_back(-1);
	else cfg->getNext(theNext, stmt, ast);

}

void PKB::cfg_getNextUp(vector<int>& theNextUp, int stmt)
{
	if(stmt <= 0) theNextUp.push_back(-1);
	else cfg->getNextUp(theNextUp, stmt, ast);
}

void PKB::printCFG()
{
	cout << "Forward map:" << endl;
	cfg->prtMap(cfg->getForwardMap(ast));
	cout << "Reverse map:" << endl;
	cfg->prtMap(cfg->getReverseMap(ast));
}

//return the condition variable 		of the while stmt in its code
int PKB::condWhile(int stmtNum)
{
	return pattern.condWhile(stmtNum, *ast);
}

int PKB::condIf(int stmtNum)
{
	return pattern.condIf(stmtNum, *ast);
}