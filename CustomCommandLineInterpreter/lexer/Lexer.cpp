#include "Lexer.h"
#include "LexerProcessor.h"
#include "VisitorException.h"
#include "TextHandler.h"

Lexer::Lexer() :input(), position(0) {

}

//eksplicitni konstruktor klase Lexer
Lexer::Lexer(std::string& i) :input(i),position(0) {
	
}

bool Lexer::accept(Visitor& v) {
	try {
		if (auto* lv = dynamic_cast<LexerProcessor*>(&v)) {
			return lv->visit(*this);
		}
		else {
			throw VisitorException("Wrong visitor\n");
		}
	}
	catch (VisitorException& e) {
		std::cerr << e.what() << "\n";
	}
	return false;
}


//Tokenizuje string unutar navodnika
Token Lexer::tokenizeQuotedString(unsigned int startingPosition) {
	std::string returningString ="";
	++position;
	char c = input[position];
	unsigned int size = static_cast<unsigned int>(input.size());
	while (c != '"' && position<size) {
		returningString+=c;
		++position;
		c = input[position];
	}
	++position;
	return Token(TokenType::QUOTED_STRING, returningString, startingPosition);
}

//tokenizuje rec, odnosno tokenizuje sve sto nisu spcijani znakovi i stringovi unutar nadnika, odnosno komande opcije i imena fajlova
Token Lexer::tokenizeWord(unsigned int startingPosition) {
	std::string returningString = "";
	char c = input[position];
	bool isFile = false;
	unsigned int size = static_cast<unsigned int>(input.size());
	while ((TextHandler::isAlphaNumeric(c)||(c=='-')||c=='.')||(c=='/') && position<size) {
		returningString += c;
		if (c == '.') {
			isFile = true;
		}
		++position;
		c = input[position];
	}
	if (input[startingPosition] == '-') {
		return Token(TokenType::OPTION, returningString, startingPosition);
	}
	else {
		if (isFile) {
			return Token(TokenType::FILE_NAME, returningString, startingPosition);
		}
		else {
			return Token(TokenType::COMMAND, returningString, startingPosition);
		}
	}
}
Token Lexer::tokenizeSpecialCharacters(unsigned int startingPosition) {
	char c = input[position];
	std::string returningString = "";
	if (c == '|') {
		returningString += c;
		++position;
		return Token(TokenType::PIPE, returningString, startingPosition);
	}
	else if (c == '<') {
		returningString += c;
		++position;
		return Token(TokenType::REDIRECT_INPUT, returningString, startingPosition);
	}
	else {
		returningString += c;
		++position;
		c = input[position];
		if (c == '>') {
			returningString += c;
			++position;
			return Token(TokenType::REDIRECT_APPEND, returningString, startingPosition);
		}
		return Token(TokenType::REDIRECT_OUTPUT, returningString, startingPosition);
	}
}

// posebno obradjuje sve tipove tokena zasebno( trenutna obrada samo jednog tokena)
Token Lexer::nextToken() {
	char c = input[position];
	int startPosition = position;
	if(TextHandler::isSpecialCharacter(c)){
		return tokenizeSpecialCharacters(startPosition);
	}
	else if (c == '"') {
		return tokenizeQuotedString(startPosition);
	}
	else{
		return tokenizeWord(startPosition);
	}
}

//Glavna funkcija koja tokenizuje celu komandnu liniju
std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens = {};
	LexerProcessor lp;
	if (accept(lp)) {
		unsigned int size = static_cast<int>(input.size());
		while (position < size) {
			char c = input[position];
			while (TextHandler::isWhiteSpace(c) && position < size) {
				++position;
				c = input[position];
			}
			if (position >= size) break;
			else {
				tokens.push_back(nextToken());
			}
		}
		position = 0;
		return tokens;
	}
	else {
		return tokens;
	}
}