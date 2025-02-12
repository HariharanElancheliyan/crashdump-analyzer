# crashdump-analyzer
CrashDumpAnalyzer.exe is a straightforward C++ executable designed to automatically analyze minidump files using the DbgEng.dll debugging engine. This tool simplifies the process of diagnosing and troubleshooting application crashes by providing an automated way to interpret the stack traces.

## How to use :
Command line arguments to launch : 

CrashDumpAnalyzer.exe `-application_path` < application_path > `-pdb_path` < pdb file path> `dumpfile_path` < dumpfile fullpath >


## Sample Command :
```
CrashDumpAnalyzer.exe -application_path C:/hari/files/application_files/ -pdb_path C:/hari/files/pdb_files/ -dumpfile_path C:/hari/files/dump_file.dmp
```
## Sample Output
```
Exception Type: EXCEPTION_INT_DIVIDE_BY_ZERO
Stack Traces:
RetAddr  
test_application!main+0x29 [D:\TestingSpace\test_application\test_application\test_application.cpp @ 67]
test_application!__scrt_common_main_seh+0xfa [D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl @ 288]
```
We can extract the stack traces from the minidump.

## Reference Links
- [DbgEng.h](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng)
- [Debug Client](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nn-dbgeng-idebugclient4)
- [Debug Symbols](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nn-dbgeng-idebugsymbols3)
- [Debug Control](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nn-dbgeng-idebugcontrol4)

- [StackOverflow Reference](https://stackoverflow.com/a/8119364)
