#pragma once
#define NOMINMAX

#include <Windows.h>
#include <dbgeng.h>
#include <string>
#include <atlbase.h>
#include <atlstr.h>
#include <DbgHelp.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <sstream>
#include <shlwapi.h>

#define WINDOWS_SYMBOLS_PATH "C:\Symbols"

class DumpAnalyzer
{
public:

	DumpAnalyzer();

	DumpAnalyzer(std::string exe_path, std::string symbol_path, std::string dumpfile_fullpath);

	bool ExtractStackTraceFromDumpFile();

	std::string GetExceptionType();

	std::string GetModuleVersion(std::string file_name = "ZMAgent.exe");

	std::vector<std::string> GetStackTrace();
	
	~DumpAnalyzer();

private:

	std::string exe_path;
	std::string symbol_path;
	std::string dumpfile_fullpath;
	std::string dump_filename;
	std::string exception_type;
	std::vector<std::string> stacktrace;

	void FormatStackTrace(std::string raw_stacktrace);

	std::string GetExceptionCodeString(ULONG32 exp_code);

	std::string GetVersionString(VS_FIXEDFILEINFO file_info);

};

