#pragma once

#ifdef CLIENTWRAPPER_EXPORTS
#define DLLAPI  __declspec(dllexport)
#else
#define DLLAPI  __declspec(dllimport)
#endif

extern "C" DLLAPI int GetLoaderGlobalState();
extern "C" DLLAPI void SetLoaderGlobalState(int state);