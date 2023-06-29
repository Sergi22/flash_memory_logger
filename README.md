# flash_memory_logger
Flash memory logger simulator

Project consist of:
1. flash memory module that simulates behaviour of flash memory in embedded systems by using C stdio library.
Module considers block-based operation of 4 bytes only, to simplify developepment process, since
this is most common block size encountered in embedded systems. Module provide init, deinit, write, read and erase functions

2. Memory logger module is implemented according to requirements and allow to read logs by ID and write logs. It assumes that log ID is incremental, and uses this information to read logs from the memory. It keeps track of memory utilization by counting used blocks. It erase oldest page when memory gets full.

3. Main is used to demonstrate example of using flash memory module. It writes 8228 bytes of data to trigger erasing of memory of 2 pages, and that attempt to read data that supposed to be erased, data that supposed to be in memory and data from first to page that were overwritten after page erase was executed. It also leaves binary file after execution, that can be inspected afterwards.
It uses a pattern inside data to easily distinguish between logs: First byte of data is 0xAA, and last is 0xFF, in between is LSB of current log ID.

How to use:
If on Windows and Visual studio code is present, compile with C/C++ runner plugin
If no visual studio use following command:
"msys64_folder"\mingw64\bin\gcc.exe -fdiagnostics-color=always -g E:\programming\flash_memory_logger/*.c E:\programming\flash_memory_logger/logger_simulator/*.c E:\programming\flash_memory_logger/logger_simulator/*.h -o E:\programming\flash_memory_logger\main.exe

substitute E:\programming with you local path.

to execute: in cmd run main.exe

If on Linux:
gcc -fdiagnostics-color=always -g /path/to/flash_memory_logger/*.c /path/to/flash_memory_logger/logger_simulator/*.c /path/to/flash_memory_logger/logger_simulator/*.h -o /path/to/flash_memory_logger/main

to execute: ./main
NOTE! Note tested.


