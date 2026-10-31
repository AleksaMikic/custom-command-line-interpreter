#include "Argument.h"

Argument::Argument() :mode(ArgumentType::FILE_NAME_ARG) {
	new (&fileName) FileNameArg();
}

Argument::Argument(ArgumentType at, std::string& str) {
	if (at == ArgumentType::FILE_NAME_ARG) {
		mode = at;
		new (&fileName) FileNameArg(str);
	}
	else {
		mode = at;
		new (&quotedString) QuotedString(QuotedStringType::NORMAL, str);
	}
}

Argument::Argument(Argument& a) :mode(a.mode) {
	if (mode == ArgumentType::FILE_NAME_ARG) {
		new (&fileName) FileNameArg(a.fileName);
	}
	else {
		new (&quotedString) QuotedString(a.quotedString);
	}
}

void Argument::setArgument(ArgumentType at, std::string& str) {
	if (mode == ArgumentType::FILE_NAME_ARG) {
		fileName.~FileNameArg();
	}
	else {
		quotedString.~QuotedString();
	}
	mode = at;
	if (at == ArgumentType::FILE_NAME_ARG) {
		new (&fileName) FileNameArg(str);
	}
	else {
		new (&quotedString) QuotedString(QuotedStringType::NORMAL, str);
	}
}

void Argument::changeStream(StreamController& sc) {
	if (mode == ArgumentType::FILE_NAME_ARG) {
		fileName.changeStream(sc);
	}
	else {
		quotedString.changeStream(sc);
	}
}

Argument::~Argument() {
	if (mode == ArgumentType::FILE_NAME_ARG) {
		fileName.~FileNameArg();
	}
	else {
		quotedString.~QuotedString();
	}
}