#ifndef PQL_PREPROCESSOR_H
#define PQL_PREPROCESSOR_H
#include "PQLparser.h"
#include "Convertor.h"
#include "QueryTable.h"

class PqlPreprocessor{

private:
	QueryTable qt;
	Validator validator;
public:

	PqlPreprocessor(std::string query);
	QueryTable getTable();

	int selectSize(); //return the number of result variables
	void selectAt(vector<int> &tupleTemp, int index); //take an index and return the result variable at index index

	int suchThatSize(); //return the number of such that conditions in related partitions
	void suchThatAt(vector<int> &relCondTemp, int index); //take an index and return the related such that condition 

	int withSize(); //return the number of with conditions in the related partition
	void withAt(vector<int> &attrCondTemp,int index);//take an index and return the related with condition 

	int patternSize(); //return the number of pattern expressions in the related partition
	void patternAt(vector<int> &expression, int index); //take an indx and return the related pattern expression
	
	int suchThatUnrelatedSize(); //return the number of such that conditions in Unrelated partitions
	void suchThatUnrelatedAt(vector<int> &relCondTemp, int index); //take an index and return the Unrelated such that condition

	int withUnrelatedSize();  //return the number of with conditions in the Unrelated partition
	void withUnrelatedAt(vector<int> &attrCondTemp, int index); //take an index and return the Unrelated with condition 

	int patternUnrelatedSize(); //return the number of pattern expressions in the Unrelated partition
	void patternUnrelatedAt(vector<int> &expression, int index); //take an indx and return the Unrelated pattern expression


	int affectsStarSize(); //return the number of affects* clauses in the whole query
	int affectsSize(); //return the number of affects clauses in the whole query
	


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