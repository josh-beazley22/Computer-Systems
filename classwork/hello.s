        .global main

        .text
main:
        mov     $1, %rax  #system call for write

        # write(1, message, 13)
        mov     $1, %rdi        # arg 1 is stdout
        mov     $message, %rsi  # arg 2 is message
        mov     $30, %rdx       # arg 3 is 13 bytes
        syscall     # bam! into system kernel.

        # exit(0)
        mov     $60, %rax       # 60 is exit
        mov     $0, %rdi        # exit code is 0
        syscall

        .data
message:    .ascii  "Eat my Shorts! --Bart Simpson\n"