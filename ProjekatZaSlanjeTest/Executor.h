#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <vector>
#include <string>
#include <memory>
#include "Base.h"
#include "Token.h"
#include "Echo.h"
#include "StreamController.h"
class Executor {
public:
	Executor();
	Executor(std::vector<std::unique_ptr<Command>>&& cs);
	Executor(StreamController&& sc,std::vector<std::unique_ptr<Command>>&& cs);
	Executor(Executor& other);

	void setStreamController(StreamController&& sc) { streamController = std::move(sc); }
	void setCommands(std::vector<std::unique_ptr<Command>>&& cs) { commands = std::move(cs); }
	void execute();
	~Executor();
private:
	StreamController streamController;
	std::vector<std::unique_ptr<Command>> commands;
};

#endif
