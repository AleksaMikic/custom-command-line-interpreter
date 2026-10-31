#ifndef QUOTED_STRING_H
#define QUOTED_STRING_H

#include"StreamController.h"
#include <string>

enum QuotedStringType {
	NORMAL,
	WHAT,
	WITH
};

// Input: text from inside double quotes and its role (normal, what, with)
// Output: the stored text, or an input stream made from it
// Responsibility: holds quoted text used as input or as a tr pattern
class QuotedString {
public:
	QuotedString():quotedStringType(QuotedStringType::NORMAL),value(){}
	QuotedString(QuotedStringType qst, std::string val) :quotedStringType(qst), value(val) {}
	
	bool isNormal() { return quotedStringType == QuotedStringType::NORMAL; }
	QuotedStringType getQuotedStringType() { return quotedStringType; }
	std::string& getValue() { return value; }

	void setValue(QuotedStringType qst, std::string& val) { quotedStringType = qst; value = val; }
	void changeStream(StreamController& sc);

	~QuotedString() {}
private:
	QuotedStringType quotedStringType;
	std::string value;
};

#endif