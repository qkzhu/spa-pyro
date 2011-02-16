#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include "Parser.h"

using namespace std;

const string Parser::keywords[] = { "procedure", "while", "if", "then", "else" };

void Parser::load_file(string name)
{
	if (file != NULL)
	{
		file->close();
		delete file;
		file = NULL;
	}

	filename = name;

	file = new ifstream(filename);
	check_valid_file();
}

 Parser::Parser(VarTable &varTable, AST &ast, string filename) : 
	varTable(varTable), ast(ast), line_num(0),
	 filename(filename), file(new ifstream()), 
	 string_buf(new stringstream(stringstream::in | stringstream::out))
 {
	 file->open(filename, ios::in);
 }

 Parser::~Parser()
 {
	 if (file)
		 file->close();

	 delete file;
	 delete string_buf;
 }

 string Parser::peek_token() 
 {
	 check_valid_file();

	 string output = get_token();
	 int size = output.size();
	 size;

	 string_buf->seekg(-size, ios_base::cur);

	 return output;
 }

 string Parser::get_token()
 {
	 check_valid_file();

	 stringstream output;	 
	 if (has_token())
	 {
		char c = '\0';

		if (is_delimiter((char)string_buf->peek()))
		{
			string_buf->read(&c, sizeof(c));
			char output[2] = { c, '\0' };
			return string(output);
		}

		while (!isspace(string_buf->peek()) && 
			!is_delimiter((char)string_buf->peek()) &&
			!string_buf->eof())
		{
			string_buf->read(&c, sizeof(c));
			output << (char)c;
		}
	 }

	 return output.str();
 }

 bool Parser::has_token()
 {
	 check_valid_file();

	 skip_spaces();

	 if (!string_buf->eof())
		 return true;

	 //keeps reading until a non-space is found.
	 string s;
	 while (string_buf->eof() && getline(*file, s))
	 {
		 delete string_buf;
		 string_buf = new stringstream(stringstream::in | stringstream::out);

		 line_num++;

		 *string_buf << s;
		 skip_spaces();
	 }
	 
	 return !string_buf->eof() || *file;
 }

 void Parser::skip_spaces()
 {
	 //eats all the spaces in the stream
	 char dummy = '\0';
	 while (isspace(string_buf->peek()))
		 string_buf->read(&dummy, sizeof(dummy));
 }

 void Parser::check_valid_file()
 {
	 if (!file->is_open())
	 {
		 cout << "Error opening file: " << filename << endl;
		 exit(1);
	 }
 }

void Parser::error(string expected, string received) 
{
	cout << "Error at line " << line_num << ": expected - \"" << 
		expected << "\" received - \"" << received << "\"" << endl;
	exit(1);
}

void Parser::match(string token) 
{
	string received = get_token();

	if (token != received)
		error(token, received);
}

void Parser::parse_program() 
{
	while(has_token())
	{
		Node *node = parse_procedure();
		ast.addProcedure(node);
	}
}

Node *Parser::parse_procedure() 
{
	match("procedure");
	
	string proc_name = get_token();
	check_valid_name(proc_name);
	Node *node = ast.createNode(Node::PROC, line_num, proc_name);
	
	match("{");
	
	ast.addDown(node, parse_stmt_list());
	
	match("}");

	return node;
}

Node *Parser::parse_stmt_list() 
{
	string tok =peek_token();
	Node *stmt_list = ast.createNode(Node::STMT_LIST, line_num, "");
	Node *prev_node = NULL; 
	
	if (tok != "" && tok != ";" && tok != "}")
	{
		prev_node = parse_assignment();
		ast.addDown(stmt_list, prev_node);
	}

	//continue parsing more statements while
	//1. there are more tokens
	//2. closing brace is not encountered
	while (strcmp(peek_token().c_str(), "") != 0 && 
		strcmp(peek_token().c_str(), "}") != 0) 
	{
		Node *new_node = parse_assignment();
		ast.addFollow(prev_node, new_node);
		prev_node = new_node;
	}
	
	return stmt_list;
}

Node *Parser::parse_assignment() 
{
	string var_name = get_token();
	check_valid_name(var_name);
	
	Node *assign = ast.createNode(Node::ASSIGN, line_num, "");
	Node *lhs = ast.createNode(Node::VAR, line_num, var_name);
	
	ast.addDown(assign, lhs);
	
	match("=");
	
	Node *rhs = NULL;
	string rhs_token = get_token();

	//catch the closing semi-colon
	string closing = get_token();
	
	if (strcmp(closing.c_str(), ";") != 0)
		error(";", closing);
		
	if (is_constant(rhs_token))	
		rhs = ast.createNode(Node::CONST, line_num, rhs_token);
		
	else if(check_variable_exists(var_name))
		rhs = ast.createNode(Node::VAR, line_num, rhs_token);
	
	else
		error("existing variable or constant", rhs_token);
	
	ast.addDown(assign, rhs);
	
	varTable.insertVar(var_name, line_num);
	
	return assign;
}

bool Parser::is_constant(string tok) 
{
	static regex exp("-?\\d+");
	return regex_match(tok.begin(), tok.end(), exp);
}

bool Parser::is_delimiter(char c)
{
	static regex exp("[{};]");
	string tok(&c);
	return regex_match(tok.begin(), tok.end(), exp);
}

bool Parser::check_variable_exists(string var_name) 
{
	return varTable.containsVar(var_name);
}

void Parser::check_valid_name(string var_name)
{
	static regex exp("[A-Za-z][A-Za-z0-9]*");
	static int keywords_len = sizeof(keywords) / sizeof(keywords[0]);
	
	if (!regex_match(var_name.begin(), var_name.end(), exp))
	{
		cout << "Error at line " << line_num << ": Invalid variable name \"" << 
			var_name << "\"" << endl;
		exit(1);
	}
	else
	{
		//checks for matches with keywords
		for (int i = 0; i < keywords_len; i++)
		{
			if (strcmp(var_name.c_str(), keywords[i].c_str()) == 0)
			{
				cout << "Cannot use \"" << var_name << "\" as a variable. " <<
					" (line " << line_num << ")" << endl;
				exit(1);
			}
		}
	}
}

