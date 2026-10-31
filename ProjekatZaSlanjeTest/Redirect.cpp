#include "Redirect.h"

void Redirect::changeStream(StreamController& sc) {
	if (getRedirectType() == RedirectType::INPUT) {
		sc.setInputFile(getFilePath());
	}
	else if (getRedirectType() == RedirectType::OUTPUT) {
		sc.setOutputFile(getFilePath(), false);
	}
	else if (getRedirectType() == RedirectType::APPEND) {
		sc.setOutputFile(getFilePath(), true);
	}
}