#include "PKB.h"


/**
 * AST Part
 */
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

vector<int> PKB::ast_GetAllAssign(){
	return ast->getAllAssign();
}

vector<int> PKB::ast_GetAllWhile(){
	return ast->getAllWhile();
}

vector<int> PKB::ast_GetAllIf(){
	return ast->getAllIf();
}

vector<int>	PKB::ast_GetAllCall(){
	return ast->getAllCall();
}

vector<Node*> PKB::ast_GetAllProc(){
	return ast->getAllProc();
}

vector<Node*> PKB::ast_GetDown(Node* n){
	return ast->getAllDown(n);
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

vector<int> PKB::pTable_getCall(int procIndex){
	return procTable->getCall(procIndex);
}

vector<int> PKB::pTable_getCalled(int procIndex){
	return procTable->getCalled(procIndex);
}

vector<int> PKB::pTable_getCall_(int procIndex) {
	return procTable->getCall_(procIndex);
}
vector<int> PKB::pTable_getCalled_(int procIndex) {
	return procTable->getCalled_(procIndex);
}

/**
 * ConstantTable Part
 */
void PKB::cTable_AddConstant(int c){
	constantTable->addConstant(c);
}

vector<int> PKB::cTable_GetAllConstants(){
	vector<int> result = convertSetToVector(constantTable->getAllConstants());

	if(result.size() == 0) result.push_back(-1);
	return result;
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
vector<int> PKB::convertSetToVector(set<int> setInt){
	vector<int> result;

	for(set<int>::iterator it = setInt.begin(); it != setInt.end(); it++)
		result.push_back(*it);

	return result;
}

vector<string> PKB::convertSetToVector(set<string> setString){
	vector<string> result;

	for(set<string>::iterator it = setString.begin(); it != setString.end(); it++)
		result.push_back(*it);

	return result;
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