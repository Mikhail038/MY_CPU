;ring vram

;=========================
    inp
    pop [3] ; a

    inp
    push 4
    div ;!!!!!!!!!!!
    pop [4] ; b

    push 35
    pop [5] ; symbol

    vsetx 160
    vsety 80

    push 1
    pop [6] ; marker

    push 0
    pop [8] ; t1 (sin t1)

    push 0
    pop [9] ; t2 (sin t2)
    dump
;=========================

    push 160 ; !!!!!!!!!!!!!!
    pop rax

    push 80 ; !!!!!!!!!!!!!!!!
    pop rbx

    push 100
    pop rcx

    push rax
    push rbx
    mul

    push 11 ; !!! == dop_start + 1
    sub

    push rcx
    add

    pop [2]

;=========================

bgn:
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
    clrscr
    updscr

    push [6]
    push 1
    jne anth

;--------------------------

    push [6]
    pop [7]

    push [3]
    push 1.2
    mul

    dup
    push 3000
    ja buf1
    call chng
buf1:

 ;   dup
  ;  push 200
   ; jb buf2
   ; call chng
 ; buf2:

    pop [3]


    push [4]
    push 1.2
    mul

    dup
    push 1500
    ja buf3
    call chng
buf3:

 ;   dup
  ;  push 100
   ; jb buf4
  ;  call chng
 ; buf4:

    pop [4]

    jump label4

anth:

    push [6]
    pop [7]


    push [3]
    push 1.6
    div


 ;   dup
  ;  push 4000
  ;  ja buf5
  ;  call chng
 ; buf5:

    dup
    push 100
    jb buf6
    call chng
buf6:

    pop [3]

    push [4]
    push 1.5
    div

  ;  dup
  ;  push 2000
  ;  ja  buf7
  ;  call chng
 ; buf7:

    dup
    push 50
    jb buf8
    call chng
buf8:

    pop [4]


label4:

    jump bgn

    hlt

;===================================

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

    pop [0] ; delta x 2

;
    call if
    push 1
    ja else1
    call pnt
    jump else2

else1:
;
    push 0
    pop [rdx]

else2:
    push 1
    add
;
    pop rdx
    ret

;=======================

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

    pop [1] ; delta y 2
;

    push rax
    mul
    sub

    ret
;==============================================================

if:
    push [0]

    dup
    push 0
    jne l1 ;!!

    push -1
    mul
l1:
    push [3]
    div

;---------------------------------------

    push [1]

    dup
    push 0
    jne l2 ; !!

    push -1
    mul
l2:
    push [4]


    div

    add

    push 1
    jae da
    push 0
    ret

;----------------------------

da:

        push [0]

        dup
        push 0
        jne l3 ;!!

        push -1
        mul
    l3:
        push [3]
        push 2
        div

        div

    ;---------------------------------------

        push [1]

        dup
        push 0
        jne l4 ; !!

        push -1
        mul
    l4:
        push [4]
        push 2
        div

        div

        add

        push 1
        jb daa
        push 0
        ret

;---------------------------

daa:
    push 1
    ret

pnt:
    push [5]
    pop [rdx]
    ret

;=============================

chng:
    push [6]
    push [7]
    jne rtrn

    push [6]
    push 1
    je chng1

    push 1
    pop [6]

    ret

chng1:

    push 0
    pop [6]

    ret

rtrn:
    ret
