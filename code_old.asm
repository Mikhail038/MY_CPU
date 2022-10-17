;video-ram circle

    vsetx 10
    vsety 15

;================================

    push 10
    dup
    pop rax

    dup
    push 2
    div
    rnd
    push 2
    mul

    pop rax
    je label3

    dup
    push rax
    ja label1

    pop
    pop
    push 2
    div
    rnd

label1:
    pop rax ; x0

;================================

    push 15
    dup
    pop rbx

    dup
    push 2
    div
    rnd
    push 2
    mul

    pop rbx
    je label3

    dup
    push rbx
    ja label2

    pop
    pop
    push 2
    div
    rnd

label2:
    pop rbx ; y0

;================================

    push rax
    push rbx
    out
    out
    hlt

    push 100
    pop rdx

    push 1
    pop rcx

    push 2
    pop [0]

    call foo


    updscr
    hlt

foo:
    pop rcx
    push rdx
    ret

label3:
    push 1
    add
    push 2
    div
    rnd

    ret
