//#include"PQLpreprocessor.h"
#include <iostream>
#include "query_Evaluator.h"


int main(void)
{

	//The PKB side needs an abstraction. So PQL side can communicate with PKB through the abstraction. 
	//The abstraction needs to contain all the information about PKB
	VarTable var;
	AST ast;
	//cout << "SPA start..." << endl;
	string s ;
	cout << "Please enter the location of the simple file: " << endl;
	cin >> s;
	Parser p(var, ast, s);
	//cout << "PKB parser start..." << endl;
	p.parse_program();
	//cout << "PQL parser start..." << endl;
	QueryTree* tree=PQLpreprocessor::preprocess();
	query_Evaluator e;

	//cout << "PQL evaluator start..." << endl;
	e.evaluate(tree, &ast, &var);  //using AST and VarTable, get value according to the Query Tree
	//cout << "Now get result ..." << endl;
	vector<int> *r = e.getResult();

	int t = r->size();
	if( t == 0){
		cout << "The result of this query is: NONE" << endl;
		system("pause");
		return 0;
	}

	int i = 0;
	cout << "The result of this query is: ";
	for(; i < t; i++){
		cout << r->at(i)<<"  ";
	}
	cout <<";"<< endl;
	system("pause");
	return 0;
}