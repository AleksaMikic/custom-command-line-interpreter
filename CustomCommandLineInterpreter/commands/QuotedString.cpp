#include "QuotedString.h"

void QuotedString::changeStream(StreamController& sc){
	sc.setInputString(getValue());
}