# Shell

This Project creates a shell process that emulates a Linux command terminal. Features include external command execution, file I/O, piping, as well as background process handling. Included is a external executable (mytimeout) that can be used inside of the shell.

## File Listing
```
root/
│
├── src/
│ ├── env.c
│ ├── execution.c
│ ├── internal.c
│ ├── job.c
│ ├── shell.c
│ └── timeout.c
│
├── include/
│ ├── env.h
│ ├── execution.h
│ ├── globals.h
│ ├── internal.h
│ ├── job.h
│ ├── shell.h
│ └── timeout.h
│
├── README.md
└── Makefile
```
## How to Compile & Execute

### Requirements
- **Compiler**: gcc

### Compilation
For a C/C++ example:
```bash
make
```
This will build the executable in bin
### Execution
```bash
bin/shell
```
or

```bash
make run
```

This will run the program shell
