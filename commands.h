// #define POP(name) \
//     double name = 0; \
//     pop_from_stack (CPU->stack, &name);

DEF_CMD ("add", add,
{
    double a = 0;
    double b = 0;
    //printf ("ya pidor! %d\n", CPU->stack->size);

    pop_from_stack (CPU->stack, &a);
    pop_from_stack (CPU->stack, &b);

    push_in_stack (CPU->stack, a + b);

    //printf ("a + b %lg\n", a + b);

    CPU->ip++;
})

DEF_CMD ("sub", sub,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);
    pop_from_stack (CPU->stack, &b);

    push_in_stack (CPU->stack, b - a);

    CPU->ip++;
})

DEF_CMD ("mul", mul,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);
    pop_from_stack (CPU->stack, &b);

    push_in_stack (CPU->stack, a * b);

    CPU->ip++;
})

DEF_CMD ("div", dvd,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);
    pop_from_stack (CPU->stack, &b);

    assert (a != 0);

    push_in_stack (CPU->stack, b / a);

    CPU->ip++;
})

DEF_CMD ("dup", dup,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);

    push_in_stack (CPU->stack, a + a);

    CPU->ip++;
})

DEF_CMD ("out", out,
{
    double x = 0;

    pop_from_stack (CPU->stack, &x);

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
