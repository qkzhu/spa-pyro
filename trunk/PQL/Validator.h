#ifndef VALIDATOR_H
#define VALIDATOR_H


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "QueryTable.h"
#include "Convertor.h"

using namespace std;
class Validator{
public:

	Validator();	

	void displayTable(vector<vector<int>> table);  // display table for debug

	
	void checkResults(QueryTable &table);  // show checking result as whole
	void checkSelect(QueryTable &table); // check select clause
	void checkSuchThat(QueryTable &table); // check such that clause
	void checkWith(QueryTable &table); // check with clause
	void checkPattern_PQL(QueryTable &table); // check pattern clause

	void checkPattern_PQLOne(const string &str); // check assign string pattern [LOOP]
	void checkPattern_PQLTwo(const string &str); // pre_check the expression.Check only outside " ". eg, _"x+y"_.
	void checkPattern_PQLThree(vector<int> &patternExpression); // check assign string pattern [One Shot]
	void checkFisrstCharacter(const vector<vector<int>> &varOfSimple, int i); //The first character of varibale must be letter
	//used to convert pattern assign string to its ASCii value
	void fillAssignVector(const string &str, vector<int> &expression, vector<int> &separatorPostion, vector<vector<int>> &varOfSimple);
	

	//get validation table
	void getValidTable(int relationType, vector<vector<int>> &validationTable);
	//get key word
	void getKeyWord(int type, string &relationType);




	
	//QueryTable preValidate(QueryTable table); // used to preprocess the table to make table driven more efficient

	/******* For Testing ******/
	string getString(int index);

private:
	dataBase validTable;
};


#endif // VALIDATOR_H