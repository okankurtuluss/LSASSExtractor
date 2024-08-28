# LSASSExtractor
LSASSExtractor is a lightweight tool designed to capture memory dumps of the LSASS process on Windows systems. By leveraging the Windows API, it locates the target process and creates a memory dump, which can be analyzed to extract sensitive information such as passwords, encryption keys, and authentication tokens. 

## Features

- **Targeted Process Dumping:** Finds and dumps the LSASS process.
- **Privilege Adjustment:** Automatically adjusts the process's privileges to enable memory dumping.
- **Minimal Output:** Designed to operate with minimal console output, making it more discreet.

## Usage

To use LSASSExtractor, compile the code in a C++ environment that supports Windows API, such as Visual Studio.

1. **Adjust Privileges:** The tool first adjusts the necessary privileges to allow the dumping of system process memory.
2. **Locate Target Process:** The tool then locates the target process by its executable name.
3. **Create Memory Dump:** Finally, the tool creates a memory dump of the target process in the specified file location.

### Compilation

LSASSExtractor requires the Windows SDK for development and can be compiled using Visual Studio. Ensure that the following libraries are linked during compilation:

- `dbghelp.lib`
- `kernel32.lib`
- `advapi32.lib`

### PoC

![](https://github.com/okankurtuluss/LSASSExtractor/blob/okankurtuluss/main/screenshots/defender.png)

![](https://github.com/okankurtuluss/LSASSExtractor/blob/okankurtuluss/main/screenshots/LSASSExtractor-dump.png)

### Antivirus Scan

![](https://github.com/okankurtuluss/LSASSExtractor/blob/okankurtuluss/main/screenshots/antivirus-scan.png)

