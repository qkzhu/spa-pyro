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
	QueryTable getTable();
	int selectSize(); //return the number of result variables
	void selectAt(vector<int> &tupleTemp, int index); //take an index and return the result variable at index indx.
	int suchThatSize(); //return the number of such that conditions in such that clause
	void suchThatAt(vector<int> &relCondTemp, int index); //take an indx and return the such that condition in 				index indx
	int withSize(); //return the number of with conditions in the with clause
	void withAt(vector<int> &attrCondTemp,int index);//take an indx and return the with condition in index indx
	int patternSize(); //return the number of pattern expressions in the pattern clause
	void patternAt(vector<int> &expression, int index); ////take an indx and return the pattern expression in index.

	//int varNum(); //return the number of all query variables
	//int varType(int var); //take an integer form of a query variable and return its type in integer





		//For keyword decoding
	int getIndex(std::string key);
	string getContent(int indx);
	int procCode(std::string key);  //take a simple procedure name and return the PQL code
	int varCode(std::string key);  //take a simple variable name and return the PQL code
	std::string varDecode(int i);  //take a code of simple variable and return the variable string
	std::string procDecode(int i);  //take the code of simple variable and return the procedure name string
	//int keywordCode(std::string key); //take a query keyword(The one between 0 and 300) and return its integer code
	//std::string keywordDecode(int i); //take a integer of 0~300 and return its coding string

	void Tokenize(const string& str,  vector<string>& tokens,  string& delimiters );
};
#endif