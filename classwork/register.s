            .global main

            .text
main:       mov         $10, %r8 # int i = 0;

top:        cmp         $0, %r8  # compare register 8 to 0
            je          end     # if true, jump to end

            # write(1, message, 13)
            mov         $1, %rax    # 1 is the write command
            mov         $1, %rdi    # arg1 is 1
            mov         $message, %rsi # arg2 is message
            mov         $13, %rdx   # arg3 is count
            syscall

            dec         %r8     # i--
            jmp        top      # jump to top

end:
            mov     $0, %rax    # return 0
            ret

            .data
message:    .ascii      "Bacon, Egg, & Cheese\n"