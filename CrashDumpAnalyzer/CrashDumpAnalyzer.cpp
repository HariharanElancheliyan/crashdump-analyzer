#include <memory>
#include <fstream>
#include "DumpAnalyzer.h"

bool IsPathExists(const std::string& path) 
{
    DWORD fileAttr = GetFileAttributesA(path.c_str());
    return (fileAttr != INVALID_FILE_ATTRIBUTES && (fileAttr & FILE_ATTRIBUTE_DIRECTORY));
}

bool WriteStackTrace(std::string exception_type, std::vector<std::string> stack_traces)
{
    std::ofstream outfile("stack_traces.txt");
    if (!outfile.is_open()) 
    {
        std::cerr << "Error opening file for writing.\n";
        return false;
    }

    outfile << "Exception Type: " << exception_type << "\n";
    outfile << "Stack Traces:\n";

    for (const auto& trace : stack_traces) {
        outfile << trace << "\n";
    }

    outfile.close();
    return true;
}

int main(int argc, char* argv[]) 
{

    if (argc <= 6)
    {
        std::cerr << "Usage: " << argv[0] << " -application_path <path> -pdb_path <path> -dumpfile_path <path>\n"; 
        return 1; 

        //Sample Arguments
        //CrashDumpAnalyzer -application_path C:/hari/files/application_files/ -pdb_path C:/hari/files/pdb_files/ -dumpfile_path C:/hari/files/dump_file.dmp
    }

    std::string application_path;
    std::string pdb_path;
    std::string dumpfile_path;

    for (int i = 1; i < argc; i += 2) 
    {
        std::string arg = argv[i];
        if (arg == "-application_path") 
        {
            application_path = argv[i + 1];
        }
        else if (arg == "-pdb_path") 
        {
            pdb_path = argv[i + 1];
        }
        else if (arg == "-dumpfile_path") 
        {
            dumpfile_path = argv[i + 1];
        }
        else 
        {
            std::cerr << "Unknown argument: " << arg << "\n";
            return 1;
        }
    }

    if (!IsPathExists(application_path)) 
    {
        std::cerr << "Application path does not exist: " << application_path << "\n";
        return 1;
    }

    if (!IsPathExists(pdb_path)) 
    {
        std::cerr << "PDB path does not exist: " << pdb_path << "\n";
        return 1;
    }

    if (!IsPathExists(dumpfile_path)) 
    {
        std::cerr << "Dumpfile path does not exist: " << dumpfile_path << "\n";
        return 1;
    }

	std::shared_ptr<DumpAnalyzer> dump_analyzer = std::make_shared<DumpAnalyzer>(application_path, pdb_path, dumpfile_path);
	
	if (dump_analyzer 
		&& dump_analyzer->ExtractStackTraceFromDumpFile()
		&& WriteStackTrace(dump_analyzer->GetExceptionType(), dump_analyzer->GetStackTrace()))
	{
        std::cout << "Stack trace extracted successfully\n";
	}
    else
    {
        std::cerr << "Failed to extract stack trace from the dumpfile\n";
    }
}