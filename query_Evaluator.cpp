#include "query_Evaluator.h"

using namespace std;

query_Evaluator::query_Evaluator(){
	result = query_Result();
}  

void query_Evaluator::evaluate(QueryTree *t, AST *ast, VarTable *var){
	QNode *root = (*t).GetTRoot();

	//Retrieve the select parameters
	QNode *selectClause = (*root).GetChild(1);
	QNode *selectElements = (*selectClause).GetChild(1);
	string type = (*selectElements).GetNodeType();
	string name = (*selectElements).GetNodeName();

	//cout << "Evaluator: Stage 1 finish...." <<endl;
	//Retrive the such that clause information
	QNode *suchThatClause = (*root).GetChild(2);
	//cout << "Evaluator: Stage 2 finish...." <<endl;
	int suchThatSize = (*suchThatClause).GetChildSize();
	//cout << "Evaluator: Stage 3 finish...." <<endl;
	QNode *relations = (*suchThatClause).GetChild(1);
	//cout << "Evaluator: Stage 4 finish...." <<endl;
	QNode *relPara1 = (*relations).GetChild(1);
	//cout << "Evaluator: Stage 5 finish...." <<endl;
	QNode *relPara2 = (*relations).GetChild(2);

	cout << "Evaluator: Data analytic finished...." <<endl;

		//with follows relation
	int stmtNumber = syntaxValidator::getStmtNValue();
	Node* n = ast->getNodeByStatementNum(stmtNumber);
	relations->ShowNode();
	if(relations->GetNodeType() == "FOLLOWS"){
		if(name == relPara2->GetNodeName()){  //DEFAULT that the other variable is stmt#
			Node* Following = ast->getFollowingStatement(n);
			if(Following != NULL) result.addInTuple(Following->stmtNum); 
			else {
				result.addInTuple(-1);
				cout << "Follows: No Following Statement." <<endl;
			}
		}else if(name == relPara1->GetNodeName()){
			Node* Previous = (ast->getPreviousStatement(n));
			if(Previous != NULL) result.addInTuple(Previous->stmtNum); 
			else{
				result.addInTuple(-1);
				cout << "Follows: No Previous Statement." <<endl;
			}
		}
		
		//With follows* relation
	}else if(relations->GetNodeType() == "FOLLOWS*"){
		
		if(name == (*relPara1).GetNodeName()){
			//cout << "comes to the follow* 1" << endl;
			getStar(ast, n, "GET PREVIOUS");
		}else if(name == (*relPara2).GetNodeName()){
			//cout << "comes to the follow* 2" << endl;
			getStar(ast, n, "GET FOLLOWING");
		}
	}else{
		set<int> all = var->getAllStmt();
		vector<int> allVec(all.begin(), all.end());
				result.addInTuple(&allVec);
	}
}

vector<int>* query_Evaluator::getResult(){
	return result.getTuple();
}


void query_Evaluator::getStar(AST* ast, Node *n, string pos){
		if(pos == "GET FOLLOWING"){
			Node* Following = ast->getFollowingStatement(n);
			while(Following != NULL){	
				int stmtN = Following->stmtNum;
				result.addInTuple(stmtN); 
				Following = ast->getFollowingStatement(ast->getNodeByStatementNum(stmtN));
			}
		}else if (pos == "GET PREVIOUS"){
			Node* Previous = ast->getPreviousStatement(n);
			while(Previous != NULL){
				int stmtN = Previous->stmtNum;
				result.addInTuple(stmtN); 
				Previous = ast->getPreviousStatement(ast->getNodeByStatementNum(stmtN));
			}
		}else{ cout << "Follows* : Can not get here, ERROR!" << endl;
			throw "Can not get here, ERROR!";
		}	
}