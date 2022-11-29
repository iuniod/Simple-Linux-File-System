# SIMPLE LINUX FILE SYSTEM PROJECT (SLFS)

**Name: Iustina Andreea Caramida**

**Name: Bianca Andreea Dumitru**

### Description:
* Short description of the project:
    * The purpouse of this project is to simulate a simple file system.
    * The file system will be able to create, delete, read, write, copy and move files and directories. 
    * The file system stores files and directories in a tree structure.

* The project is written in C and uses the following libraries:
    * *stdio.h*,
    * *stdlib.h*,
    * *string.h*.

* More details about the code:
    * The code is divided into 2 files:
        * `tree.c` - contains the bash commands;
        * `utils.c` - contains utility functions.
    * The code is commented.
    * The code is formatted using *clang-format*.
    * For `tree` command, we used `lsrec` function from `utils.c` in which we remebered the depth of each node and printed the correct number of spaces before the name of the file/folder.
    * Bash commands that are implemented:
        * `touch` - creates a file;
        * `mkdir` - creates a directory;
        * `ls` - lists the files and directories from a given path;
        * `cd` - changes the current directory;
        * `pwd` - prints the current directory path;
        * `tree` - prints the tree structure of the file system;
        * `rm` - removes a file;
        * `rmdir` - removes an empty directory;
        * `rm -r` as `rmrec` - removes a directory and all its content;
        * `cp` - copies a file;
        * `mv` - moves a file;

* How to run the code:
    * Compile the code using `make`.
    * Run the code using `./sfls`.
    * Run the test script using `./check.sh`.