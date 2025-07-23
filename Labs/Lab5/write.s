        .global main

        .text
main:   
        # Write input_message
        mov     $1, %rax
        mov     $1, %rdi
        mov     $input_message, %rsi
        mov     $24, %rdx
        syscall
        # Read user input
        mov     $0, %rax
        mov     $0, %rdi
        mov     $buffer, %rsi
        mov     $100, %rdx
        syscall
        # Write output_message
        mov     $1, %rax
        mov     $1, %rdi
        mov     $output_message, %rsi
        mov     $18, %rdx
        syscall
        # Write user input which was stored into %r8
        mov     $1, %rax
        mov     $1, %rdi
        mov     $buffer, %rsi
        mov     $100, %rdx
        syscall
        # Close program.
        mov     $60, %rax
        mov     $0, %rdi
        syscall

        .data
buffer:         .space 100
input_message:      .ascii  "Please enter a message: "
output_message:     .ascii  "You have entered: "
