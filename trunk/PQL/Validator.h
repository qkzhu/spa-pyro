#ifndef VALIDATOR_H
#define VALIDATOR_H


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "QueryTable.h"
#include "Convertor.h"

using namespace std;
class Validator{
public:

	void populateTable(); //read data from file and add them into suchThatTable<vector>
	//void fillTable(vector <vector<int>> table, string fileName);
	void displayTable(vector<vector<int>> table);  // display table for debug

	vector<int> getIndex(vector<vector<int>> table,int name); // get a vector of index if name = nameInsideTable
	
	void checkResults(QueryTable &table);  // show checking result as whole
	void checkSelect(QueryTable &table); // check select clause
	void checkSuchThat(QueryTable &table); // check such that clause
	void checkWith(QueryTable &table); // check with clause
	void checkPattern(QueryTable &table); // check pattern clause

	void checkAssignPattern(const string &str); // check assign string pattern [LOOP]
	void checkPatternString(vector<int> &patternExpression); // check assign string pattern [One Shot]
	void preCheckAssign(const string &str); // pre_check the expression.Check only outside " ". eg, _"x+y"_.

	void checkFisrstCharacter(const vector<vector<int>> &varOfSimple, int i);
	void fillAssignVector(const string &str, vector<int> &expression, vector<int> &separatorPostion, vector<vector<int>> &varOfSimple);
	


	vector<vector<int>>  getsuchThatTable();
	vector<vector<int>>  getwithTable();

	//QueryTable preValidate(QueryTable table); // used to preprocess the table to make table driven more efficient

	/******* For Testing ******/
	

	/**************/
private:
	vector< vector<int> > suchThatTable;
	vector< vector<int> > withTable;
};


#endif // VALIDATOR_H