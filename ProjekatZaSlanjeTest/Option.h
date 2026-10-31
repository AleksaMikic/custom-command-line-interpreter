#ifndef OPTION_H
#define OPTION_H

#include<string>

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
