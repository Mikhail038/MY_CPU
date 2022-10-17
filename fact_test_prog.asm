;factorial!

    push 1
    inp
    rnd

    call step
    jump end

step:
    dup
    push 1
    jae rtrn

    dup
    push 1
    sub
    call step

rtrn:
    mul
    ret

end:
    out
    hlt

