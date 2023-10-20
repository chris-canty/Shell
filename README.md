# Shell

This Project creates a shell process that emulates a Linux command terminal. Features include external command execution, file I/O, piping, as well as background process handling. Included is a external executable (mytimeout) that can be used inside of the shell.

## Group Members
- **Patric Nurczyk**: pjn20@fsu.edu
- **Chris Canty**: cmc20k@fsu.edu
- **Nathan Wallen**: ndw21b@fsu.edu
## Division of Labor

### Part 1: Prompt
- **Responsibilities**: Needs to make a prompt that should indicate the absolute working directory, the user name, and the machine name.
- **Assigned to**: Patric Nurczyk, Chris Canty

### Part 2: Environment Variables
- **Responsibilities**: Needs to create a way to return the value of environment variables.
- **Assigned to**: Patric Nurczyk, Nathan Wallen

### Part 3: Tilde Expansion
- **Responsibilities**: Needs to convert ~/ to the user's home directory.
- **Assigned to**: Nathan Wallen, Chris Canty

### Part 4: $PATH Search
- **Responsibilities**: Needs to impliment functionality to find the path of external commands.
- **Assigned to**: Patric Nurczyk, Nathan Wallen

### Part 5: External Command Execution
- **Responsibilities**: Needs to implement functionality to run external commands
- **Assigned to**: Chris Canty, Nathan Wallen

### Part 6: I/O Redirection
- **Responsibilities**: Needs to implement functionality so we can replace the keyboard with input from a specified file and redirect output to a designated file.
- **Assigned to**: Chris Canty, Nathan Wallen, Patric Nurczyk

### Part 7: Piping
- **Responsibilities**: Needs to implement functionality to support piping to multiple programs.
- **Assigned to**: Patric Nurczyk, Chris Canty

### Part 8: Background Processing
- **Responsibilities**: Needs to implement functionality to support background processing
- **Assigned to**: Nathan Wallen, Patric Nurczyk

### Part 9: Internal Command Execution
- **Responsibilities**: Need to implement support for the exit, cd, and jobs commands
- **Assigned to**: Nathan Wallen, Chris Canty

### Part 10: External Timeout Executable
- **Responsibilities**: We will implement the external executable/command "mytimeout" that can be executed in our shell.
- **Assigned to**: Patric Nurczyk, Chris Canty

### Extra Credit
- **Responsibilities**: See Extra Credit Section
- **Assigned to**: Patric Nurczyk, Chris Canty, Nathan Wallen

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

## Bugs
- **Bug 1**: Tokens are always delimited by white space, even within " " 
- **Bug 2**: Using the same file for Input and Output will simply truncate the file.
- **Bug 3**: Cannot execute piping and file I/O at the same time.

## Extra Credit
- **Extra Credit 1**: Support unlimited number of pipes [2] **Complete**
- **Extra Credit 2**: Support piping and I/O redirection in a single command [2] **Incomplete**
- **Extra Credit 3**: Shell-ception: Execute your shell from within a running shell process repeatedly [1] **Complete**

## Considerations
Can currently run unlimited pipes, however cannot perform I/O redirection and piping at the same time
