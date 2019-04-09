#include <assert.h>

#include "arch/atomic.h"

#include "init/init.h"

#include "kernel/def.h"
#include "kernel/task.h"
#include "kernel/panic.h"
#include "kernel/sysclk.h"
#include "kernel/msg.h"

#include "devices/usart.h"

struct task test_atom8 = TASK(TASK_P0, TASK_N15, 100);

__os_task void do_atom8(void *nil)
{
	static const char fail_read[] = "Failed read atom8\n";
	static const char fail_inc[] = "Failed inc atom8\n";
	static const char success[] = "Success\n";

	atomic8_t atom = {0};

	if (atomic8_read(&atom) != 0)
		print_kernel(fail_read);

	if (atomic8_inc(&atom) != 1)
		print_kernel(fail_inc);
	else
		print_kernel(success);

	panic_kernel(0);
}

int main()
{
	init_kernel();
	run_task(&test_atom8,
		 do_atom8,
		 NULL);
	run_kernel();
}
