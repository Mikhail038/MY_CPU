// #define POP(name) \
//     double name = 0; \
//     pop_from_stack (CPU->stack, &name);

DEF_CMD ("add", add,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);
    pop_from_stack (CPU->stack, &b);

    push_in_stack (CPU->stack, a + b);

    return 0;
    break;
})

DEF_CMD ("sub", sub,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);
    pop_from_stack (CPU->stack, &b);

    push_in_stack (CPU->stack, b - a);

    return 0;
    break;
})

DEF_CMD ("mul", mul,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);
    pop_from_stack (CPU->stack, &b);

    push_in_stack (CPU->stack, a * b);

    return 0;
    break;
})

DEF_CMD ("div", dvd,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);
    pop_from_stack (CPU->stack, &b);

    assert (a != 0);

    push_in_stack (CPU->stack, b / a);

    return 0;
    break;
})

DEF_CMD ("dup", dup,
{
    double a = 0;
    double b = 0;

    pop_from_stack (CPU->stack, &a);

    push_in_stack (CPU->stack, a + a);

    return 0;
    break;
})

DEF_CMD ("out", out,
{
    printf ("out:  %lg", CPU->stack->data[CPU->stack->size]);

    return 0;
    break;
})

DEF_CMD ("hlt", hlt,
{
    return 1;
    break;
})
