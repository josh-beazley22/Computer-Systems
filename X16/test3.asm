start:
    add %r0, %r5, %r7
    add %r2, %r3, $10
    add %r0, %r5, %r7
    add %r2, %r3, $10
    brn LABEL
    brp LABEL
    brz LABEL
    brzp LABEL
    brnp LABEL
    brnz LABEL
    brnzp LABEL
    br  LABEL

LABEL:
    jmp %r6

    jsr LABEL
    jsrr %r4

    ld %r2, morelabel
    ldi %r1, morelabel
    ldr %r1, %r4, $5

    lea %r3, morelabel

    not %r2, %r4

    st %r0, LABEL
    sti %r1, morelabel
    str %r2, %r1, $6

    getc
    putc
    puts
    enter
    putsp
    halt



morelabel:
    val  $16