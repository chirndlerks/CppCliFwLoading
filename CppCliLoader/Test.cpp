#include "Test.h"
#include "..\CppCliLib1\Test.h"
#include "..\CppCliLib2\Test.h"

int GetLoaderGlobalState() {
	return GetGlobalState();
}

void SetLoaderGlobalState(int state) {
	SetGlobalState(state);
}

public ref class LoaderWrapper sealed {
public:
	static int Get() {
		return GetLoaderGlobalState();
	}
	static void Set(int state) {
		SetLoaderGlobalState(state);
	}
};