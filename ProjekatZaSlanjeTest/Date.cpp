#define _CRT_SECURE_NO_WARNINGS

#include "Date.h"
#include "DateProcessor.h"
#include "VisitorException.h"
#include <ctime>

Date::Date(std::string& str, CommandBlock& cb,int cbi,int cbs) :Command(str), commandBlock(cb),commandBlockIndex(cbi), commandBlockSize(cbs){

}

Date::Date(Date& other) :Command(other.getCommandName()), commandBlock(other.getCommandBlock()),commandBlockIndex(other.getCommandBlockIndex()),commandBlockSize(other.getCommandBlockSize()) {

}

void Date::create() {
	std::vector<Token>& tokens = getCommandBlock().getTokens();
	for (unsigned int i = 1; i < static_cast<unsigned int>(tokens.size()); i++) {
		if (tokens[i].getTokenType() == TokenType::REDIRECT_INPUT) {
			Redirect ri(RedirectType::INPUT, tokens[i + 1].getValue());
			getInput().setValue(ri);
			i++;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_OUTPUT) {
			Redirect ro(RedirectType::OUTPUT, tokens[i + 1].getValue());
			getOutput().setValue(ro);
			i++;
		}
		else if (tokens[i].getTokenType() == TokenType::REDIRECT_APPEND) {
			Redirect ro(RedirectType::APPEND, tokens[i + 1].getValue());
			getOutput().setValue(ro);
			i++;
		}
	}
}

std::unique_ptr<Command> Date::clone() {
	return std::make_unique<Date>(*this);
}

bool Date::accept(Visitor& v) {
	try {
		if (auto* dp = dynamic_cast<DateProcessor*>(&v)) {
			return dp->visit(*this);
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

std::string Date::getCurrentSystemDate(bool useLocalTime = true) {
	std::string returningString(64, '\0');
	try {
		std::time_t currentTime = std::time(nullptr);

		if (currentTime == static_cast<std::time_t>(-1)) {
			throw CPIException("Error getting time!");
		}

		std::tm* convertedTime;
		if (useLocalTime) {
			convertedTime = std::localtime(&currentTime);
		}
		else {
			convertedTime = std::gmtime(&currentTime);
		}
		if (!convertedTime) {
			throw CPIException("Error in converting time!");
		}

		std::size_t stringSize = std::strftime(&returningString[0], returningString.size(), "%Y-%m-%d", convertedTime);

		if (stringSize == 0) {
			throw CPIException("Error formatting date!");
		}
		returningString.resize(stringSize);
	}
	catch (CPIException& e) {
		std::cerr << e.what() << "\n";
	}
	return returningString;
}


void Date::execute(StreamController& sc) {
	changeStreams(sc);
	std::string workingString = getCurrentSystemDate();
	sc.setOutputCapture(true);
	if ((sc.getOutput().getStreamType() == StreamType::CONSOLE_STREAM && this->getCommandBlockIndex() == this->getCommandBlockSize() - 1)
			||sc.getOutput().getStreamType() != StreamType::CONSOLE_STREAM) {
		sc.getOutput() << workingString;
		sc.getOutput().closeIfFile();
		sc.getOutput() << "\n";
	}
	sc.setInputString(workingString);
	sc.setOutputCout();
}