#include "Driver.h"

using namespace std;

Driver::Driver(){
}

int Driver::getFollowingStatement(int i){
	switch(i){
	case 1: return 2;
	case 2: return 3;
	case 3: return 4;
	case 4: return 7;
	case 5: return 6;
	case 7: return 8;
	default: return -1;
	}
}

int Driver::getPreviousStatement(int i){
	switch(i){
		case 2: return 1;
		case 3: return 2;
		case 4: return 3;
		case 6: return 5;
		case 7: return 4;
		case 8: return 7;
		default: return -1;	
	}
}

int Driver::getProgSize(){
	return 8;
}

vector<int> *Driver::getAllStatements(){
	vector<int> *a ;
	(*a).insert((*a).begin(),1);
	(*a).insert((*a).begin()+1,2);
	(*a).insert((*a).begin()+2,3);
	(*a).insert((*a).begin()+3,4);
	(*a).insert((*a).begin()+4,5);
	(*a).insert((*a).begin()+5,6);
	(*a).insert((*a).begin()+6,7);
	(*a).insert((*a).begin()+7,8);

	return a;
}