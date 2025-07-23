[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/z2J4TAK_)
# Assembling Giza and Words with GAS
<!--- Access your assignment [here](https://docs.google.com/document/d/1zCFxYM60W2IElCAWJOEyRvN2LoQZuyxSbmpXm7feQfk/edit?usp=sharing). --->
![Giza](images/giza.png)

In this assignment, we are going to write two programs using x86-64 assembler instructions. Like the ancient Egyptians, we are going to assemble a (text) pyramid out of some very basic building blocks (of assembler code), and then we will count some words in a file. We'll gain a fuller appreciation of why most programmers prefer to work in higher level languages like C.


# GNU Assembler (GAS)

There are several assembly language compilers in use today. There is the [Netwide Assembler](https://www.nasm.us/docs.php), a platform independent assembler for the x86 platform that runs on many different platforms, including both 32 bit and 64 bit, as well as Windows and MacOS and Linux. That's the most popular assembler for Linux.

However, to make sure that our code will compile and run the same for everyone, we're going to assume and require a **Linux 64 bit platform, and use GNU Linux tools**. To that end, you may use your own 64-bit Linux, the lab 64-bit Linux machines, your Windows machine running a 64-bit WSL, or your Mac running a 64-bit Linux VM. Mac users can't do the lab natively – the best would be to ssh into the lab machines.

The GNU assembler can simply be called with gcc. We add the `-no-pie` and `-z noexecstack` arguments to simplify things[^1].


# The Giza Program

Your program is named <strong><code>giza.s</code></strong> and will compile to a program named <strong><code>giza</code></strong>.  Your program basically takes a single command line argument, a number of lines of the pyramid to print. For example, if the command line argument is 5, then the output is:


```
    $ ./giza 5
    *
    **
    ***
    ****
    *****
    Finito
```


The program outputs that number of stars in a pyramid, and also `Finito` at the end.  

If the command line argument is 1, then the output is


```
    $ ./giza 1
    *
    Finito
```


Your program can assume that there will be a number in the command line arguments (you don't have to check for insufficient, too many, or incorrect arguments). There is only ever one number in the command line, and that's how many rows of the pyramid you need to print.


## Compiling with the C Standard Library

Since we are going to call C functions from assembly, your program should start with the entry point as main. You specify it this way. The first directive indicates that your program starts at main, and the .text section comes next. 


```
            .global      main

            .text
    main:    # your program starts here
```



## Defining Data

Your program can reserve space for string data with the `.ascii` directive. Do this at the end of your program for any strings or data you want to use.

For example:


```
    hello_message:	.ascii  	"Hello, world"
```


This stores the string in memory. Note that it is not null terminated. If you want a null terminated string, use `.asciz`
```
	hello_terminated:	.asciz	"Hello, world\n"
```
This one has a newline and is also null terminated.

You can now refer to these data addresses with `$hello_message` and `$hello_terminated` in your program.


## C Function Calling Conventions

When you call functions in C, registers hold the first few arguments. 



* `%rdi` holds the first arg 
* `%rsi` holds the second arg 
* `%rdx` holds the third arg

When you have too many arguments, the rest go on the stack. This is a good reason to try and hold the number of arguments down in the functions you call. Register arguments are faster than arguments on the stack in memory.  The return value is in %eax for 32 bits or %rax for 64 bits.

Functions may modify the stack and change register values. The usual pattern to calling functions is shown below. The detailed C calling conventions are as follows:

Caller:



* Registers %r10-%r11 and any registers that parameters are put into should be saved by pushing on the stack
    * E.g. `push %r10   # save r10`
* Pass parameters by putting arg1 into %rdi, arg2 into %rsi, etc.
* Call the function
* Return value is expected in %rax
* Restore any caller saved registers by popping off the stack in the reverse order that they were pushed on

Callee:



* Allocate any local variables by subtracting the stack pointer
    * E.g.` sub $4, %rsp   # alloc an int` 
* %rbx, %rbp, %r12 through %r15 must be saved by the callee if they are going to be used
* Now proceed with actual function code
* Restore any values of callee saved registers if used by popping off the stack in the reverse order that they were pushed
* Deallocate local variables by increasing stack by same amount we subtracted earlier
* Put return value in %rax
* Return to caller

Suppose you're using %r11d to hold some data, and you want to call the function foo. You first push r11d on the stack, then move any arguments that the function needs into the %rdi, %rsi or %rdx registers, then call the function. After the function returns, the return values are available in %rax (or %eax for 32 bit values).

	


```
    push 	%r11d           # save r11d because you're using it                 

    mov         $123, %rdi      # move arg1 into rdi
    call        foo             # call function named foo

    pop		$r13d 	    # restore the register you were using


    # the result will be available in %eax since it's a 32 bit value
```


For the function foo:


```
foo:
	sub		$4, %rsp		# allocate an int as a local variable
	push		%r13			# save r13 as we will use it


	# proceed with actual function code
	mov		$5, %r13
	add		%rdi, %r13
	mov		%r13, %(rsp)      # calculate some value in local variable

	pop		%r13			# restore register r13 to original value
	mov		%(rsp), %rax	# put return value in rax
	ret
```

#### Important: Align Stack before Calling Function
The x86 architecture requires that the stack be aligned on a 16 bit value, ie. the stack value must be a multiple of 16.
Your stack may or may not be aligned.
If you find that you're seg faulting on calling a function, you must align the stack.
The easiest way is to push a value onto the stack, call the function, then pop 
a value to restore the stack. 

For example:
```
	push 	%r8		# just push any register
	call 	printf		# call C library function
	pop	%r8		# restore old value by popping off
```
The push instruction makes sure that the stack is aligned properly.

## Handling Command Line Arguments

Your GAS program has to handle command line arguments. In C, your program has an entry point that looks like this:
```
	int main(int argc, char* argv[])
```
When the shell executes your program, it assumes that you will accept argc and argv in the same format. The convention is that:

* `argc` ends up in `rdi (the first arg is always here)`
* `argv` ends up in `rsi (the second arg is always here)`

Since we are using a 64 bit platform, pointers are 8 bytes long. Therefore the i<sup>th</sup> element of argv is `argv + 8 * i`. That is, `argv` is the C `argv[0]`, and `argv + 8` is the C `argv[1]`, and so on.

`argv[0]` is `(%rsi)` in GAS notation. `argv[1]` is available in GAS notation as `8(%rsi)` when main is first called.


## Converting Strings to Ints

Since the first arg is the number of lines to print, you have to convert it to an integer. You can call the C function <code>[atoi](https://www.tutorialspoint.com/c_standard_library/c_function_atoi.htm)</code> in order to do the conversion (or you can write the equivalent of atoi in assembler). Similar to the way your main function is called, <code>rdi</code> should hold the pointer to the string you want to convert when you call <code>atoi</code>.


```
mov         8(%rsi), %rdi   # move argv[1] into rdi
call        atoi            # call into C to convert to int
                            # the result will be available in %eax
```



## Writing Output

### write

You can use a syscall to [write](https://man7.org/linux/man-pages/man2/write.2.html) your output, or you can use printf. 

When making the write syscall:
* `rdi` holds the first arg (the file descriptor -- use 1 for stdout)
* `rsi` holds the second arg (the pointer to the string)
* `rdx` holds the third arg (the number of bytes to write)
* The register `rax` holds the system call id. For `write`, that value is 1.

### printf
To call `printf` from assembly language, you just pass the format string in rdi as usual for the first argument,
pass any format specifier arguments in the next argument register rsi, then rdx, etc.  There is one surprise: you need to zero out the `al` register (the low 8 bits of eax/rax), 
or else printf will assume you're passing it vector registers and crash.
```
mov $formatStr, %rdi  # first argument: format string
mov $5, %rsi          # second argument: integer to print
mov $0, %al           # magic for varargs (0==no magic, 
                      #                    to prevent a crash!)

call printf
```

The format string is stored in the `.data` section.
```
		.data
formatStr:		.asciz "The int is %d\n"
```
The C standard library buffers output until there is a newline. If you try and print something without a newline, it won't show up until there is one. 



## Comparing Values

`cmp` is the compare instruction that compares two operands. The result is stored in the accumulator. Based on the compare, you can use one of the jump commands to branch to a label.

`je` lumps to a label if the compare was equal

Also:
* `jne` - jump not equal
* `jl` - jump less
* `jnl` - jump not less
* `jg` - jump greater
* `jng` - jump not greater
* `jmp` - always jump

Remember that `cmp a, b` computes `b-a`. For example, consider:
```
cmp %r1, %r2
jl  location
```
The jump will occur if `%r2` is less than `%r1`.

## Exiting Cleanly

Your program should exit cleanly by returning 0 from main.
```
	mov 		$0, %eax    # put the return value in %eax
	ret                         # return from main
```


Don't exit with anything other than a 0, or the autograder will fail.


## An Example Program

The best way to start is with an example program. Think about how you would modify this program to get Giza.



* Start with printing a single *. 
* Then try looping and printing some fixed number of stars.
* Then convert the command line argument into an int.
* Try looping and printing a variable number of stars given the command line argument.
* Then write the final program.
* Hint:
    * This is best realized as two loops, an outer and inner loop.
    * You'll probably need to use instructions like `inc` and `dec`, and `cmp` and `jmp`.


```
# --------------------------------------------------------------
# Writes "Hello, World" to the console using only system calls. 
# Runs on 64-bit Linux only.
#
# To run
#     gcc -no-pie hello.s && ./a.out
# -----------------------------------------------------------

        	.global main

        	.text
main:
        # write(1, message, 13)
        mov     $1, %rax                # system call 1 is write
        mov     $1, %rdi                # file handle 1 is stdout
        mov     $message, %rsi          # address of string to output
        mov     $13, %rdx               # number of bytes
        syscall                         # invoke operating system 

        # exit(0)
        mov     $0, %eax               # return 0
        ret

	# char* message = "Hello, world\n"
message: 	.ascii  "Hello, world\n"
```



## Compiling and Testing

Write your code in <strong><code>giza.s</code></strong>.  You can compile your code with the provided Makefile. To build giza:
```
	make giza
```
This builds `giza.s` with gcc, and links it with the C libraries. A gcc flag `-no-pie` is given to prevent gcc from linking with position independent code (which we will learn about in Homework 8). The make program will produce an executable called <strong><code>giza</code></strong>.

Test your code by running it with different values, e.g. `"./giza 7"`, `"./giza 42"`


# The WC Program

The Unix [wc(1)](https://linux.die.net/man/1/wc) program counts the number of words in a file. The program you will write is similar, and also called **wc** and you will write it in **wc.s.** Our program though takes no arguments. It simply reads from standard input until it sees the end of the file. It will count the number of words in the input stream, and then print "Result=_xxx_" where _xxx_ is the number of words that it counted. For example, you can count the number of words in a file using input redirection like this:

	`./wc &lt; hamlet.txt`

The command above should output the number of words in the hamlet.txt file. If the file contains
```
	To be or not to be
	That is the question
```


The output would be "Result=10".

A word is defined as consecutive non-whitespace characters. A whitespace character is either a space (ASCII code 32), a newline (ASCII code 10), carriage return (ASCII code 13) or a tab (ASCII code 9).


## Starter Code

I've given you some starter code in wc.s to help kick things off.  The first piece you're given is the **print_result** function. This function uses [printf(3)](https://man7.org/linux/man-pages/man3/printf.3.html) to output the answer in the format required.


```
# The print_result function takes one arg in %edi, and prints the
# result as "Result=xxx" on standard output using printf.
print_result:
        mov     %edi, %esi            # arg2 is the number
        mov     $format, %rdi         # arg1 is the format string
	mov	$0, %al		      # magic value for varargs
        call    printf

        ret

format:
        .asciz  "Result=%d\n"
```


The second function I've given you is the **skipws** function. This function reads from standard input and skips over whitespace until it reads a non-whitespace character, or reaches the end of file. If it reaches EOF, it returns -1, otherwise it returns 0.  If it returns 0, that means you have seen the start of a new word. 


```
# The skipws function reads from standard input, until it sees a non
# space character or end of file. It returns -1 on EOF or 0 otherwise.
skipws:

skipws_top:
        # read a character, the result is in %eax
	call	getchar

        # check if getchar returned EOF
    	cmp    	$-1, %eax
        jne	skipws_checkspace

        # We got an EOF, so return -1 in %eax
        mov	$-1, %eax
        ret

skipws_checkspace:
        # Check whether the character we read was a space
        movl    %eax, %edi    # the char read was in %eax
        Call    isspace

        cmp     $1, %eax
        je	skipws_top      # was a space, so we keep looping

        mov 	$0, %eax      # not a space, so we return from the function
	ret
```

The `skipws` function uses the function `isspace` which takes an int as an argument and returns 1 if it is a space, or 0 if it is not a space.


## Your Task

Your task is to write:



1. <strong><code>isspace</code></strong>
2. <strong><code>readword</code></strong>
3. The <strong><code>main</code></strong> program.

<strong><code>isspace</code></strong> is used to determine if a character is a space.


```
# The isspace function takes one arg in %edi, and returns 1 in %eax if
# if the arg is a space, or 0 in %eax if the arg is not
isspace:
      # arg1 in %edi, compare it to space (32), newline (10),
      # carriage return (13) and tab (9).
      # TODO: Your code here
	ret
```


The next function to write is <strong><code>readword</code></strong>. This function is very similar to <strong><code>skipws</code></strong>, except that it reads characters from standard input and continues until it sees a whitespace character. It basically functions as the inverse of skipws, and you should be able to just modify the code from skipws slightly.


```
# The readword function reads from standard input, until it sees a white
# space character or end of file. It returns -1 on EOF or 0 otherwise.
readword:
        # TODO: Your code here
        ret
```


The last thing to do is to use <strong><code>skipws</code></strong>, <strong><code>readword</code></strong> and <strong><code>print_result</code></strong> to write the main program.


## Reading Input

In order to read standard input, call the [getchar(3)](https://man7.org/linux/man-pages/man3/getchar.3p.html) function provided by the C standard library. You can see this done in `skipws`.


## Compiling and Testing

Write your code in  **wc.s**.  You can compile your code with the provided Makefile. To build wc:


```
	make wc
```


Test your code by running it and [redirecting](https://www.tutorialspoint.com/unix/unix-io-redirections.htm) different files, e.g. `"./wc < giza.s".`
If you're typing your input in interactively, generate an EOF (end of file) by typing Control-D.

You can debug the `wc` executable the same way you normally do with gdb.
```
gdb wc
```
To run the file, use
```
(gdb) run < foo.txt
```
This will cause your program to be run within gdb while taking input from foo.txt.
If your program seg faults:
- Get a stack backtrace with `bt`
- Look at the contents of registers with `info registers`


# Hints
If you find yourself seg faulting in calling `printf` or `getchar`, it could be because the Linux API requires that just before a CALL that the stack be at least 16-byte (or 32-byte) aligned. After the C runtime calls your main function the stack is misaligned by 8 because the return pointer was placed on the stack by CALL. To realign to a 16-byte boundary before a CALL, you can simply PUSH any general purpose register onto the stack and POP it off at the end. For example,
```
	push 	%r8		# just push any register
	call 	printf		# call C library function
	pop	%r8		# restore old value by popping off
```


<!-- Footnotes themselves at the bottom. -->
## Notes

[^1]:
     The `-no-pie` flag is telling gcc not to make a [position independent executable](https://access.redhat.com/blogs/766093/posts/1975793) (PIE). We'll learn about linking and loading later in the quarter. The `-z noexecstack` flag tells the compiler not to make an executable stack. It's necessary because we are taking some shortcuts in our assembler, to make it simpler.
