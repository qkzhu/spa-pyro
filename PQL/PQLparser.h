//this class will read pqlqueryfile line by line and parse it 
#ifndef PQLPARSER_H
#define PQLPARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "Validator.h"
#include"Convertor.h"
#include"TokenList.h" 
//#include"QueryTable.h"
using namespace std;

class PqlParser
{
public:
  static int const STEP=100; 
  static int const NORMAL=0;
  static int const GRAMMERDEFINED=100;
  static int const USERDEFINED=300; 
 
  static void parser(string s, QueryTable *table);
private:

  static int prefix(string token);
  static string selectParser(int type,TokenList *qu,QueryTable *table);
  static string suchThatParser(int type,TokenList *qu,QueryTable *table);
  static string withParser(int type,TokenList *qu,QueryTable *table);
  static string patternParser(int type,TokenList *qu,QueryTable *table);
  static string declarParser(string type,TokenList *qu,QueryTable *table);
};
#endif