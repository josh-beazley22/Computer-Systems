        .global main

        .text
main:
        # Write input_message
        mov     $1, %rax
        mov     $1, %rdi
        mov     $input_message, %rsi
        mov     $24, %rdx
        syscall

        # Read first digit
        mov     $0, %rax            # sys_read
        mov     $0, %rdi            # stdin
        lea     buffer(%rip), %rsi  # buffer to store input
        mov     $2, %rdx            # read 2 bytes (digit + newline)
        syscall

        # Load first digit
        movzbq  buffer(%rip), %r8   # first character into r8
        sub     $'0', %r8b          # convert ASCII to number

        # Write input_message
        mov     $1, %rax
        mov     $1, %rdi
        mov     $input_message, %rsi
        mov     $24, %rdx
        syscall

        # Read second digit
        mov     $0, %rax            # sys_read
        mov     $0, %rdi            # stdin
        lea     buffer(%rip), %rsi  # buffer to store input
        mov     $2, %rdx            # read 2 bytes (digit + newline)
        syscall

        # Load second digit
        movzbq  buffer(%rip), %r9   # first character into r8
        sub     $'0', %r9b          # convert ASCII to number

        # Add two numbers together
        mov     %r8b, %al
        add     %r9b, %al
        # convert to ASCII
        add     $'0', %al
        # store into buffer
        mov     %al, result(%rip)

        # Write output_message
        mov     $1, %rax
        mov     $1, %rdi
        mov     $output_message, %rsi
        mov     $14, %rdx
        syscall

        # Write the sum.
        mov     $1, %rax            # sys_write
        mov     $1, %rdi            # stdout
        lea     result(%rip), %rsi
        mov     $1, %rdx            # one character
        syscall

        # Write newline
        mov     $1, %rax            # sys_write
        mov     $1, %rdi            # stdout
        lea     newline(%rip), %rsi
        mov     $1, %rdx            # one character
        syscall

        # Close program.
        mov     $60, %rax
        mov     $0, %rdi
        syscall

        .data
buffer:             .space  2
input_message:      .asciz  "Please enter a number: "
output_message:     .asciz  "The sum is: "
result:
        .byte 0                    # where we store result character
newline:
        .ascii "\n"
