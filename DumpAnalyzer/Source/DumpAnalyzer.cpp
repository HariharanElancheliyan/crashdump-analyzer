#include "DumpAnalyzer.h"
#include "OutputCallback.h"
#include <regex>

DumpAnalyzer::DumpAnalyzer()
{
}

DumpAnalyzer::DumpAnalyzer(std::string exe_path, std::string symbol_path, std::string dumpfile_fullpath)
	: exe_path(exe_path), symbol_path(symbol_path), dumpfile_fullpath(dumpfile_fullpath)
{
}

std::string DumpAnalyzer::GetExceptionCodeString(ULONG32 exp_code)
{
	std::string ExceptionCodeAsStr = "EXCEPTION_UNKNOWN";

	switch (exp_code)
	{
	case EXCEPTION_ACCESS_VIOLATION:
	{
		ExceptionCodeAsStr = "EXCEPTION_ACCESS_VIOLATION";
		break;
	}
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
	{
		ExceptionCodeAsStr = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		break;
	}
	case EXCEPTION_BREAKPOINT:
	{
		ExceptionCodeAsStr = "EXCEPTION_BREAKPOINT";
		break;
	}
	case EXCEPTION_DATATYPE_MISALIGNMENT:
	{
		ExceptionCodeAsStr = "EXCEPTION_DATATYPE_MISALIGNMENT";
		break;
	}
	case EXCEPTION_FLT_DENORMAL_OPERAND:
	{
		ExceptionCodeAsStr = "EXCEPTION_FLT_DENORMAL_OPERAND";
		break;
	}
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
	{
		ExceptionCodeAsStr = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		break;
	}
	case EXCEPTION_FLT_INEXACT_RESULT:
	{
		ExceptionCodeAsStr = "EXCEPTION_FLT_INEXACT_RESULT";
		break;
	}
	case EXCEPTION_FLT_INVALID_OPERATION:
	{
		ExceptionCodeAsStr = "EXCEPTION_FLT_INVALID_OPERATION";
		break;
	}
	case EXCEPTION_FLT_OVERFLOW:
	{
		ExceptionCodeAsStr = "EXCEPTION_FLT_OVERFLOW";
		break;
	}
	case EXCEPTION_FLT_STACK_CHECK:
	{
		ExceptionCodeAsStr = "EXCEPTION_FLT_STACK_CHECK";
		break;
	}
	case EXCEPTION_FLT_UNDERFLOW:
	{
		ExceptionCodeAsStr = "EXCEPTION_FLT_UNDERFLOW";
		break;
	}
	case EXCEPTION_ILLEGAL_INSTRUCTION:
	{
		ExceptionCodeAsStr = "EXCEPTION_ILLEGAL_INSTRUCTION";
		break;
	}
	case EXCEPTION_IN_PAGE_ERROR:
	{
		ExceptionCodeAsStr = "EXCEPTION_IN_PAGE_ERROR";
		break;
	}
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
	{
		ExceptionCodeAsStr = "EXCEPTION_INT_DIVIDE_BY_ZERO";
		break;
	}
	case EXCEPTION_INT_OVERFLOW:
	{
		ExceptionCodeAsStr = "EXCEPTION_INT_OVERFLOW";
		break;
	}
	case EXCEPTION_INVALID_DISPOSITION:
	{
		ExceptionCodeAsStr = "EXCEPTION_INVALID_DISPOSITION";
		break;
	}
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
	{
		ExceptionCodeAsStr = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		break;
	}
	case EXCEPTION_PRIV_INSTRUCTION:
	{
		ExceptionCodeAsStr = "EXCEPTION_PRIV_INSTRUCTION";
		break;
	}
	case EXCEPTION_SINGLE_STEP:
	{
		ExceptionCodeAsStr = "EXCEPTION_SINGLE_STEP";
		break;
	}
	case EXCEPTION_STACK_OVERFLOW:
	{
		ExceptionCodeAsStr = "EXCEPTION_STACK_OVERFLOW";
		break;
	}
	default:
	{
		ExceptionCodeAsStr = "EXCEPTION_UNKNOWN";
		break;
	}
	}

	return ExceptionCodeAsStr;
}

std::string DumpAnalyzer::GetVersionString(VS_FIXEDFILEINFO file_info)
{
	DWORD majorVersion = HIWORD(file_info.dwFileVersionMS);
	DWORD minorVersion = LOWORD(file_info.dwFileVersionMS);
	DWORD buildNumber = HIWORD(file_info.dwFileVersionLS);
	DWORD revisionNumber = LOWORD(file_info.dwFileVersionLS);

	std::ostringstream version;
	version << majorVersion << '.' << minorVersion << '.' << buildNumber << '.' << revisionNumber;

	return version.str();
}

bool DumpAnalyzer::ExtractStackTraceFromDumpFile()
{
	try
	{
		CComPtr<IDebugClient4> debug_client = nullptr;
		CComPtr<IDebugControl4> debug_control = nullptr;
		CComPtr<IDebugSymbols3> debug_symbol = nullptr;
		HRESULT hr = E_FAIL;

		hr = DebugCreate(__uuidof(IDebugClient4), (void**)&debug_client);

		if (SUCCEEDED(hr))
		{
			hr = debug_client->QueryInterface(__uuidof(IDebugControl4), (void**)&debug_control);

			if (SUCCEEDED(hr))
			{
				hr = debug_client->QueryInterface(__uuidof(IDebugSymbols3), (void**)&debug_symbol);

				if (SUCCEEDED(hr) && debug_client && debug_control && debug_symbol)
				{
					hr = debug_symbol->SetSymbolOptions(SYMOPT_LOAD_LINES);
					hr = debug_symbol->SetSymbolPath(symbol_path.c_str());
					hr = debug_symbol->AppendSymbolPath(WINDOWS_SYMBOLS_PATH);
					hr = debug_symbol->SetImagePath(exe_path.c_str());

					hr = debug_client->OpenDumpFile(dumpfile_fullpath.c_str());
					hr = debug_control->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE);

					if (SUCCEEDED(hr))
					{
						hr = debug_symbol->SetScopeFromStoredEvent();

						OutputCallback output_callback;
						hr = debug_client->SetOutputCallbacks(&output_callback);

						if (SUCCEEDED(hr))
						{
							ULONG Count = 50;
							PDEBUG_STACK_FRAME stack_frame = new DEBUG_STACK_FRAME[Count];
							hr = debug_control->GetStackTrace(NULL, NULL, NULL, stack_frame, Count, &Count);

							if (SUCCEEDED(hr))
							{
								hr = debug_control->OutputStackTrace(DEBUG_OUTCTL_ALL_CLIENTS, stack_frame, Count, DEBUG_STACK_SOURCE_LINE |
									DEBUG_STACK_FRAME_ADDRESSES |
									DEBUG_STACK_COLUMN_NAMES |
									DEBUG_STACK_FRAME_NUMBERS);


								if (SUCCEEDED(hr))
								{
									if (debug_client)
									{
										FormatStackTrace(output_callback.GetStackTrace());

										debug_client->SetOutputCallbacks(NULL);
										debug_client->EndSession(DEBUG_END_PASSIVE);
										return true;
									}
								}
							}
						}
					}

				}
			}

			if (debug_client)
			{
				debug_client->EndSession(DEBUG_END_PASSIVE);
			}
		}
	}
	catch (...)
	{

	}

	return false;
}

void DumpAnalyzer::FormatStackTrace(std::string raw_stacktrace)
{
	try
	{
		std::vector<std::string> formatted_stacktrace;
		std::istringstream ss_stk(raw_stacktrace);
		std::string line;

		while (std::getline(ss_stk, line, '\n'))
		{
			if (!line.empty())
			{
				for (int index = 0; index < 3; index++)
				{
					size_t pos = line.find(" ");
					line = line.substr(pos + 1);
				}

				stacktrace.push_back(line);
			}
		}
	}
	catch (...)
	{

	}

}

std::string DumpAnalyzer::GetExceptionType()
{
	std::string exception_type = "Unknown Exception";

	try
	{
		void* pStreamStart = NULL;
		ULONG uStreamSize = 0;
		MINIDUMP_DIRECTORY* dump_directory = NULL;
		BOOL read_result = FALSE;

		HANDLE dump_file = CreateFile(CString(dumpfile_fullpath.c_str()), FILE_GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);

		if (dump_file != INVALID_HANDLE_VALUE)
		{

			HANDLE file_mapping = CreateFileMapping(dump_file, NULL, PAGE_READONLY, 0, 0, 0);
			LPVOID dumpfile_mapview = MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);

			read_result = MiniDumpReadDumpStream(dumpfile_mapview, ExceptionStream, &dump_directory, &pStreamStart, &uStreamSize);

			if (read_result)
			{
				MINIDUMP_EXCEPTION_STREAM* pExceptionStream = (MINIDUMP_EXCEPTION_STREAM*)pStreamStart;
				if (pExceptionStream != NULL && uStreamSize >= sizeof(MINIDUMP_EXCEPTION_STREAM))
				{
					exception_type = GetExceptionCodeString(pExceptionStream->ExceptionRecord.ExceptionCode);
				}

			}

			this->exception_type = exception_type;

			CloseHandle(dump_file);
			UnmapViewOfFile(dumpfile_mapview);
			FlushViewOfFile(dumpfile_mapview, NULL);
			CloseHandle(file_mapping);
		}
	}
	catch (...)
	{

	}

	return exception_type;
}

std::string DumpAnalyzer::GetModuleVersion(std::string file_name)
{
	void* pStreamStart = NULL;
	ULONG uStreamSize = 0;
	MINIDUMP_DIRECTORY* dump_directory = NULL;
	BOOL read_result = FALSE;
	std::string module_version;

	HANDLE dump_file = CreateFile(CString(dumpfile_fullpath.c_str()), FILE_GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);

	if (dump_file != INVALID_HANDLE_VALUE)
	{

		HANDLE file_mapping = CreateFileMapping(dump_file, NULL, PAGE_READONLY, 0, 0, 0);
		LPVOID dumpfile_mapview = MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);

		read_result = MiniDumpReadDumpStream(dumpfile_mapview, ModuleListStream, &dump_directory, &pStreamStart, &uStreamSize);

		if (read_result)
		{
			MINIDUMP_MODULE_LIST* pModuleList = (MINIDUMP_MODULE_LIST*)pStreamStart;
			for (ULONG i = 0; i < pModuleList->NumberOfModules; ++i)
			{
				const MINIDUMP_MODULE& module = pModuleList->Modules[i];
				MINIDUMP_STRING* minidump_str = reinterpret_cast<MINIDUMP_STRING*>(reinterpret_cast<PBYTE>(dumpfile_mapview) + module.ModuleNameRva);
				CString module_name = CString(minidump_str->Buffer);
				if (module_name.Find(CString(file_name.c_str())) != -1)
				{
					module_version = GetVersionString(module.VersionInfo);
				}
			}

		}

		CloseHandle(dump_file);
		UnmapViewOfFile(dumpfile_mapview);
		FlushViewOfFile(dumpfile_mapview, NULL);
		CloseHandle(file_mapping);
	}

	return module_version;
}

std::vector<std::string> DumpAnalyzer::GetStackTrace()
{
	return stacktrace;
}



DumpAnalyzer::~DumpAnalyzer()
{
}
