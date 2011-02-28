//this class will mainly validate the pqlquery and index it; 
#include<fstream>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include"Tokenizer.h"
using namespace std;

class Convertor
{
private:

	static map<int,string> indexToKeyword;//map the index with the keyword; index->keyword
	static map<string, int> keywordToIndex;//map the keyword with the index; keyword->index
	static map<string, string> shortcutToContent;//s->stmt;
public:
	static bool isDeclar(string token);
	static int update();//everytime try to update before using it;
	static int getIndex(string token);//given a token, looking up for the coressponding index, and return it, 
	static string getShortCut(string content);
	static string getContent(string shortcut);
										//if not in the table, then return -1; 
	static string getKeyword(int index);//given a index, looking for the coressponding keyword, and return it,
	static void insertShortcut(string,string);
	static void insertIndex(int,string);
	//if not in the table,then return string "NULL";

};