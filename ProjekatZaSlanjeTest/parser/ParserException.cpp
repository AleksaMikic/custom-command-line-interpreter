#include "ParserException.h"

ParserException::ParserException(const std::string& m) :CPIException(m) {
	wholeMessage = m;
}


ParserException::ParserException(const std::string& m, std::vector<unsigned int>& p, std::vector<CommandBlock>& i,std::vector<std::string> ms)
	:CPIException(m,p), input(i),messages(ms) {
	wholeMessage = "";
	wholeMessage += m+'\n';
	unsigned int j = 0;
	for (unsigned int i = 0; i < static_cast<unsigned int>(input.size()); i++) {
		if (j < static_cast<unsigned int>(positions.size())) {
			if (positions[j] == i) {
				wholeMessage += "Exception in " + std::to_string(i+1) + ". command block:\n";
				wholeMessage += input[i].convertCommandBlockToString();
				while (j < static_cast<unsigned int>(positions.size())) {
					if (positions[j] != i) {
						break;
					}
					wholeMessage += messages[j];
					++j;
				}
				wholeMessage += '\n';
			}
		}
	}

}
const char* ParserException::what() const {
	return wholeMessage.c_str();
}

ParserException::~ParserException() {

}