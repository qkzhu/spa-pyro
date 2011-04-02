#ifndef CONVERTOR_H
#define CONVERTOR_H


//this class will mainly validate the pqlquery and index it; 
#include<fstream>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include"Tokenizer.h"
using namespace std;

class Convertor
{
private:
	//static int index;
	static map<int,string> indexToKeyword;//map the index with the keyword; index->keyword
	static map<string, int> keywordToIndex;//map the keyword with the index; keyword->index
	static map<string, string> shortcutToContent;//s->stmt;
	static bool isPatterned(string s);
public:
	static void getAllIndex(vector<int> &indexlist);
	static void showIndexTable();
	static const int QUATEDSTRING=400;
	static const int PATTERNSTRING=500;
	static bool isQuated(string s);
	static bool isDeclared(string token);
	static int update();//everytime try to update before using it;
	static int getIndex(string token);//given a token, looking up for the coressponding index, and return it, 
	static string getShortCut(string content);
	static string getContent(string shortcut);
	static bool isPureNum(string token);
	
	//if not in the table, then return -1; 
	static string getKeyword(int index);//given a index, looking for the coressponding keyword, and return it,
	static void insertShortcut(string,string);
	static void insertIndex(int,string);
	//if not in the table,then return string "NULL";
};
#endif;