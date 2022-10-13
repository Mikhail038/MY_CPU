 #define POP(name) \
    double name = 0; \
    pop_from_stack (CPU->stack, &name); \
    printf ("   poped %lg\n", name);

 #define PUSH(name) \
    push_in_stack (CPU->stack, name); \
    printf ("   pushed %lg\n", name);


//---------------------------------------------------------------------------


#define PRS_STD \
    fprintf (Code->listing_file, "    %0.4d 0x%0.2X %s\n", \
     Code->ip - 1, Code->ArrCode[Code->ip - 1], ArrCommands[j].name); \
    return 0;

#define REP_STD \
    add_to_array (Array, ArrCommands[i].name); \
    Code->ip++; \
    Array->Buffer[Array->pointer] = '\n'; \
    Array->pointer++; \
    return 0;


//---------------------------------------------------------------------------
#define PRS_JMP \
    if (parse_jump (Source, Code, ArrCommands[j].name) != 0) \
    { \
        return -1; \
    } \
    else return 0;

#define REP_JMP \
    add_to_array (Array, ArrCommands[i].name); \
    reparse_jump (Array, Code, ArrCommands[i].name); \
    return 0;

#define STD_JUMP \
    int label = 0; \
    CPU->ip++; \
    for (int i = 0; i < sizeof (int); i++) \
    { \
        ((unsigned char*) &label)[i] = CPU->Array[CPU->ip]; \
        CPU->ip++; \
    } \
    printf ("   jumped %d from %d\n", label, CPU->ip); \
    CPU->ip = label;

//===========================================================================

DEF_CMD ("hlt", hlt, 0,
{
    printf ("halted in %d\n", CPU->ip);
    return 1;
},
{
    PRS_STD
},
{
    add_to_array (Array, ArrCommands[i].name);
    Code->ip++;
    Array->Buffer[Array->pointer] = '\n';
    Array->pointer++;

    Array->Buffer[Array->pointer] = '\n';
    Array->pointer++;

    return 0;
})

DEF_CMD ("push", push, 1,
{
    run_push (CPU);
},
{
    //printf ("!!!");
    if (parse_push_or_pop (Source, Code, "pop") != 0)
    {
        return -1;
    }
    else return 0;
},
{
    add_to_array (Array, ArrCommands[i].name);
    reparse_push_or_pop (Array, Code, ArrCommands[i].name);
    return 0;
})

DEF_CMD ("pop", pop, 2,
{
    run_pop (CPU);
},
{
    if (parse_push_or_pop (Source, Code, "pop") != 0)
    {
        return -1;
    }
    else return 0;
},
{
    add_to_array (Array, ArrCommands[i].name);
    reparse_push_or_pop (Array, Code, ArrCommands[i].name);
    return 0;
})

DEF_CMD ("add", add, 3,
{
    POP (a);
    POP (b);

    PUSH (a + b);

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("sub", sub, 4,
{
    POP (a);
    POP (b);

    PUSH (b - a);

    CPU->ip++;
},
{
    PRS_STD
},
{

})

DEF_CMD ("mul", mul, 5,
{
    POP (a);
    POP (b);

    PUSH (a * b);

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("div", m_div, 6,
{
    POP (a);
    POP (b);


    assert (a != 0);

    PUSH (b / a);


    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("inp", inp, 7,
{
    double x = 0;

    printf ("Enter number to push:\n");
    scanf ("%lg", &x);

    push_in_stack (CPU->stack, x);

    printf ("   inputed %lg\n", x);

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("out", out, 8,
{
    POP (x);

    printf ("\n\nout:  %lg\n", x);
    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("dump", dump, 9,
{
    printf ("Work in progress\n");
    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})


DEF_CMD ("dup", dup, 10,
{
    POP (a);

    PUSH (a);
    PUSH (a);

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("jump", jump, 11,
{
    STD_JUMP
},
{
    PRS_JMP
},
{
    REP_JMP
}
) //jump

DEF_CMD ("ja", ja, 12,
{
    POP(a);
    POP(b);

    if (a > b)
    {
        STD_JUMP
    }
    else
    {
        CPU->ip += 5;
    }
},
{
    PRS_JMP
},
{
    REP_JMP
}
) //ja

DEF_CMD ("jb", jb, 13,
{
    POP(a);
    POP(b);

    if (a < b)
    {
        STD_JUMP
    }
    else
    {
        CPU->ip += 5;
    }
},
{
    PRS_JMP
},
{
    REP_JMP
}
) //jb

DEF_CMD ("jae", jae, 14,
{
    POP(a);
    POP(b);

    if (a >= b)
    {
        STD_JUMP
    }
    else
    {
        CPU->ip += 5;
    }
},
{
    PRS_JMP
},
{
    REP_JMP
}) //jae

DEF_CMD ("jbe", jbe, 15,
{
    POP(a);
    POP(b);

    if (a <= b)
    {
        STD_JUMP
    }
    else
    {
        CPU->ip += 5;
    }
},
{
    PRS_JMP
},
{
    REP_JMP
}) //jbe

DEF_CMD ("je", je, 16,
{
    POP(a);
    POP(b);

    if (a == b)
    {
        STD_JUMP
    }
    else
    {
        CPU->ip += 5;
    }
},
{
    PRS_JMP
},
{
    REP_JMP
}) //je

DEF_CMD ("jne", jne, 17,
{
    POP(a);
    POP(b);

    if (a != b)
    {
        STD_JUMP
    }
    else
    {
        CPU->ip += 5;
    }
},
{
    PRS_JMP
},
{
    REP_JMP
}) //jne


DEF_CMD ("call", call, 18,
{
    int label = 0;

    CPU->ip++;

    for (int i = 0; i < sizeof (int); i++)
    {
        ((unsigned char*) &label)[i] = CPU->Array[CPU->ip];
        CPU->ip++;
    }
    push_in_stack (CPU->addres_stack, CPU->ip);

    printf ("   called %d from %d\n", label, CPU->ip);
    CPU->ip = label;
},
{
    PRS_JMP
},
{
    REP_JMP
})

DEF_CMD ("ret", ret, 19,
{
    double d_label = 0;

    pop_from_stack (CPU->addres_stack, &d_label);

    int label = (int) d_label;

    printf ("   returned %d from %d\n", label, CPU->ip);
    CPU->ip = label;
},
{
    PRS_STD
},
{
    add_to_array (Array, ArrCommands[i].name);
    Code->ip++;
    Array->Buffer[Array->pointer] = '\n';
    Array->pointer++;

    Array->Buffer[Array->pointer] = '\n';
    Array->pointer++;

    return 0;
})

DEF_CMD ("sqrt", m_sqrt, 20,
{
    POP (a);

    PUSH (sqrt (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("sin", m_sin, 21,
{
    POP (a);

    PUSH (sin (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("cos", m_cos, 22,
{
    POP (a);

    PUSH (cos (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("rnd", rnd, 23,
{
    POP (a);

    PUSH (floor (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("vsetx", vsetx, 24,
{
    POP (a);

    PUSH (floor (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("vsety", vsety, 25,
{
    POP (a);

    PUSH (floor (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("updscr", updscr, 26,
{
    POP (a);

    PUSH (floor (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("clrscr", clrscr, 27,
{
    POP (a);

    PUSH (floor (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

DEF_CMD ("clrbuf", clrbuf, 28,
{
    POP (a);

    PUSH (floor (a));

    CPU->ip++;
},
{
    PRS_STD
},
{
    REP_STD
})

#undef STD_JUMP

#undef POP

#undef PUSH
