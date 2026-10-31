#ifndef FILE_NAME_H
#define FILE_NAME_H

#include <string>
#include "StreamController.h"
// Input: a file path
// Output: the stored path
// Responsibility: holds the target file of commands that work on files (touch, truncate, rm)
class FileName {
public:
	FileName() {}
	FileName(std::string& fp):filePath(fp){}
	std::string& getFilePath() { return filePath; }
	void setFilePath(std::string& fp) { filePath = fp; }
	~FileName() {}
private:
	std::string filePath;
};

// Input: a file path given as a command argument
// Output: changes the input stream to that file
// Responsibility: a file name that is read as a command's input
class FileNameArg :public FileName {
public:
	FileNameArg() {}
	FileNameArg(std::string&fpa):FileName(fpa){}
	void changeStream(StreamController& sc);
	~FileNameArg() {  }
};
#endif
