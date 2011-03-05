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

int PqlPreprocessor::getIndex(std::string key){
	return Convertor::getIndex(key);
}

int PqlPreprocessor::procCode(string key){
	string tmp = "\""+key+ "\"";
	return Convertor::getIndex(tmp);
}

int PqlPreprocessor::varCode(string key){
	string tmp = "\""+key+ "\"";
	return Convertor::getIndex(tmp);
}

string PqlPreprocessor::varDecode(int i){
	string indx = Convertor::getKeyword(i);
	Tokenizer s;
	s.set(indx, "\"");
	return s.next();
}

string PqlPreprocessor::procDecode(int i){
	string indx = Convertor::getKeyword(i);
	Tokenizer s;
	s.set(indx, "\"");
	return s.next();
}