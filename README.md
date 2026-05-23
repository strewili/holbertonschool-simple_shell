# Simple Shell

## Table of Contents

1. Project Overview
2. Repository
3. Description
4. Learning Objectives
5. Compilation
6. Usage
7. Interactive Mode
8. Non-Interactive Mode
9. File Input Mode
10. Prompt
11. Command Execution
12. Arguments
13. PATH Handling
14. Builtins
15. Exit
16. Env
17. Setenv
18. Unsetenv
19. Cd
20. Ctrl+C
21. Errors
22. Memory
23. Flow
24. Examples
25. Authors

## Project Overview

Simple Shell is a UNIX command line interpreter.

It is written in the C programming language.

The project is part of the Holberton School curriculum.

The goal is to understand how a shell works.

A shell is a program that receives commands from a user.

The shell reads a command line.

The shell parses the command line.

The shell checks if the command is built into the shell.

The shell searches for external commands.

The shell creates a new process.

The shell executes the requested program.

The shell waits for the program to finish.

The shell displays a prompt again.

This project teaches process control.

This project teaches environment handling.

This project teaches memory management.

This project teaches system calls.

This project teaches error handling.

This project teaches how UNIX programs communicate.

The shell is small.

The shell is simple.

The shell is useful for learning.

The shell is not a full Bash replacement.

The shell implements selected required features.

The shell follows Betty style.

The shell is compiled with strict flags.

The shell should not have memory leaks.

The shell should handle errors carefully.

The shell should behave correctly in interactive mode.

The shell should behave correctly in non-interactive mode.

The shell should handle input from a file.

The shell should support command arguments.

The shell should support PATH lookup.

The shell should support useful builtins.

The shell should keep running after Ctrl+C.

The shell should exit cleanly on EOF.

The shell should return meaningful status values.

## Repository

GitHub repository:
DescriptionSimple Shell reads commands from standard input.
Standard input is usually the keyboard.
Standard input can also be a pipe.
Standard input can also come from a file.
The shell reads one line at a time.
Each line contains one command.
A command may have arguments.
Arguments are separated by spaces.
The command name is stored first.
The arguments follow the command name.
The argument list ends with NULL.
This format is required by execve.
The shell uses fork to create a child process.
The shell uses execve to run an external program.
The shell uses wait or waitpid to wait for the child.
The shell uses environ to pass the environment.
The shell uses PATH to find commands.
The shell uses access to check executables.
The shell uses signal to handle Ctrl+C.
The shell uses malloc for dynamic memory.
The shell uses free to release memory.
The shell uses write to display output.
The shell uses perror for some errors.
The shell uses custom string functions in some files.
The shell supports direct paths.
Example direct path:
sh



/bin/ls

The shell supports command names through PATH.
Example command name:
sh



ls

The shell supports commands with options.
Example:
sh



ls -l

The shell supports commands with many arguments.
Example:
sh



echo hello world

The shell supports builtins.
Builtins are commands handled inside the shell.
Builtins do not always require fork.
Examples of builtins are exit, env, setenv, unsetenv, and cd.
Learning ObjectivesThis project helps explain what happens behind a command line.
It shows how input is read.
It shows how strings are split.
It shows how arguments are stored.
It shows how processes are created.
It shows how programs are executed.
It shows how the parent process waits.
It shows how exit status works.
It shows how environment variables work.
It shows how PATH works.
It shows how errors should be displayed.
It shows how signals can be handled.
It shows why memory must be freed.
It shows why edge cases matter.
It shows why coding style matters.
It shows why simple programs still need careful design.
CompilationCompile the project with:
sh



gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

Another possible binary name is:
sh



gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o simple_shell

The flags are important.
-Wall enables many warnings.
-Werror treats warnings as errors.
-Wextra enables extra warnings.
-pedantic enforces strict C rules.
-std=gnu89 uses the expected C standard.
The project should compile without warnings.
The project should compile without errors.
The binary should be executable.
The binary can be started from the terminal.
UsageRun the shell interactively:
sh



./hsh

Run the shell with a pipe:
sh



echo "ls" | ./hsh

Run the shell with a file:
sh



./hsh commands.txt

Exit the shell:
sh



exit

Exit with a status:
sh



exit 98

Interactive ModeInteractive mode happens when standard input is a terminal.
In interactive mode, the shell prints a prompt.
The user types a command.
The user presses Enter.
The shell executes the command.
The shell prints the prompt again.
Example:
sh



($) ls
($) pwd
($) exit

The shell should not exit when Ctrl+C is pressed.
The shell should print a new prompt after Ctrl+C.
The shell should exit when Ctrl+D is pressed.
Ctrl+D sends end of file.
EOF means there is no more input.
When EOF is received, the shell frees memory.
Then the shell exits.
Non-Interactive ModeNon-interactive mode happens when input comes from a pipe.
Example:
sh



echo "ls -l" | ./hsh

In this mode, the shell should not print a prompt.
The shell reads the command.
The shell executes the command.
The shell exits when input ends.
This mode is useful for automated tests.
This mode is useful for scripts.
This mode is useful for checker programs.
File Input ModeFile input mode happens when a filename is passed.
Example:
sh



./hsh commands.txt

The file contains commands.
Each command is written on its own line.
The shell opens the file.
The shell reads commands from the file.
The shell executes each command.
The shell does not print a prompt.
The shell exits when the file ends.
Example file:
sh



echo hello
ls
pwd

Expected behavior:
sh



hello
AUTHORS README.md main.c shell.h
/path/to/current/directory

If the file cannot be opened, the shell prints an error.
The shell should return a failure status.
PromptThe prompt used by this shell is:
sh



($)

The prompt appears only in interactive mode.
The prompt tells the user the shell is ready.
The prompt should appear before reading input.
The prompt should appear again after command execution.
The prompt should not appear in pipe mode.
The prompt should not appear in file input mode.
Command ExecutionCommand execution begins after parsing.
The shell receives a command name.
The shell receives arguments.
The shell checks if the command is a builtin.
If the command is a builtin, the shell runs it directly.
If the command is external, the shell searches PATH.
If the command is found, the shell forks.
The child process calls execve.
The parent process waits.
When the child exits, the parent gets the status.
The shell stores the status.
The shell continues running.
ArgumentsA command can have no arguments.
Example:
sh



ls

A command can have one argument.
Example:
sh



ls -l

A command can have many arguments.
Example:
sh



echo hello Holberton School

The shell splits the input line by delimiters.
Common delimiters are spaces and tabs.
The command name is argument zero.
For this command:
sh



echo hello

The argument array is:
Add to chat
args[0] = "echo"
args[1] = "hello"
args[2] = NULL

The NULL at the end is required.
execve needs the array to end with NULL.
PATH HandlingPATH is an environment variable.
PATH contains directories.
Directories are separated by colons.
Example:
Add to chat
/usr/local/bin:/usr/bin:/bin

When the user types:
sh



ls

The shell searches for ls in each directory.
The shell may check:
Add to chat
/usr/local/bin/ls
/usr/bin/ls
/bin/ls

The shell uses access to test the file.
The file must exist.
The file must be executable.
If a match is found, the shell executes it.
If no match is found, the shell prints an error.
If the command contains /, PATH is not needed.
Example:
sh



/bin/ls

Example:
sh



./program

BuiltinsBuiltins are commands implemented inside the shell.
They do not need an external executable.
Some builtins change the shell process itself.
For example, cd must change the current shell directory.
If cd ran only in a child process, the parent directory would not change.
This is why cd is a builtin.
The shell supports common builtins.
Supported builtins include:
Add to chat
exit
env
setenv
unsetenv
cd

exit BuiltinThe exit builtin closes the shell.
Usage:
sh



exit

Usage with status:
sh



exit 0

Another example:
sh



exit 98

If no status is provided, the shell exits with the last status.
If the status is invalid, the shell prints an error.
Example invalid status:
sh



exit hello

The shell should handle invalid numbers.
The shell should free memory before exiting.
env BuiltinThe env builtin prints environment variables.
Usage:
sh



env

Example output:
Add to chat
PATH=/usr/bin:/bin
HOME=/home/user
PWD=/home/user/project

The shell uses environ.
environ is a global variable.
environ stores environment strings.
Each environment string has this format:
Add to chat
NAME=value

setenv BuiltinThe setenv builtin creates or updates an environment variable.
Usage:
sh



setenv VARIABLE VALUE

Example:
sh



setenv NAME Holberton

After this, env may show:
Add to chat
NAME=Holberton

If the variable already exists, its value is changed.
If the command fails, an error should be printed.
If arguments are missing, an error should be printed.
unsetenv BuiltinThe unsetenv builtin removes an environment variable.
Usage:
sh



unsetenv VARIABLE

Example:
sh



unsetenv NAME

After removal, NAME should not appear in env output.
If the command fails, an error should be printed.
If the argument is missing, an error should be printed.
cd BuiltinThe cd builtin changes directory.
Usage:
sh



cd DIRECTORY

Example:
sh



cd /tmp

To go to the parent directory:
sh



cd ..

To go home:
sh



cd

If no directory is given, cd uses HOME.
The shell should update PWD after changing directory.
The shell should update OLDPWD when possible.
The shell should handle errors from chdir.
If the directory does not exist, cd should print an error.
Ctrl+C HandlingCtrl+C sends SIGINT.
Without handling SIGINT, the shell may quit.
The shell should not quit when Ctrl+C is pressed.
The shell should print a new line.
The shell should show a new prompt.
The shell uses signal to handle SIGINT.
Example handler behavior:
Add to chat
User presses Ctrl+C
Shell prints new prompt
Shell continues running

EOF HandlingEOF means end of file.
In terminal mode, Ctrl+D sends EOF.
When EOF happens, the shell stops reading.
The shell frees allocated memory.
The shell exits cleanly.
Interactive shells often print a newline before exiting.
ErrorsErrors are part of shell behavior.
If a command is not found, print an error.
Example:
sh



fakecommand

Possible output:
Add to chat
./hsh: 1: fakecommand: not found

If execve fails, perror can be used.
If fork fails, perror can be used.
If setenv fails, print to stderr.
If unsetenv fails, print to stderr.
If cd fails, print to stderr.
Errors should not crash the shell.
The shell should continue after recoverable errors.
Example: file input
File content:

echo hello

pwd

ls

Command:

./hsh commands.txt

The shell reads from commands.txt.

The shell does not print a prompt.

The shell executes all commands.

The shell exits at the end of the file.

Testing
Testing should include interactive behavior.

Testing should include non-interactive behavior.

Testing should include file input mode.

Testing should include commands with arguments.

Testing should include PATH commands.

Testing should include direct path commands.

Testing should include env.

Testing should include exit.

Testing should include setenv.

Testing should include unsetenv.

Testing should include cd.

Testing should include invalid commands.

Testing should include empty lines.

Testing should include Ctrl+C manually.

Testing should include EOF manually.

Example compile test:

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

Example command test:

echo "echo hello" | ./hsh

Expected output:

hello

Example invalid command test:

echo "notacommand" | ./hsh

Expected behavior:

The shell prints an error.

Betty Style
The project should follow Betty style.

Functions should have proper comments.

Lines should not be too long.

Indentation should be correct.

Unused variables should be removed.

Header files should use include guards.

Each file should include only what it needs.

The code should compile with no warnings.

Run Betty if available:

betty *.c *.h

Limitations
This shell is educational.

It is not Bash.

It may not support pipes.

It may not support redirections.

It may not support job control.

It may not support advanced quoting.

It may not support command substitution.

It may not support all shell syntax.

The goal is to implement required project features clearly.

Authors
Shouq Alqarni

Baydir Aldossari

Final Notes
Simple Shell is a small project with many important lessons.

It connects C programming with operating system behavior.

It shows how commands become processes.

It shows how environment variables are passed.

It shows why error handling matters.

It shows why memory management matters.

It shows why testing matters.

It gives a practical view of UNIX internals.

It is a strong foundation for understanding real shells.
```text
holbertonschool-simple_shell
