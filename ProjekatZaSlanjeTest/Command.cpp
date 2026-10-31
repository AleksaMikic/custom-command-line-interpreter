#include "Command.h"

void Command::changeStreams(StreamController& sc) {
	if (input.checkHasValue()) {
		input.getValue()->changeStream(sc);
	}
	if (output.checkHasValue()) {
		output.getValue()->changeStream(sc);
	}
}