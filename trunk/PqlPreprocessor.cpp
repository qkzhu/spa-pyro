#include "PqlPreprocessor.h"


PqlPreprocessor::PqlPreprocessor(string query){
	qt = PqlParser::parser(query);
}

int PqlPreprocessor::selectSize(){
	return qt.selectSize();
}

vector<int> PqlPreprocessor::selectAt(int i){
	return qt.selectAt(i);
}

int PqlPreprocessor::suchThatSize(){
	return qt.suchThatSize();
}

vector<int> PqlPreprocessor::suchThatAt(int i){
	return qt.suchThatAt(i);
}

int PqlPreprocessor::withSize(){
	return qt.withSize();
}

vector<int> PqlPreprocessor::withAt(int i){
	return qt.withAt(i);
}

int PqlPreprocessor::procCode(string key){
	return 1;
}

int PqlPreprocessor::varCode(string key){
	return 2;
}

string PqlPreprocessor::varDecode(int i){
	return "a";
}

string PqlPreprocessor::procDecode(int i){
	return "b";
}