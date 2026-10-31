#include "Executor.h"

Executor::Executor() :streamController(), commands() {

}

Executor::Executor(std::vector<std::unique_ptr<Command>>&& cs) :streamController() {
	commands = std::move(cs);
}

Executor::Executor(StreamController&& sc,std::vector<std::unique_ptr<Command>>&& cs){
	streamController = std::move(sc);
	commands = std::move(cs);
}


Executor::Executor(Executor& other) :streamController(std::move(other.streamController)), commands(std::move(other.commands)) {
	
}

void Executor::execute() {
	for (unsigned int i = 0; i < static_cast<unsigned int>(commands.size()); i++) {
		commands[i]->execute(streamController);
	}
	streamController.setDefault();
}

Executor::~Executor(){

}