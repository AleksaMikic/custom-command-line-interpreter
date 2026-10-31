#include "Visitor.h"

Visitor::Visitor(std::vector<unsigned int> ps, std::vector <std::string> ms) :positions(ps), messages(ms) {

}

Visitor::Visitor(Visitor& v) :positions(v.positions), messages(v.messages) {

}

 std::vector<unsigned int>& Visitor::getPositions() {
	return positions;
}
 std::vector<std::string>& Visitor::getMessages() {
	return messages;
}

 Visitor::~Visitor() {

}