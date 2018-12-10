#include "YAROS.h"

/* foo has priority level 0, nice value of 5 and a stack of 50 bytes */
struct task foo = TASK(TASK_P0, TASK_N5, 50);

/* foo has priority level 4, nice value of 9 and a stack of 75 bytess */
struct task bar = TASK(TASK_P4, TASK_N9, 75);

OS_TASK void
do_foo(void *task);

OS_TASK void
do_bar(void *nil);

int
main(int argc, char *argv[])
{
    /* Initialize the Kernel */
    kinit();

    /* Foo is scheduled */
    run_task(&foo,
             do_foo,
             &bar);

    /* Bar is suspended */
    init_task(&bar,
              do_bar,
              NULL);

    krun();
}


OS_TASK void
do_foo(void *bar_task)
{
    static const char foo_str[] = {'f', 'o', 'o', '\n'};

    DEBUG("Entering do_foo");

    while (1) {



        /* Write "foo_str\n" to USART 0 */
        write_usart(0, foo_str, sizeof(foo_str));

        /* Resume bar */
        resume(bar_task);

        /* Wait 1 second */
        wait(HZ);

        /* Suspend bar */
        suspend(bar_task);
    }
}

OS_TASK void
do_bar(void *nil)
{
    static const char bar_str[] = {'b', 'a', 'r', '\n'};

    while (1) {

        /* Write "bar\n" to USART 0 */
        write_usart(0, bar_str, sizeof(bar_str));

        /* Wait 1/2 second */
        wait(HZ / 2);
    }
}
