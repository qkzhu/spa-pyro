#include "PKBException.h"
#include <iostream>


PKBException::PKBException(string msg)
{
	message = msg;
}

void PKBException::Report()
{
	cout<<this->message<<endl;
}