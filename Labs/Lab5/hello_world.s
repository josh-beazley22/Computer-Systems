          .global    main

          .text
main:     mov       $1, %rax                # system call for write
          mov       $1, %rdi                # file handle 1 is stdout
          mov       $message, %rsi          # address of string to output
          mov       $13, %rdx               # number of bytes
          syscall                           # invoke operating system to do the write
          mov       $60, %rax               # system call for exit
          mov       $0, %rdi                # exit code 0
          syscall                           # invoke operating system to exit

          .data
message:  .ascii    "Hello, World\n"        # note the newline at the end
