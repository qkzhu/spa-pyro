#ifndef PQL_PREPROCESSOR_H
#define PQL_PREPROCESSOR_H
#include "PQLparser.h"
#include "Convertor.h"
#include "QueryTable.h"

class PqlPreprocessor{

private:
	QueryTable qt;

public:

	PqlPreprocessor(std::string query);

	int selectSize(); //return the number of result variables
	std::vector<int> selectAt(int indx); //take an index and return the result variable at index indx.
	int suchThatSize(); //return the number of such that conditions in such that clause
	std::vector<int> suchThatAt(int indx); //take an indx and return the such that condition in 				index indx
	int withSize(); //return the number of with conditions in the with clause
	std::vector<int> withAt(int indx); //take an indx and return the with condition in index indx

	//int varNum(); //return the number of all query variables
	//int varType(int var); //take an integer form of a query variable and return its type in integer

		//For keyword decoding
	int getIndex(std::string key);
	int procCode(std::string key);  //take a simple procedure name and return the PQL code
	int varCode(std::string key);  //take a simple variable name and return the PQL code
	std::string varDecode(int i);  //take a code of simple variable and return the variable string
	std::string procDecode(int i);  //take the code of simple variable and return the procedure name string
	//int keywordCode(std::string key); //take a query keyword(The one between 0 and 300) and return its integer code
	//std::string keywordDecode(int i); //take a integer of 0~300 and return its coding string


};
#endif