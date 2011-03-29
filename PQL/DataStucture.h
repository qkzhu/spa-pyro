#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <vector>

using namespace std;

struct Select{
	vector<int> tuple;
};

struct SuchThat{
	bool argumentNoCorrect;
	vector<int> relCond;
};

struct With{
	bool argumentNoCorrect;
	vector <int> attrCond; 
};

struct Declaration{
	vector<int> declaration;
};

struct Pattern_PQL{
	bool argumentNoCorrect;
	int patternStringNo;
	vector<int> expression;
};

enum KEYWORD
  {
	AND=0,
	SELECT=1,
	SUCH_THAT=2,
	WITH=3,
	PATTERN=4,
	USES=9,
    MODIFIES=10,
	CALLS=11,
	CALLS_T=12,
	//VALUE=101,
	INT=201,
	
	ASSIGN=53,
	WHILE=54,	
	IF=55,
	CALL=59,
	STMT=51,
	
	COMMA=151,
	S_COLON	=152,
	L_QUOT	=153,
	R_QUOT	=154,
	EQUAL	=155,
	DOT		=156,
	UNDERSCORE =157,
	QUOTE	=158,
	L_ARROW =159,
	R_ARROW =160,
	BOOLEAN	=60,
	VAROFSIMPLE =	202,
	PATTERNOFSIMPLE = 205
  };

#endif //DATASTRUCTURE_H