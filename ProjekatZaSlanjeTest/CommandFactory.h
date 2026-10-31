#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include <memory>
#include "Echo.h"
#include "Prompt.h"
#include "Time.h"
#include "Date.h"
#include "Touch.h"
#include "Truncate.h"
#include "Rm.h"
#include "Wc.h"
#include "Tr.h"
#include "Head.h"
#include "Batch.h"

class CommandFactory {
public:
	static std::unique_ptr<Command> createCommand(std::string& type, CommandBlock& cb,int cbi,int cbs);
};

#endif