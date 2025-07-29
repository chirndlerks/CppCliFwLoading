#include "Test.h"

void SetGlobalState(int state) {
	CSLib::Class1::SetGlobalState(state);
}

public ref class SetGlobalStateWrapper sealed {
public:
	static void Set(int state) {
		SetGlobalState(state);
	}
};