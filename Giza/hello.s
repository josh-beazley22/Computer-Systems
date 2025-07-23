        # gcc -no-pie -z noexecstack giza.s && ./a.out 
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
message: 
  .ascii  "Hello, world\n"


@     mov $formatStr, %rdi  # first argument: format string
@     mov $5, %rsi          # second argument: integer to print
@     mov $0, %al           # magic for varargs (0==no magic, 
@                         #                    to prevent a crash!)

@     call printf

@     mov $0, %eax
@     ret
@       .data
@ formatStr:		.asciz "The int is %d\n"


    mov     $0, %r11        # k=0
inner_loop:
    cmp     %r11, %r9
    jng     inner_end

    # print(*)
    mov     $1, %rax                # system call 1 is write
    mov     $1, %rdi                # file handle 1 is stdout
    mov     $star, %rsi          # address of string to output
    mov     $1, %rdx               # number of bytes
    syscall                         # invoke operating system 
    inc     %r11
    jmp     inner_loop
inner_end: