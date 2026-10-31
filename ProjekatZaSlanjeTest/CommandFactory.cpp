#include"CommandFactory.h"

std::unique_ptr<Command> CommandFactory::createCommand(std::string& type,CommandBlock& cb,int cbi,int cbs) {
	if (type == "echo") {
		return std::make_unique<Echo>(type, cb, cbi,cbs);
	}
	else if (type == "prompt") {
		return std::make_unique<Prompt>(type, cb, cbi,cbs);
	}
	else if (type == "time") {
		return std::make_unique<Time>(type, cb, cbi,cbs);
	}
	else if (type == "date") {
		return std::make_unique<Date>(type, cb, cbi,cbs);
	}
	else if (type == "touch") {
		return std::make_unique<Touch>(type, cb, cbi,cbs);
	}
	else if (type == "truncate") {
		return std::make_unique<Truncate>(type, cb, cbi,cbs);
	}
	else if (type == "rm") {
		return std::make_unique<Rm>(type, cb, cbi,cbs);
	}
	else if (type == "wc") {
		return std::make_unique<Wc>(type, cb, cbi,cbs);
	}
	else if (type == "tr") {
		return std::make_unique<Tr>(type, cb, cbi,cbs);
	}
	else if (type == "head") {
		return std::make_unique<Head>(type, cb, cbi,cbs);
	}
	else if (type == "batch") {
		return std::make_unique<Batch>(type, cb, cbi,cbs);
	}
	else{
		return nullptr;
	}
}