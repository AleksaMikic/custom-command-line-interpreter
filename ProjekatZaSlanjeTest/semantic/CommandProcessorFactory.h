#ifndef COMMAND_PROCESSOR_FACTORY_H
#define COMMAND_RPOCESSOR_FACTORY_H

#include <memory>
#include <string>
#include "Command.h"
#include "EchoProcessor.h"
#include "PromptProcessor.h"
#include "TimeProcessor.h"
#include "DateProcessor.h"
#include "TouchProcessor.h"
#include "TruncateProcessor.h"
#include "RmProcessor.h"
#include "WcProcessor.h"
#include "TrProcessor.h"
#include "HeadProcessor.h"
#include "BatchProcessor.h"

// Input: a command name and the block position
// Output: a new validator for that command, or nullptr for an unknown name
// Responsibility: the single place that maps command names to their validators
class CommandProcessorFactory {
public:
	static std::unique_ptr<CommandVisitor> createCommandProcessor(std::string& type,unsigned int cp);
};

#endif
