#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "QueryTable.h"

using namespace std;
class Validator{
public:

	void populateTable(); //read data from file and add them into suchThatTable<vector>
	//void fillTable(vector <vector<int>> table, string fileName);
	void displayTable(vector<vector<int>> table);  // display table for debug
	vector<int> getIndex(vector<vector<int>> table,int name); // get a vector of index if name = nameInsideTable
	bool checkSuchThat(QueryTable table); // check such that clause
	bool checkWith(QueryTable table); // check with clause
	vector<vector<int>>  getsuchThatTable();
	vector<vector<int>>  getwithTable();
	QueryTable preValidate(QueryTable table); // used to preprocess the table to make table driven more efficient

private:
	vector< vector<int> > suchThatTable;
	vector< vector<int> > withTable;


};
#endif