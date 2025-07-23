# Simple Shell
<!--- The assignment is available [here](https://docs.google.com/document/d/1A2Ju0sUMoMtlB3iN-bWhv4Vth8AsKCm7nOE5M0FaOe0/edit?usp=sharing). --->
![Shell](images/shell.jpg)


## Introduction

This problem involves building a simple shell program (also known as the terminal program, where you enter commands like `ls` and `cd`). The shell program prompts users with a simple `$ ` prompt. It then reads commands from the user on the command line, and executes them. **It reads and executes commands until the user types Ctrl-D. It does not exit after one command**. This signals that the user wants to exit.  The Ctrl-D will cause functions that read standard input to detect an EOF (End Of File), at which point your program should exit. 

Write your code in **shell.c.**  

This shell is very simple. There are only three <span style="text-decoration:underline;">built-in</span> commands:



* ls
* cd
* last4

The actual Unix shell executes these commands by running programs, but our shell has these commands built in, so you have to write the code for all 3 commands.


## The ls Command

The `ls` command lists files. It can take either no arguments, or one argument.



* With no argument, it lists all files in the current directory.
* With one argument, it lists files in the target directory. If the target directory is not found, it prints `Directory not found`.

Files are listed with a type in front of it. 



* Regular files are shown with `REG` prepended. 
* Directories are shown with `DIR` prepended.  

Example output from the shell program is shown below when the program is run. User input is red and program output is white.


```
$ ls
DIR .
DIR ..
REG shell.o
DIR test
REG Makefile
REG shell
REG README.md
REG shell.c
REG .gitignore
DIR .git
$ ls test
DIR .
DIR ..
REG test_input.txt
REG test_main.cpp
$ ls foo
Directory not found
$ 
```



## The cd Command

The `cd` command changes the current working directory. The command can take either zero or one argument.



* With no arguments, the cd command changes the working directory back to the directory where the simple shell initially started.
* With 1 argument, the cd command changes the working directory to the target directory.


```
$ ls
DIR .
DIR ..
REG shell.o
DIR test
REG Dockerfile
REG Makefile
REG shell
REG README.md
REG shell.c
REG .gitignore
DIR .git
$ cd test
$ ls
DIR .
DIR ..
REG test_input.txt
REG test_main.cpp
$ cd ..
$ cd foo
Directory not found
$
```



## The last4 Command

The `last4` command takes one argument, the name of a file. It prints out :
* -- Last 4 lines --
* The last 4 lines of the file
* -- End --

Here's an example of the last4 command being used to print out the last 4 lines of the file test/moby.txt.

```
$ last4 test/moby.txt
-- Last 4 lines --
world. But the time of his justification has now come. His three whales
are as good whales as were ever published in Wapping, at any rate; and
his stump as unquestionable a stump as any you will find in the western
clearings
-- End --
$ 
```


The command should work when the target file has less than 4 lines as well.


```
$ last4 test/moby-short.txt
-- Last 4 lines --
crippled beggar (or kedger, as the sailors say) holding a painted board
before him, representing the tragic scene in which he lost his leg.
-- End --
$ 
```


If the file can't be opened, it prints "File not found\n".


```
$ last4 non-existent-file
File not found
$ 
```



## System Calls


### Reading Directories



* You can open directories with <code>[opendir(3)](https://www.man7.org/linux/man-pages/man3/fdopendir.3.html)</code>
* You read directories with<code> [readdir(3)](https://man7.org/linux/man-pages/man3/readdir.3.html)</code>
* You need to find out how to read the `struct dirent`.
    * You can use <strong><code>man dirent</code></strong> for more info if that's necessary.
    * It's safe to assume that the member <strong><code>d_type</code></strong> is present for Linux and MacOS.


### Changing Directories



* Get the current working directory with<code> [getcwd(3)](https://man7.org/linux/man-pages/man3/getcwd.3.html)</code>
* Change the current working directory with <code>[chdir(3)](https://linux.die.net/man/3/chdir)</code>
* Remember that <strong><code>"."</code></strong> is the current working directory.
* Remember that <code>".."</code> is the parent directory.


### Reading Files



* Open a file with [fopen(3)](https://man7.org/linux/man-pages/man3/fopen.3.html)
* Find out the position in the file with [ftell(3)](https://man7.org/linux/man-pages/man3/ftell.3p.html)
* Read a line with [getline(3)](https://man7.org/linux/man-pages/man3/getline.3.html)


## Your Code

Write your code in **shell.c.**  

You can build it with 

	make

You can run it with

	make run

Test with

	make test-ls
 	make test-cd
  	make test-last4

Alternatively, to test everything:

 	make test


## Hints



* Do "ls" first with no arguments. It's the easiest.
* Then figure out how to do "ls target"
* Then do "cd target" 
* After that you can write "cd" with no arguments, to change back to the home directory.
* "last4" is the hardest, so leave it to last.
