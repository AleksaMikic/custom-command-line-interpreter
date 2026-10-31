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

class CommandProcessorFactory {
public:
	static std::unique_ptr<CommandVisitor> createCommandProcessor(std::string& type,unsigned int cp);
};

#endif
