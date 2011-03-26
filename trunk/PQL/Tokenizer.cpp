
// Tokenizer.cpp


#include "Tokenizer.h"

bool LQuate=false;
bool RQuate=false;

// constructor
Tokenizer::Tokenizer() : buffer(""), token(""), delimiter(DEFAULT_DELIMITER)
{
    currPos = buffer.begin();
}

Tokenizer::Tokenizer(const std::string& str, const std::string& delimiter) : buffer(str), token(""), delimiter(delimiter)
{
    currPos = buffer.begin();
}


// destructor
Tokenizer::~Tokenizer()
{
}




// reset string buffer, delimiter and the currsor position
void Tokenizer::set(const std::string& str, const std::string& delimiter)
{
    this->buffer = str;
    this->delimiter = delimiter;
    this->currPos = buffer.begin();
}

void Tokenizer::setString(const std::string& str)
{
    this->buffer = str;
    this->currPos = buffer.begin();
}

void Tokenizer::setDelimiter(const std::string& delimiter)
{
    this->delimiter = delimiter;
    this->currPos = buffer.begin();
}




// return the next token. If cannot find a token anymore, return "".
std::string Tokenizer::next()
{
	
    if(buffer.size() <= 0) return "";           // skip if buffer is empty

    token.clear();                              // reset token string

    this->skipDelimiter();                      // skip leading delimiters

    // append each char to token string until it meets delimiter
    while(currPos != buffer.end() && (!isDelimiter(*currPos)||(LQuate==true&&RQuate==false)))
    {
        token += *currPos;
		if(*currPos=='\"'&&LQuate==false)
		   LQuate=true;
		else if(*currPos=='\"'&&LQuate==true)
			RQuate=true;
        ++currPos;
    }
	if(LQuate==true&&RQuate==false) throw new std::string("miss RQuate");
	LQuate=false;
	RQuate=false;
    return token;
}




// skip ang leading delimiters
void Tokenizer::skipDelimiter()
{
    while(currPos != buffer.end() && isDelimiter(*currPos))
        ++currPos;
}


// return true if the current character is delimiter
bool Tokenizer::isDelimiter(char c)
{
    return (delimiter.find(c) != std::string::npos);
}
