[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/fzOjc2Dn)
# Ultron

# Introduction

For this homework, we are going to work with pointers and built-in arrays. We are also going to build our own computer, called Ultron. Rather than soldering chips together, we are building a software-based simulation of a computer in C. We can write programs and debug our programs that execute on Ultron using our own programming language called U.


# Memory and Registers

The Ultron has a memory of a certain size. The size is configured when the Ultron object is created. The memory is a simple array of integers. You can configure Ultron with a memory size of up to 100 integers, but for our purposes, we will use Ultron memory that is very small, e.g. 32 integers. 

Ultron has two special _registers_ that hold integers. Registers contain information that Ultron uses in special ways. One register is the _accumulator_. Ultron uses this to store and hold values in calculations. The other register is the _program counter_. The program counter tells Ultron which instruction to execute next in the program. Once an instruction is fetched, the program counter advances to the next location.


# Programming Ultron

Despite the name, Ultron is a very simple computer. Programs in Ultron are written in the U programming language. Even though it's simple, it's perfectly possible to write programs that do sophisticated things, for example, a loop that prints numbers from 1 to 100.

All information in Ultron programs is handled in terms of instructions. **An instruction is a 4 digit number**, e.g. 1010, 2010, 3011, 4300, etc. These instructions are referenced by their location numbers. For example, when an Ultron has a memory size of 32, the instructions are in locations from 0, 1, 2, .. 31.

Each instruction in U is 4 digits, and it occupies one location of Ultron's memory. The first two digits of each U instruction is the _operation_. This specifies what Ultron should do. Here are the operations. 


```
#define READ       10        // read a word from input into memory
#define WRITE      11        // write a word from memory to the screen

#define LOAD       20        // load a word from memory into accumulator
#define STORE      21        // store a word from accumulator into memory

#define ADD        30        // add to accumulator the value from memory
#define SUBTRACT   31        // subtract from accum the value in memory

#define BRANCHNEG  40        // branch if accumulator is negative to location
#define HALT       43        // stop executing
```


The last two digits of the instruction specify the _operand_. This is the location of the memory to which the operation applies.  


## Example 1.

An instruction "1012" means an operation of "10" and an operand of "12". So this means read input from the user and store it in location 12.


## Example 2

Consider the following U program. It is a series of 4 digit instructions. The instructions are stored in memory location 00 to 06. Ultron always starts executing its programs at memory location 0.

      


<table>
  <tr>
   <td>Memory location
   </td>
   <td>Word
   </td>
   <td>Instruction
   </td>
  </tr>
  <tr>
   <td>0
   </td>
   <td>1010
   </td>
   <td>Read from input into variable A (location 10)
   </td>
  </tr>
  <tr>
   <td>1
   </td>
   <td>1011
   </td>
   <td>Read from input into variable B (location 11)
   </td>
  </tr>
  <tr>
   <td>2
   </td>
   <td>2010
   </td>
   <td>Load variable A into accumulator
   </td>
  </tr>
  <tr>
   <td>3
   </td>
   <td>3011
   </td>
   <td>Add variable B into accumulator
   </td>
  </tr>
  <tr>
   <td>4
   </td>
   <td>2112
   </td>
   <td>Store accumulator into variable C (location 12)
   </td>
  </tr>
  <tr>
   <td>5
   </td>
   <td>1112
   </td>
   <td>Print variable C (location 12)
   </td>
  </tr>
  <tr>
   <td>6
   </td>
   <td>4300
   </td>
   <td>Halt and end program
   </td>
  </tr>
</table>


The program reads variables A and B, adds them together and stores the result in variable C, then prints the result C to the screen. 



* When reading input, U prompts the user with a **<code>"? "</code></strong>. 
* When printing the result, U shows <strong><code>"= "</code></strong> and then the value.  

Running the U program with inputs <strong>42</strong> and <strong>55</strong> would therefore produce the following output:


```
? 42
? 55
= 97
```

## Example 3

Here is another example program. This program compares two values and prints the value that is larger.


<table>
  <tr>
   <td>Memory location
   </td>
   <td>Word
   </td>
   <td>Instruction
   </td>
  </tr>
  <tr>
   <td>0
   </td>
   <td>1009
   </td>
   <td>Read from input into variable A (location 09)
   </td>
  </tr>
  <tr>
   <td>1
   </td>
   <td>1010
   </td>
   <td>Read from input into variable B (location 10)
   </td>
  </tr>
  <tr>
   <td>2
   </td>
   <td>2009
   </td>
   <td>Load variable A into accumulator
   </td>
  </tr>
  <tr>
   <td>3
   </td>
   <td>3110
   </td>
   <td>Subtract variable B from accumulator
   </td>
  </tr>
  <tr>
   <td>4
   </td>
   <td>4007
   </td>
   <td>Branch to 07 if negative
   </td>
  </tr>
  <tr>
   <td>5
   </td>
   <td>1109
   </td>
   <td>Print variable A (location 09)
   </td>
  </tr>
  <tr>
   <td>6
   </td>
   <td>4300
   </td>
   <td>Halt and end program
   </td>
  </tr>
  <tr>
   <td>7
   </td>
   <td>1110
   </td>
   <td>Print variable B (location 10)
   </td>
  </tr>
  <tr>
   <td>8
   </td>
   <td>4300
   </td>
   <td>Halt and end program
   </td>
  </tr>
  <tr>
   <td>9
   </td>
   <td>0000
   </td>
   <td>Variable A
   </td>
  </tr>
  <tr>
   <td>10
   </td>
   <td>0000
   </td>
   <td>Variable B
   </td>
  </tr>
</table>



# Your Task

Your task is to write Ultron. The header file is in **ultron.h** and the implementation in **ultron.c**. The methods you need to write are listed below.


## Abstract Data Type

Ultron is represented as an abstract data type Ultron defined in ultron.h.

```
	typedef struct {
		// Add your definitions here
	} Ultron;
```



## Constructor

The Ultron simulator is created by initializing a struct of class Ultron.



* `void ultron_init(Ultron* ultron)`
    * This function is given a pointer to an Ultron object that must be initialized.
    * The function initializes the contents of the Ultron pointer and creates a computer with 32 integers of memory.
        * An Ultron object has an integer array of size 32 that represents the memory of the computer.
        * It should clear the memory by setting all values in **memory** to 0.
        * It should initialize two integer variables to represent the program counter and accumulator register. Initialize both to 0. 


## Dumping Memory

The Ultron class provides a way for you to dump the contents of the computer. It prints out the registers and the contents of memory.



* `void ultron_dump(Ultron* ultron)`
    * Print out the contents of memory. For example, printing out the contents of an Ultron would print the following. The contents of location 0 is 1010, the contents of location 1 is 2010, and so on.
```
PC: 8
AC: -1
 0 1010
 1 2010
 2 3111
 3 4008
 4 2110
 5 1110
 6 2012
 7 4001
 8 4300
 9    0
10    0
11    1
12   -1
13    0
14    0
15    0
16    0
17    0
18    0
19    0
20    0
21    0
22    0
23    0
24    0
25    0
26    0
27    0
28    0
29    0
30    0
31    0
```

The program counter is given by PC, the accumulator by AC, and then it prints the memory locations and the associated value.


To get right adjusted output, read the [documentation on printf](https://man7.org/linux/man-pages/man3/printf.3.html). 


* `int ultron_memory(Ultron* ultron, int location)`
    * Return the content at the given memory location. This simply returns the contents of the **memory** array at index location.
* `int ultron_accumulator(Ultron* ultron)`
    * Return the value of the accumulator
* `int ultron_pc(Ultron* ultron)`
    * Return the value of the program counter 


## Loading A Program

The Ultron class provides a way for you to load a program into the simulation's memory.



* `void ultron_load(Ultron* ultron, int size, int* instructions)` 
    * Copy `size` instructions from the array of integer instructions into Ultron's memory, starting at memory location 0.
    * E.g.. If there are 20 instructions, copy all 20 instructions into Ultron's memory.


## Running A Program

After a program has been loaded into memory, we can now execute the program.



* `int ultron_run(Ultron* ultron)`
    * Runs the program, until the program is halted or encounters an error. The entry point of the program is memory location 0. Return 0 if the program halted successfully, or one of the following error codes if it encounters an error 
        * Operation is invalid: return -1
            * Your operation should only be one of the operation values listed in the constants above.
        * Operand outside the range of allowable memory: return -2.
            * Your operand should only be in the range of 0 to 31
        * Program counter outside the range of allowable memory: return -3
            * Your program counter should only be in the range of 0 to 31

The program counter tells us the location of the next instruction to perform. We fetch the instruction by using the C statement below. If Ultron's memory is represented by the array of ints **memory** and the program counter by the variable **pc**, then:

	

	`int instruction = memory[pc];        `

The operation and operand are extracted from the 4-digit instruction by

	`int operation = instruction / 100;    // gets first two digits`


```
	int operand = instruction % 100;      // gets last two digits
```


Now Ultron has to figure out what instruction to execute, and what to do for each instruction. The following table should help you determine what action to take.


<table>
  <tr>
   <td>U operation
   </td>
   <td>C equivalent
   </td>
   <td>Comment
   </td>
  </tr>
  <tr>
   <td>READ
   </td>
   <td><code>printf("? ");</code>
<p>
<code>scanf("%d", &memory[operand]);</code>
   </td>
   <td>Prompt user, read into memory location
   </td>
  </tr>
  <tr>
   <td>WRITE
   </td>
   <td><code>printf("= %d\n", memory[operand]);</code>
   </td>
   <td>Print value at memory location
   </td>
  </tr>
  <tr>
   <td>LOAD
   </td>
   <td><code>accumulator = memory[operand];</code>
   </td>
   <td>Load memory location into accumulator
   </td>
  </tr>
  <tr>
   <td>STORE
   </td>
   <td><code>memory[operand] = accumulator;</code>
   </td>
   <td>Store from accumulator to memory location
   </td>
  </tr>
  <tr>
   <td>ADD
   </td>
   <td><code>accumulator += memory[operand];</code>
   </td>
   <td>Add to accumulator from value at memory location
   </td>
  </tr>
  <tr>
   <td>SUBTRACT
   </td>
   <td><code>accumulator -= memory[operand];</code>
   </td>
   <td>Subtract value at memory location from accumulator
   </td>
  </tr>
  <tr>
   <td>BRANCHNEG
   </td>
   <td><code>if (accumulator &lt; 0) {</code>
<p>
<code>    pc = operand;</code>
<p>
<code>}</code>
   </td>
   <td>Move program counter to new location if accumulator is negative
   </td>
  </tr>
  <tr>
   <td>HALT
   </td>
   <td><code>return 0;      // Stop execution</code>
   </td>
   <td>Halt, return 0
   </td>
  </tr>
</table>


**Once you finish an instruction, you must increment the program counter by 1 to move to the next instruction to execute, except for HALT operations. **The program counter stays the same for HALT operations. For the others (BRANCHNEG is a special case that we consider below):

**	`pc += 1;`**

For the BRANCHNEG operation:



* If the accumulator is negative, you want to take the branch, so the program counter is updated to the operand.
* If the accumulator is not negative, you do NOT want to take the branch, so the program counter is just incremented normally.


## Main Program

We use the Ultron simulator in the following manner, in a main program that we write. **Write your main program in a separate file**, **<span style="text-decoration:underline;">main.c</span>**.  The program accepts these command line parameters:

	`./ultron [-i instructions] [-f filename]`

Your main program should behave like this:



* The number of instructions is specified on the command line with the `-i` flag. If it is not specified, the default is 32.
* Read a program in the specified file with the `-f` flag. If the flag is not specified, read from standard input.
* Load the program instructions into Ultron's memory
* Run the program
* Print the result of running the program (the return value from `ultron_run`).
* Dump the memory at the end.

Note that the two arguments are exclusive. Either you specify `-i`, or you specify `-f`.

For example, if the user wants to input a program with 7 instructions, it would respond as below. User input is red and black is program output. Note that while the user is only entering 7 instructions, the total memory size of Ultron is still 32 integer words.


```
    $ultron -i 7
    1010
    1011
    2010
    3011
    2112
    1112
    4300
    ? 5
    ? 9
    = 14
    Result: 0
    PC: 6
    AC: 14
     0 1010
     1 1011
     2 2010
     3 3011
     4 2112
     5 1112
     6 4300
     7    0
     8    0
     9    0
    10    5
    11    9
    12   14
    13    0
    14    0
    15    0
    16    0
    17    0
    18    0
    19    0
    20    0
    21    0
    22    0
    23    0
    24    0
    25    0
    26    0
    27    0
    28    0
    29    0
    30    0
    31    0
```



# Building, Running, Testing

Build the program with 

	**<code>make</code></strong> 

You will notice that this Makefile uses g++ to compile `test_main.cpp` instead of gcc. This is because we are using a C++ test framework to do automated testing,. Mixing C and C++ is straightforward.  C definitions are wrapped in an `extern "C"` wrapper, as you can see in test_main.cpp

Run it with

	**<code>./ultron [-i instructions] [-f filename]</code></strong>

Note that you can run ultron either with the `-i `flag, or with the `-f` flag, but not both together. `-i` should cause ultron to read instructions from the user, while `-f` causes ultron to read the program from a file.

Run the formal tests with

	**<code>make test</code></strong>


## An Ultron Program

Now write a U program in **count.u** that reads a number from the user, then prints downwards from the number - 1 to 0.  Check that it works with your Ultron. Note that you should not add any more instructions to Ultron. It's a RISC computer with a very reduced instruction set, and it is totally possible to do count down loops with just the given instruction set.

For example, when count.u is run, and if the user enters 5, it should print:


```
$ ./ultron -f count.u
? 5
= 4
= 3
= 2
= 1
= 0
```


<code>... <em>plus a result and a dump of memory</em></code>


## Example Ultron Program

An example program is given in **program.u.** You can run the program with

	`./ultron -f program.u`

The program is shown below. Comments are shown in italics.


```
    1010               Load user input into location 10
    2010               Load location 10 into accumulator
    3011               Add location 11 into accumulator
    2110               Store accumulator back into location 10
    1110               Print the contents of location 10
    4300               Halt
    0                  Content of location 6 is a constant 0
    0                  Content of location 7 is a constant 0
    0                  Content of location 8 is a constant 0
    0                  Content of location 9 is a constant 0
    0                  Content of location 10 is a constant 0
    -1                 Content of location 11 is a constant -1
```


When you run this program, it asks the user for a number. It then subtracts 1 from the number and prints the result. The output from running the program is shown below. The user input is shown in red.


```
$ ./ultron -f program.u
? 23
= 22
Result: 0
PC: 5
AC: 22
 0 1010
 1 2010
 2 3011
 3 2110
 4 1110
 5 4300
 6    0
 7    0
 8    0
 9    0
10   22
11   -1
12    0
13    0
14    0
15    0
16    0
17    0
18    0
19    0
20    0
21    0
22    0
23    0
24    0
25    0
26    0
27    0
28    0
29    0
30    0
31    0
```



# Submitting

Make sure to submit:



* ultron.h, the header
* ultron.c, the implementation
* main.c, the main program
* count.u, your Ultron counting program


# Hints



* Reading and writing from files is done with the functions `fopen(3)`, `fread(3`) and `fwrite(3). `Consult the man pages for each of these calls. They're also documented in the C Programming Language.
* `fgets(3)` is useful for reading lines.
* `getopt(3)` is useful for parsing arguments.
