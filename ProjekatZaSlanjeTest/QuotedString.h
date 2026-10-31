#ifndef QUOTED_STRING_H
#define QUOTED_STRING_H

#include"StreamController.h"
#include <string>

enum QuotedStringType {
	NORMAL,
	WHAT,
	WITH
};

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