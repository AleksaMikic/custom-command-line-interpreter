#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "FileName.h"
#include "QuotedString.h"

enum ArgumentType {
	FILE_NAME_ARG,
	QUOTED_STRING_ARG
};

class Argument {
public:
	Argument();
	Argument(ArgumentType at, std::string& str);
	Argument(Argument& a);

	void setArgument(ArgumentType at, std::string& str);
	ArgumentType getMode() { return mode; }

	void changeStream(StreamController& sc);

	~Argument();
private:
	ArgumentType mode;
	union {
		FileNameArg fileName;
		QuotedString quotedString;
	};
};

#endif