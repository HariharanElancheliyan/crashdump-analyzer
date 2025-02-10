#pragma once
#include <DbgEng.h>
#include <string>

class OutputCallback : public IDebugOutputCallbacks
{
public:
	STDMETHOD(QueryInterface)(THIS_ _In_ REFIID InterfaceId, _Out_ PVOID* Interface);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);
	STDMETHOD(Output)(THIS_ _In_ ULONG Mask, _In_ PCSTR Text);
	std::string GetStackTrace();

private:
	std::string stack_trace;
};

