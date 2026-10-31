#ifndef FILE_NAME_H
#define FILE_NAME_H

#include <string>
#include "StreamController.h"
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

class FileNameArg :public FileName {
public:
	FileNameArg() {}
	FileNameArg(std::string&fpa):FileName(fpa){}
	void changeStream(StreamController& sc);
	~FileNameArg() {  }
};
#endif
