#ifndef PKB_EXCEPTION_H
#define PKB_EXCEPTION_H

#include <string>

using namespace std;


class PKBException
{
public:
	string message;
	PKBException(string msg);
	void Report();
};

class PKBInvalidNodeException:public PKBException{};

class PKBInvalidProcedureException:public PKBException{};

class PKBInvalidInputException:public PKBException{};

#endif