#ifndef STREAM_CONTROLLER_H
#define STREAM_CONTROLLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>

enum StreamType {
	CONSOLE_STREAM,
	FILE_STREAM,
	STRING_STREAM
};

// Input: the keyboard, a file name or a string
// Output: the selected input stream, or its whole content as one string
// Responsibility: hides where input comes from so commands read it the same way
class MyInput {
public:
	MyInput();
	MyInput(MyInput& other)=delete;
	MyInput& operator=(MyInput& other) = delete;
	MyInput(MyInput&& other)noexcept ;
	MyInput& operator=(MyInput&& other)noexcept ;

	void setCin();
	void setFile(const std::string& fileName);
	void setString(const std::string& str);

	template<typename T>
	MyInput& operator>>(T& val) {
		getStream() >> val;
		return *this;
	}
	StreamType getStreamType();
	std::istream& getStream();
	std::string getFullInput();//vrsi kovneziju u stringstream pa vraca ono sto je u njemu
	std::string captureFullInput();//konzumira ceo string
private:
	bool useCin = true;
	std::unique_ptr<std::istream> myIStream;
	StreamType streamType = StreamType::CONSOLE_STREAM;
};

// Input: the console, a file name (overwrite or append) or an in-memory string
// Output: the selected output stream and an optional copy of everything written
// Responsibility: hides where output goes so commands write it the same way
class MyOutput {
public:
	MyOutput();
	MyOutput(MyOutput& other) = delete;
	MyOutput& operator=(MyOutput& other) = delete;
	MyOutput(MyOutput&& other)noexcept ;
	MyOutput& operator=(MyOutput&& other)noexcept;

	void setCout();
	void setFile(const std::string& fileName,bool append=false);
	void setString();

	void setCapture(bool enable);
	void clearCapture();

	void closeIfFile();

	template<typename T>
	MyOutput& operator<<(T& value) {
		auto& stream = getStream();
		stream << value;
		if (capturingEnabled) {
			caputreBuffer << value;
		}
		return *this;
	}
	StreamType getStreamType();
	std::ostream& getStream();
	std::string getString();

private:
	bool useCout = true;
	std::unique_ptr<std::ostream> myOStream;

	bool capturingEnabled = false;
	std::ostringstream caputreBuffer;

	StreamType streamType = StreamType::CONSOLE_STREAM;
};


// Input: redirect and argument requests from commands
// Output: the current input and output streams
// Responsibility: carries data between commands in a pipeline and restores console streams after each line
class StreamController {
public:
	MyInput& getInput() { return input; }
	MyOutput& getOutput() { return output; }

	void setInputCin() { input.setCin(); }
	void setInputFile(const std::string& inputFile) { input.setFile(inputFile); };
	void setInputString(const std::string& str) { input.setString(str); }

	void setOutputCout() { output.setCout(); }
	void setOutputFile(const std::string& outputFile, bool append);
	void setOutputString() { output.setString(); }

	void setOutputCapture(bool enable) { output.setCapture(enable); }
	void clearOutputCapture() { output.clearCapture(); }

	void setDefault() { input.setCin(); output.setCout(); }

	std::string captureInputFull() { return input.captureFullInput(); }
	std::string getFullInputString() { return input.getFullInput(); }
	std::string getOutputString() { return output.getString(); }

	StreamController();
	StreamController(StreamController& other)=delete;
	StreamController& operator=(StreamController& other) = delete;
	StreamController(StreamController&& other)noexcept;
	StreamController& operator=(StreamController&& other)noexcept;
	
private:
	MyInput input;
	MyOutput output;
};
#endif
