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
	int tokenList(string s);
	 string getNextToken();
	 vector<string> getTokenList();
	 void showTokenList();
	 int getSize();
	
private:
	// static int index;
	 vector<string> fullVector;
	 Tokenizer tokenizer;
	 vector<string> tokenBy(string s,string deliminator);
	 void mergeVector(vector<string>vector);
	 int TokenListLength(vector<string> s);
};
#endif;