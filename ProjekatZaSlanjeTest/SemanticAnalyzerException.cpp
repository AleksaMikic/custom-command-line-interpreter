#include "SemanticAnalyzerException.h"

SemanticAnalyzerException::SemanticAnalyzerException(const std::string& m):CPIException(m) {
	wholeMessage += message;
}

SemanticAnalyzerException::SemanticAnalyzerException(const std::string& m, std::vector<CommandBlock> i,std::vector<unsigned int>& p, std::vector<std::string> ms):CPIException(m,p),messages(ms) {
	std::string commandLine = "";
	for (unsigned int j = 0; j < static_cast<unsigned int>(i.size()); ++j) {
		if (j != static_cast<unsigned int>(i.size()) - 1) {
			commandLine += i[j].convertCommandBlockToString() + " | ";
		}
		else {
			commandLine += i[j].convertCommandBlockToString();
		}
	}
	wholeMessage += message +commandLine+ "\n\n";
	for (unsigned int j = 0; j < static_cast<unsigned int>(messages.size()); ++j) {
		wholeMessage += messages[j];
	}
}

const  char* SemanticAnalyzerException::what() const {
	return wholeMessage.c_str();
}

SemanticAnalyzerException::~SemanticAnalyzerException() {

}