#ifndef CONVERTOR_H
#define CONVERTOR_H
/*keyword 0-300; userdefined variable 301-400; pattern in the simple 401-500*/

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

	static map<int,string> indexToKeyword;//map the index with the keyword; index->keyword; 0->and
	static map<string, int> keywordToIndex;//map the keyword with the index; keyword->index; and->0
	static map<string, string> shortcutToContent; //declare stmt s; store the s->stmt;
	static map<int, string> indexToDeclaration; //declare stmt s; s 301->s;
	static map<string,int> declarationToIndex; //declare stmt s; s->301;
	static bool isPatterned(string s);         //to justify whether this string is a pattern string _"", ""_, _""_,"";
	static int ToIndexCount(string token);     //to count whether the some string token appear in any of the 
	                                          //map keywordToIndex and declarationToIndex;
	static int ToIndexFind(string token);     // if some string appears in any one of 
	                                          //the map keywordToIndex and declarationToIndex; then get the index of the string;
   
public:
	static string stringToLower(string strToConvert);//convert the string to lower case; 
	static void getAllIndex(vector<int> &indexlist); // this is just for testing purpose;
	                                                 //output all the declaration which is stored in the delcarationToIndex
	                                                  
	static void showIndexTable(); // for self testing                    
	static const int QUATEDSTRING=400;//this is the starting index for string like pattern; first of string will be 401;
	static bool isQuated(string s);   //this string the quated "string"; 
	static bool isDeclared(string token); //this string is one of the declaration;
	static int update();//everytime try to reload the information stored in the maps before using it;
	static int getIndex(string token);//given a token, looking up for the coressponding index, and return it, 
	static string getContent(string shortcut);//declare stmt s; given s get stmt; 
	static bool isPureNum(string token);//whether the string is only composed by number;
	static string getKeyword(int index);//given a index, looking for the coressponding keyword, and return it,
	static void insertShortcut(string,string);//declare insert s,stmt;
	static void insertIndex(int,string);//insert 301 s, into the indexToDeclaration map;
	//for all the "getFunction", 
	//if not in the table,then return string "NULL"or int -1;
};
#endif;