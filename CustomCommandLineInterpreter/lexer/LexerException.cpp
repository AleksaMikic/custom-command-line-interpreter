#include "LexerException.h"

LexerException::LexerException(const std::string& m) : CPIException(m){
	wholeMessage = m;
}
LexerException::LexerException(const std::string& m, std::vector<unsigned int>& p, std::string& i):CPIException(m,p), input(i) {
	wholeMessage = "Lexical error has occurred!\n" + input + "\n";
	unsigned int indexI=0;
	unsigned int indexJ=0;
	if (!p.empty()) {
		while (indexJ < (unsigned int)p.size() && indexI < (unsigned int)i.size()) {
			if (p[indexJ] == indexI) {
				indexJ++;
				wholeMessage += "^";
			}
			else {
				wholeMessage += " ";
			}
			indexI++;
		}
	}
}

const char* LexerException::what() const noexcept {
	return wholeMessage.c_str();
}

LexerException::~LexerException() {

}