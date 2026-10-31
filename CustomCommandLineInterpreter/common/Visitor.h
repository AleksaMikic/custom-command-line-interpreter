#ifndef VISITOR_H
#define VISITOR_H

#include <vector>
#include <string>

// Input: error positions and messages added by a validator
// Output: the collected positions and messages
// Responsibility: base class of all validators; stores what went wrong
class Visitor {
public:
	Visitor(std::vector<unsigned int> ps, std::vector<std::string> ms);
	Visitor(Visitor& v);
	Visitor() = default;

	virtual std::vector<unsigned int>& getPositions();
	virtual std::vector<std::string>& getMessages();

	virtual ~Visitor();
private:
	std::vector<unsigned int> positions;
	std::vector<std::string> messages;
};

#endif
