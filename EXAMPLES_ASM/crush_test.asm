pop
bax:
push 3
pop rax
push rax
push 5
pop [rcx]
dup
dup
add
push 1
call juju
dump
end:
dump
hlt


juju:
    push 1
    add

    dup
    push 5
    je end

    dump
    call juju
    ret
