#include "FileName.h"

void FileNameArg::changeStream(StreamController& sc) {
	sc.setInputFile(getFilePath());
}