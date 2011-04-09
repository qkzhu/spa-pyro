#ifndef TOKEN_H
#define TOKEN_H
#include "Tokenizer.h"
#include <vector>
#include <iostream>
using namespace std;
class TokenList
{
public:
	TokenList();
	TokenList(string s);
	int tokenList(string s);//token the string s into a tokenlist;
	string getNextToken();//get the next token;
	vector<string> getTokenList();//return the final tokenlist;
	void showTokenList();//show out all the tokens inside the fullVector;
	int getSize();//get the size of the fullVector;
	
private:
	vector<string> fullVector;//store all the finalized tokens; 
	Tokenizer tokenizer;// the object of tokenizer going to be used;
	vector<string> tokenBy(string s,string deliminator);//set the deliminator the tokenizer;
	void mergeVector(vector<string> vector);//merge all the temp tokenlist into the fullVector;
	int TokenListLength(vector<string> s);// get the charater length of the whole tokenlist;

};
#endif;