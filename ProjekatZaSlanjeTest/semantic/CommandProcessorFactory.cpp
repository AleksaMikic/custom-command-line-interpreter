#include "CommandProcessorFactory.h"

std::unique_ptr<CommandVisitor> CommandProcessorFactory::createCommandProcessor(std::string& type,unsigned int cp) {
	if (type == "echo") {
		return std::make_unique<EchoProcessor>(cp);
	}
	else if (type == "prompt") {
		return std::make_unique<PromptProcessor>(cp);
	}
	else if (type == "time") {
		return std::make_unique<TimeProcessor>(cp);
	}
	else if (type == "date") {
		return std::make_unique<DateProcessor>(cp);
	}
	else if (type == "touch") {
		return std::make_unique<TouchProcessor>(cp);
	}
	else if (type == "truncate") {
		return std::make_unique<TruncateProcessor>(cp);
	}
	else if (type == "rm") {
		return std::make_unique<RmProcessor>(cp);
	}
	else if (type == "wc") {
		return std::make_unique<WcProcessor>(cp);
	}
	else if (type == "tr") {
		return std::make_unique<TrProcessor>(cp);
	}
	else if (type == "head") {
		return std::make_unique<HeadProcessor>(cp);
	}
	else if (type == "batch") {
		return std::make_unique<BatchProcessor>(cp);
	}
	else {
		return nullptr;
	}
}