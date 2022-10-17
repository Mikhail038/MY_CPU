;new circle vram

;=========================
    inp
    pop [3]

    push 35
    pop [4]

    vsetx 100
    vsety 100

;=========================

    push 100
    pop rax

    push 100
    pop rbx

    push 100
    pop rcx

    push 0
    pop rdx

    push rax
    push rbx
    mul

    push 11 ; !!! == dop_start + 1
    sub

    push rcx
    add

    pop [2]
;=========================

    ;push rcx
    push 100
    pop rdx

strt:
    call foo

    push rdx
    dup
    push [2]
    ja strt

;=========================

end:
    updscr
    hlt


foo:
    push rdx
    ;123
    dup
    push rcx
    sub
    ;23

    dup
    push rax

    call Mod

    push rax
    push 2
    div

    sub

    dup
    mul

    pop [0] ; delta x

;
    call if
    push 1
    ja else1
    call pnt

else1:
;

    push 1
    add
;
    pop rdx
    ret

Mod: ; x x y

    div
    floor

;
    dup
    push rbx
    push 2
    div
    sub

    dup
    mul

    pop [1] ; delta y
;

    push rax
    mul
    sub

    ret

if:
    push [0]
    push [1]
    add

    push [3]
    dup
    mul

    jae da
    push 0
    ret
da:
    push 1
    ret

pnt:
    push rdx
    push [4]
    pop [rdx]
    pop
    ret

