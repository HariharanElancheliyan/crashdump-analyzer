#include "OutputCallback.h"

HRESULT OutputCallback::QueryInterface(_In_ REFIID InterfaceId,_Out_ PVOID* Interface)
{
    *Interface = NULL;

    if (IsEqualIID(InterfaceId, __uuidof(IUnknown)) ||
        IsEqualIID(InterfaceId, __uuidof(IDebugOutputCallbacks)))
    {
        *Interface = (IDebugOutputCallbacks *)this;
        AddRef();
        return S_OK;
    }
    else
    {
        return E_NOINTERFACE;
    }
}

ULONG OutputCallback::AddRef()
{
    return 1;
}


ULONG OutputCallback::Release()
{
	stack_trace.clear();
	return 0;
}


HRESULT OutputCallback::Output( _In_ ULONG Mask, _In_ PCSTR Text)
{
   stack_trace.append(Text);
   return S_OK;
}

std::string OutputCallback::GetStackTrace()
{
	return stack_trace;
}