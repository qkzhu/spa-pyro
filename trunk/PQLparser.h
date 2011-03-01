//this class will read pqlqueryfile line by line and parse it 
#ifndef PQLPARSER_H
#define PQLPARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "Validator.h"
#include"Convertor.h"
//#include"QueryTable.h"
using namespace std;

class PqlParser
{
public:
  static int const NORMAL=0;
  static int const GRAMMERDEFINED=101;
  static int const USERDEFINED=301; 
  static int prefix(string token);
  static QueryTable parser(string s);
};
#endif