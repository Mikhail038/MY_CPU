;ring vram

;=========================
  ;  inp !!!!!!!!!!!!!!!
  push 2000
    dup
    pop [9]
    pop [3] ; a

  ;  inp !!!!!!!!!!!!!!!
  push 2000
    push 4
    div ;!!!!!!!!!!!
    dup
    pop [8]
    pop [4] ; b

    push 35
    pop [5] ; symbol

    vsetx 160
    vsety 80

    push 1
    pop [6] ; marker

    push 0
    pop [10] ; t1

    push 0
    pop [11] ; t2
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
    push 4000
    ja buf1
    call chng
buf1:

 ;   dup
  ;  push 200
   ; jb buf2
   ; call chng
 ; buf2:

  ;  dup
    pop [3]
 ;   pop [10]


    push [4]
    push 1.2
    mul

    dup
    push 2000
    ja buf3
    call chng
buf3:

 ;   dup
  ;  push 100
   ; jb buf4
  ;  call chng
 ; buf4:

 ;   dup
    pop [4]
  ;  pop [11]

    jump label4

anth:

    push [6]
    pop [7]

    push [3]
    push 1.2
    div


 ;   dup
  ;  push 4000
  ;  ja buf5
  ;  call chng
 ; buf5:

    dup
    push 200
    jb buf6
    call chng
buf6:

 ;   dup
    pop [3]
  ;  pop [10]

    push [4]
    push 1.2
    div

  ;  dup
  ;  push 2000
  ;  ja  buf7
  ;  call chng
 ; buf7:

    dup
    push 100
    jb buf8
    call chng
buf8:

 ;   dup
    pop [4]
  ;  pop [11]


label4:

    push [10]
  ;  push 1.55
  ;  jae cos-1

  ;  push 3.14
 ;   add

cos-1:

    dup
    push 0
    add
    pop [10]

    cos
    dup
    mul
    sqrt

    push [8]
    mul

    pop [3]

    push [11]
    dup
    mul
    sqrt

  ;  dup
  ;  push 1.55
  ;  jae cos-2

   ; push 3.14
   ; add

cos-2:

    dup
    push 0.03
    add
    pop [11]

    cos
    dup
    mul
    sqrt

    push [9]
    mul

    pop [4]


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
    push 111
    out
    ret
