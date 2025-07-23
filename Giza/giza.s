# gcc -no-pie -z noexecstack giza.s && ./a.out 

        .global     main

        .text
main:
    mov         8(%rsi), %rdi   # move argv[1] into rdi
    call        atoi            # call into C to convert to int
                                    # the result will be available in %eax

    mov     %eax, %r9d        # n=10
    mov     $0, %r10d        # i=0
outer_loop:
    cmp     %r10d, %r9d
    jng     outer_end

    mov     $0, %r8d        # k=0
inner_loop:
    cmp     %r8d, %r10d
    jl     inner_end

    # print(*)
    mov     $1, %rax                # system call 1 is write
    mov     $1, %rdi                # file handle 1 is stdout
    mov     $star, %rsi          # address of string to output
    mov     $1, %rdx               # number of bytes
    syscall                         # invoke operating system 
    inc     %r8d
    jmp     inner_loop
inner_end:


    # print(\n)
    mov     $1, %rax                # system call 1 is write
    mov     $1, %rdi                # file handle 1 is stdout
    mov     $newline, %rsi          # address of string to output
    mov     $1, %rdx               # number of bytes
    syscall                         # invoke operating system 
    inc     %r10d
    jmp     outer_loop
outer_end:

    # write Finito
    mov     $1, %rax
    mov     $1, %rdi 
    mov     $finish, %rsi 
    mov     $7, %rdx 
    syscall

    mov     $0, %eax
    ret



    .data
star:       .ascii      "*"
newline:    .ascii      "\n"
finish:     .ascii      "Finito\n"
