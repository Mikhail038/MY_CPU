#define m1 32
#define m2 64
#define m3 128


DEF_CMD (hlt,  0)

DEF_CMD (push, 1)

DEF_CMD (pop, 2)

DEF_CMD (add, 3)

DEF_CMD (sub, 4)

DEF_CMD (mul, 5)

DEF_CMD (inp, 7)

DEF_CMD (out, 8)

DEF_CMD (dump, 9)

DEF_CMD (dup, 10)

DEF_CMD (jump, 11)

DEF_CMD (ja, 11 + m1)

DEF_CMD (jb, 11 + m2)

DEF_CMD (jae, 11 + m1 + m3)

DEF_CMD (jbe, 11 + m2 + m3)

DEF_CMD (je, 11 + m3)

DEF_CMD (jne, 11 + m1 + m2 + m3)

DEF_CMD (call, 12)

DEF_CMD (ret, 13)





