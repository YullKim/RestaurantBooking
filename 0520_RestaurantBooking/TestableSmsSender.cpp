#include "SmsSender.cpp"
#include <iostream>
using namespace std;

class TestableSmsSender : public SmsSender {
public:
	void send(Schedule* schedule) override {
		cout << "[Testing] send method execute from TestableSmsSender class\n";
		sendMethodIsCalled = true;
	}

	bool isSendMethodIsCalled() {
		return sendMethodIsCalled;
	}

private:
	bool sendMethodIsCalled;
};
