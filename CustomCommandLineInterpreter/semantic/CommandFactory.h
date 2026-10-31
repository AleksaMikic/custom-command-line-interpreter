#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include <memory>
#include "Echo.h"
#include "Prompt.h"
#include "TimeCommand.h"
#include "Date.h"
#include "Touch.h"
#include "Truncate.h"
#include "Rm.h"
#include "Wc.h"
#include "Tr.h"
#include "Head.h"
#include "Batch.h"

// Input: a command name and its command block
// Output: a new command object of the matching class, or nullptr for an unknown name
// Responsibility: the single place that maps command names to command classes
class CommandFactory {
public:
	static std::unique_ptr<Command> createCommand(std::string& type, CommandBlock& cb,int cbi,int cbs);
};

#endif