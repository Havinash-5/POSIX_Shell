## Custom Linux Shell (Advanced OS Assignment)
##Overview

This project is a custom Linux shell implemented in C++ as part of the Advanced Operating Systems course assignment.
The shell mimics several features of GNU Bash, including command execution, background processes, I/O redirection, pipes, search, history, and autocompletion.

##Compilation & Execution
g++ myshell.cpp -lreadline -o myshell
./myshell


Dependencies:

g++ (C++ compiler)

libreadline-dev (for history + autocompletion)

Features Implemented
Basic Commands

cd <dir> → change directory (., .., -, ~ supported)

pwd → print working directory

ls → list directory contents

Supports flags -a, -l, -al / -la

Supports ls -a -l <dir> format

echo <args> → prints arguments

exit → quits the shell

 Process Handling

Foreground and Background execution using &

Displays PID for background jobs

#Process Info

pinfo [pid] → displays:

Process state (R, S, Z, T with + for foreground)

Memory usage

Executable path

# History

history → prints last 10 commands

history <n> → prints last n commands (max 20)

# Search

search <filename> → recursively searches for file in current directory tree

# Input/Output Redirection

< → input redirection

> → output redirection (overwrite)

>> → output redirection (append)

Examples:

sort < file.txt > sorted.txt
echo "Hello" >> log.txt

# Pipes

Supports multiple pipes:

cat file.txt | grep "hello" | wc -l

#Project Structure
.
├── myshell.cpp   # main shell implementation
├── Makefile      # optional build file
├── README.md     # project documentation


## Limitations / Future Work

Does not yet handle background job management (jobs, fg, bg)

No signal handling (Ctrl+C, Ctrl+Z management)

Autocompletion 
