# crashdump-analyzer
CrashDumpAnalyzer.exe is a straightforward C++ executable designed to automatically analyze minidump files using the DbgEng.dll debugging engine. This tool simplifies the process of diagnosing and troubleshooting application crashes by providing an automated way to interpret the stack traces.

## How to use :
Command line arguments to launch : 

CrashDumpAnalyzer.exe `-application_path` < application_path > `-pdb_path` < pdb file path> `dumpfile_path` < dumpfile fullpath >


## Sample Command :
```
CrashDumpAnalyzer.exe -application_path C:/hari/files/application_files/ -pdb_path C:/hari/files/pdb_files/ -dumpfile_path C:/hari/files/dump_file.dmp
```


## Reference Links
- [DbgEng.h](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng)
- [Debug Client](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nn-dbgeng-idebugclient4)
- [Debug Symbols](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nn-dbgeng-idebugsymbols3)
- [Debug Control](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/dbgeng/nn-dbgeng-idebugcontrol4)

- [StackOverflow Reference](https://stackoverflow.com/a/8119364)
