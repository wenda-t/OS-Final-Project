# CSCI 3431: OS-Final-Project

Project written in C that demonstrates core Operating System concepts, including Inter-Process Communication (IPC), Synchronization, Memory Management, and Performance Modeling.

## Project Structure
- `/src`: Contains all C source files.
- `/bin`: Contains compiled executables (generated after build).
- `/logs`: Contains the centralized system logs (generated after boot.sh)
- `boot.sh`: The automated build and execution script.

## Modules
1. **Logging System:** 
A IPC-based logger using Unix Domain Sockets to record system actions with timestamps.
2. **Peterson's Solution:** 
Simulation of mutual exclusion for two processes using busy waiting.
3. **File Management:** 
Basic file operations including Create, Read, Write, Delete, and List.
4. **Memory Allocation:** 
First-fit contiguous memory allocation with support for deallocation and compaction.
5. **Amdahl's Law:** 
Calculation of theoretical speedup and comparison between serial and parallel execution.
6. **Main Menu:** 
A master controller that manages process lifecycle using `fork()`, `exec()`, and `wait()`.

## Installation & Execution
To build and run the project, ensure you are in the root directory and execute the following commands:

```bash
# Grant execution permissions to the build script
chmod +x boot.sh

# Run the build script and start the program
./boot.sh
```

## Group Members
Liam Jay
Liam Ahern
Wenda Tan
Connor Gerrard
Danesh Elahi