#ifndef OPTION_H
#define OPTION_H

#include<string>

// Input: option text such as -w or -n5
// Output: the stored option text
// Responsibility: holds a command option
class Option {
public:
	Option():value() {}
	Option(std::string v):value(v){}
	std::string& getValue() { return value; }
	void setOption(std::string& v) { value = v; }
	~Option() {};
private:
	std::string value;
};

#endif
