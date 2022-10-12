 #define POP(name) \
    double name = 0; \
    pop_from_stack (CPU->stack, &name); \
    printf ("   poped %lg\n", name);

 #define PUSH(name) \
    push_in_stack (CPU->stack, name); \
    printf ("   pushed %lg\n", name);

DEF_CMD ("add", add,
{
    POP (a);
    POP (b);

    PUSH (a + b);

    CPU->ip++;
})

DEF_CMD ("sub", sub,
{
    POP (a);
    POP (b);

    PUSH (b - a);

    CPU->ip++;
})

DEF_CMD ("mul", mul,
{
    POP (a);
    POP (b);

    PUSH (a * b);

    CPU->ip++;
})

DEF_CMD ("div", dvd,
{
    POP (a);
    POP (b);


    assert (a != 0);

    PUSH (b / a);


    CPU->ip++;
})

DEF_CMD ("dup", dup,
{
    POP (a);

    PUSH (a);
    PUSH (a);

    CPU->ip++;
})

DEF_CMD ("out", out,
{
    POP (x);

    printf ("out:  %lg\n", x);
    CPU->ip++;
})

DEF_CMD ("hlt", hlt,
{
    printf ("halted\n");
    return 1;
})


DEF_CMD ("push", push,
{
    run_push (CPU);
})

DEF_CMD ("pop", pop,
{
    run_pop (CPU);
})

DEF_CMD ("jump", jump,
{
    run_jump (CPU);
})

DEF_CMD ("call", call,
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
})

DEF_CMD ("ret", ret,
{
    double d_label = 0;

    pop_from_stack (CPU->addres_stack, &d_label);

    int label = (int) d_label;

    printf ("   returned %d from %d\n", label, CPU->ip);
    CPU->ip = label;
})

#undef POP

#undef PUSH
