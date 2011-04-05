#ifndef DATASTUCTURE_H
#define DATASTUCTURE_H

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
	//query body
	AND				=	0,
	SELECT			=	1,
	SUCH_THAT		=	2,
	WITH			=	3,
	PATTERN			=	4,
	PARENT			=	5,
    PARENT_T        =	6,
    FOLLOWS			=	7,
	FOLLOWS_T       =	8,
	USES			=	9,
    MODIFIES		=	10,
	CALLS			=	11,
	CALLS_T			=	12,
	NEXT			=   13,
    NEXT_T			=	14,
    AFFECTS			=	15,
    AFFECTS_T       =	16,

	//variable types
	STMT			=	51,
	PROG_L			=	52,
	ASSIGN			=	53,
	WHILE			=	54,	
	IF				=	55,
	CONSTANT        =	56,
	VARIABLE        =	57,
	PROCEDURE		=	58,
	CALL			=	59,
	BOOLEAN			=	60,


	//variable attributes
	VALUE           =	101,
	VARNAME			=	102,
	PROCNAME        =	103,
	STMT_NUM        =	104,	


	//punctuations
	COMMA			=	151,
	S_COLON			=	152,
	L_QUOT			=	153,
	R_QUOT			=	154,
	EQUAL			=	155,
	DOT				=	156,
	UNDERSCORE		=	157,
	QUOTE			=	158,
	L_ARROW			=	159,
	R_ARROW			=	160,
	
	INT				=	201,
	VAROFSIMPLE		=	202,
	PROCOFSIMPLE	=	203,
	PATTERNOFSIMPLE =	205,



	NONEXIST		=	-10
  };

#endif //DATASTUCTURE_H