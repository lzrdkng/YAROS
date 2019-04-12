#include <string.h>
#include <stdio.h>

#include "arch/atomic.h"

#include "init/init.h"
#include "kernel/task.h"
#include "kernel/panic.h"
#include "kernel/sysclk.h"
#include "kernel/msg.h"
#include "devices/usart.h"
#include "ipc/mutex.h"

/* foo has priority level 0, nice value of 5 and a stack of 50 bytes */
struct task foo = TASK(TASK_P0, TASK_N15, 100);

/* foo has priority level 4, nice value of 9 and a stack of 75 bytess */
struct task bar = TASK(TASK_P4, TASK_N15, 75);

OS_TASK void
do_foo(void *task);

OS_TASK void
do_bar(void *nil);

int
main(int argc, char *argv[])
{
	/* Initialize the Kernel */
	init_kernel();

	/* Foo is scheduled */
	run_task(&foo,
		 do_foo,
		 &bar);

	/* Bar is suspended */
	run_task(&bar,
		 do_bar,
		 NULL);

	run_kernel();
}


OS_TASK void
do_foo(void *bar_task)
{
	static const char foo_str[] = "foo\n";

	DEBUG("do_foo");

	while (1) {
		print_kernel(foo_str);
		wait(HZ);
	}

}

OS_TASK void
do_bar(void *nil)
{
	static const char bar_str[] = "bar\n";

	while (1) {
		print_kernel(bar_str);
	}


}
