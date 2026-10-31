#include "StreamController.h"
void MyInput::setCin() {
	useCin = true;
	streamType = StreamType::CONSOLE_STREAM;
}

void MyInput::setFile(const std::string& fileName) {
	useCin = false;
	myIStream = std::make_unique<std::ifstream>(fileName);
	streamType = StreamType::FILE_STREAM;
}

void MyInput::setString(const std::string& str) {
	useCin = false;
	myIStream = std::make_unique<std::stringstream>(str);
	streamType = StreamType::STRING_STREAM;
}

StreamType MyInput::getStreamType(){
	return streamType;
}

std::istream& MyInput::getStream() {
	if (useCin) {
		return std::cin;
	}
	else {
		return *myIStream;
	}
}


std::string MyInput::captureFullInput() {
	if (useCin) {
		std::stringstream buffer;
		while (true) {
			int cint=std::cin.get();
			if (cint == EOF || std::cin.eof()||cint ==26) break;
			buffer.put(static_cast<char>(cint));
		}
		std::cin.clear();
		return buffer.str();
	}
	if (!myIStream) {
		return "";
	}
	if (streamType == StreamType::STRING_STREAM) {
		return static_cast<std::stringstream*>(myIStream.get())->str();
	}
	// StreamType::FILE
	std::stringstream buffer;
	auto& stream = *myIStream;
	auto pos = stream.tellg(); 
	stream.seekg(0);           
	char c;
	while (stream.get(c)) buffer.put(c);
	stream.clear();            
	stream.seekg(pos);        
	return buffer.str();
}

std::string MyInput::getFullInput() {
	return captureFullInput();
}


MyInput::MyInput() {
	setCin();
}

MyInput::MyInput(MyInput&& other) noexcept
	:useCin(std::move(other.useCin)),
	streamType(std::move(other.streamType)),
	myIStream(std::move(other.myIStream)){
}

MyInput& MyInput::operator=(MyInput&& other) noexcept {
	if (this != &other) {
		useCin = std::move(other.useCin);
		streamType = std::move(other.streamType);
		myIStream = std::move(other.myIStream);
	}
	return *this;
}

void MyOutput::setCout() {
	useCout = true;
	clearCapture();
	streamType = StreamType::CONSOLE_STREAM;
}

void MyOutput::setFile(const std::string& fileName,bool append) {
	useCout = false;
	if (append) {
		myOStream = std::make_unique<std::ofstream>(fileName,std::ios::app);
	}
	else {
		myOStream = std::make_unique<std::ofstream>(fileName);
	}
	clearCapture();
	streamType = StreamType::FILE_STREAM;
}

void MyOutput::setString() {
	useCout = false;
	myOStream = std::make_unique<std::stringstream>();
	clearCapture();
	streamType = StreamType::STRING_STREAM;
}

void MyOutput::setCapture(bool enable) {
	if (enable != capturingEnabled) {
		caputreBuffer.str("");
		capturingEnabled = enable;
	}
}

void MyOutput::closeIfFile() {
	if (streamType == StreamType::FILE_STREAM) {
		std::ofstream* stream = dynamic_cast<std::ofstream*>(&getStream());
		stream->close();
	}
}

void MyOutput::clearCapture() {
	if (capturingEnabled) {
		caputreBuffer.str("");
	}
}

StreamType MyOutput::getStreamType() {
	return streamType;
}

std::ostream& MyOutput::getStream() {
	if (useCout) {
		return std::cout;
	}
	else {
		return *myOStream;
	}
}

std::string MyOutput::getString() {
	if (capturingEnabled) {
		return caputreBuffer.str();
	}
	else if(!useCout && myOStream) {
		return static_cast<std::ostringstream*>(myOStream.get())->str();
	}
	else {
		return "";
	}
}

MyOutput::MyOutput() {
	setCout();
}

MyOutput::MyOutput(MyOutput&& other) noexcept
	: useCout(std::move(other.useCout)),
	streamType(std::move(other.streamType)),
	myOStream(std::move(other.myOStream)),
	caputreBuffer(std::move(other.caputreBuffer.str())),
	capturingEnabled(std::move(other.capturingEnabled))
{
}

MyOutput& MyOutput::operator=(MyOutput&& other) noexcept{
	if (this != &other) {
		useCout = std::move(other.useCout);
		streamType = std::move(other.streamType);
		myOStream = std::move(other.myOStream);
		caputreBuffer.str(other.caputreBuffer.str());
		capturingEnabled = std::move(other.capturingEnabled);
	}
	return *this;
}

StreamController::StreamController() {
	input.setCin();
	output.setCout();
}

StreamController::StreamController(StreamController&& other)noexcept
	:input(std::move(other.input)),
	output(std::move(other.output))
{
}

StreamController& StreamController::operator=(StreamController&& other)noexcept {
	if (this != &other) {
		input = std::move(other.input);
		output = std::move(other.output);
	}
	return *this;
}

void StreamController::setOutputFile(const std::string& outputFile, bool append=false){
	output.setFile(outputFile, append);
}
