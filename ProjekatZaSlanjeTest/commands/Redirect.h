#ifndef REDIRECT_H
#define REDIRECT_H
#include <string>
#include "VisitableElement.h"
#include "StreamController.h"

enum RedirectType {
	INPUT,
	OUTPUT,
	APPEND
};

// Input: redirect type (input, output, append) and a file path
// Output: changes the input or output stream to that file
// Responsibility: holds one redirection of a command
class Redirect {
public:
	Redirect(RedirectType rt, std::string fp) :redirectType(rt), filePath(fp) {}
	RedirectType getRedirectType()const { return redirectType; }
	std::string& getFilePath() { return filePath; }

	void changeStream(StreamController& sc);

	~Redirect() {}
private:
	RedirectType redirectType;
	std::string filePath;
};

#endif
