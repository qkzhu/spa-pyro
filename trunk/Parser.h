#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include "VarTable.h"
#include "AST.h"

using namespace std;

class Parser 
{
public:
	Parser(VarTable &varTable, AST &ast, std::string filename);
	void parse_program();
	void load_file(std::string filename); //loads a new source file
	~Parser();

private:
	int line_num;
	VarTable &varTable;
	AST &ast;

	string get_token(); //extracts next token, returns empty string if none found.	
	string peek_token(); //peeks next token, empty string if none found
	bool has_token();//checks whether there are more tokens to be read
	void skip_spaces();

	void error(std::string expected, std::string received);
	void match(std::string token);

	Node *parse_procedure();
	Node *parse_stmt_list();
	Node *parse_assignment();

	bool is_constant(string tok);
	bool is_delimiter(char tok);

	bool check_variable_exists(std::string var_name);
	void check_valid_name(std::string var_name);
	void check_valid_file();

	string filename;
	ifstream *file;
	stringstream *string_buf;

	static const string keywords[];
};