#include "Test.h"

int GetGlobalState() {
	return CSLib::Class1::MyGlobalState;
}

public ref class GetGlobalStateWrapper sealed {
public:
	static int Get() {
		return GetGlobalState();
	}
};