#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <vector>
#include "DataStucture.h"
using namespace std;

class dataBase{

public:
	// display functions for debugging
	void displayTable(vector<vector<int>> &table);

	//populate functions
	void populateModifiesTables();
	void populateCallsTables();
	void populateUseTables();
	void populateParentTables();
	void populateFollowsTables();
	void populateNextTables();
	void populateAffectsTables();

	void populateWithTables();

	//populate all tables;
	void populateAllTables();

	// accessor functions
	void getModifyTable(vector<vector<int>> &table);
	void getCallsTable(vector<vector<int>> &table);
	void getUseTable(vector<vector<int>> &table);
	void getParentTable(vector<vector<int>> &table);
	void getFollowsTable(vector<vector<int>> &table);
	void getNextTable(vector<vector<int>> &table);
	void getAffectsTable(vector<vector<int>> &table);

	void getWithTable(vector<vector<int>> &table);



private:
	vector<vector<int>> modifyTable;
	vector<vector<int>> callsTable;
	vector<vector<int>> useTable;
	vector<vector<int>> parentTable;
	vector<vector<int>> followsTable;
	vector<vector<int>> nextTable;
	vector<vector<int>> affectsTable;
	vector<vector<int>> withTable;
	
};

#endif //DATABASE_H