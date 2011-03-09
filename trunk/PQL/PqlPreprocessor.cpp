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
	//cout << i << endl;
	string indx = Convertor::getKeyword(i);
	vector<string> v;
	string tmp = "\"";
	Tokenize(indx, v, tmp);
	//cout << "It should be correct: "<<v.at(0) << endl;
	return v.at(0);
}

void PqlPreprocessor::Tokenize(const string& str,
                      vector<string>& tokens,
                      string& delimiters)
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}