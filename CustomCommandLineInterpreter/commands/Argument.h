#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "FileName.h"
#include "QuotedString.h"

enum ArgumentType {
	FILE_NAME_ARG,
	QUOTED_STRING_ARG
};

// Input: a file name or a quoted string
// Output: changes the input stream to the file contents or the string
// Responsibility: holds a command argument of either kind
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