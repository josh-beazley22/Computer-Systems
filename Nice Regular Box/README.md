[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/NmMtbefx)
# Nice Regular Box in C

Write a program **nice_regular_box.c**. 

Your program will read in a number (no need to prompt the user), and then reads in that number of lines from the terminal.  Then the program should print an list of strings formatted in a nice regular box.

## Example 1
If the user inputs this:
```
5
Grim visaged war has smooth'd his wrinkled front
And now, instead of mounting barded steeds
To fright the souls of fearful adversaries
He capers nimbly in a lady's chamber
To the lascivious pleasing of a lute
 ```

The program should print:
```
++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Grim visaged war has smooth'd his wrinkled front +
+ And now, instead of mounting barded steeds       +
+ To fright the souls of fearful adversaries       +
+ He capers nimbly in a lady's chamber             +
+ To the lascivious pleasing of a lute             +
++++++++++++++++++++++++++++++++++++++++++++++++++++
 ```
 
 ## Example 2
 If the user inputs:
 ```
 2
 
 Avengers, assemble!
 ```
 
Note the empty line between `2` and `Avengers, assemble`. The program should print:
 ```
 +++++++++++++++++++++++
 +                     +
 + Avengers, assemble! +
 +++++++++++++++++++++++
 ```
 
 ## Example 3
 If the user inputs:
 ```
 0
 ```
 
 The program should print:
 ```
 ++++
 ++++
 ```
 
# Building Your Program
The starter code here includes a Makefile. You can build and run your program with it. Type `make` to build and `make run` to build and run.
The autograder uses the Makefile to build and run your code as well.

 # Downloading ("cloning") the code
 You should download the code in this assignment to your computer in order to copy the test code to your computer.
 Otherwise you won't have the aforementioned test scripts you can run.
 
 ## Method 1
 The simplest way is to click on the *Code* button (the one with the downward arrow), download the zip file, then unzip on your computer.
 ![image](https://user-images.githubusercontent.com/15932762/123204440-f7918e00-d46c-11eb-88bc-ad87d2d99f66.png)

 To unzip a file on Linux, the command is
 ```
 unzip filename-you-want-to-unzip.zip
 ```
 
 On WSL on Windows, you may need to install unzip with
 ```
 sudo apt install zip unzip
 ```
 
 MacOS already has unzip installed.
 
 ## Method 2
 Method 2 uses git. 
 
 WSL users will need to install git with
 ```
 sudo apt install git
 ```
 
 MacOS users will need to install git with
 ```
 brew install git
 ```
 
 Then click on the *Code* button, and copy the URL you're presented with under "Clone". The URL should look like
 `https://github.com/...`.
 Type this command:
 ```
 git clone the-url-you-copied.git
 ```

# Testing
There is a `Makefile` in the skeleton code. It is used to build and test your program. You will learn how to create and use Makefiles later in the class.

There are 5 test cases, and you can test them one at a time by entering these commands:
```
make test-1
make test-2
make test-3
make test-4
make test-5
```

You can run all the tests with this command:
```
make test
```

# Submitting
Submit your **nice_regular_box.c** file. Put your **nice_regular_box.c** in the root directory of this repo, next to README.md.

# Autograding
The autograder will run against your submission and check your output against the expected output. 

Your output must be exact.

# Helpful Hints

## Reading lines
Use the C library function `fgets` to read a line. Type `man fgets` in the terminal to read about the function and see how it is supposed to work.

## Reading line after reading an int
The program requires you to read an int, then read a line after that.

You can read an int with 
```
int num;
scanf("%d", &num);
```

If you then try to read a line with either `fgets` or `getline` directly after reading an int,
you'll find that the calls will return an empty line.
When the user types a number, e.g. "42" and then hits Enter, the characters in the input stream
are a '4', '2' and the Enter key registers as a newline '\n'.
The `scanf` function scans until it sees a non digit, and consumes the digits to place in the `num` variable.
It leaves the newline in the input stream.
Hence the next call to read a line will read an empty line.

The solution to this is to call `getchar` and have it consume the newline.
```
int num;
scanf("%d", &num);
getchar();         // consume the newline
```
