#ifndef YR_TASK_H
#define YR_TASK_H

#include "kernel/errno.h"
#include "kernel/def.h"
#include "util/list.h"

#define MIN_STACK_SIZE 40

/**
 * @def TASK
 *
 * I guess this is where C++ templates could be useful...
 *
 * Something like:
 *
 * struct task<20> my_task = {TASK_P0, TASK_N10};
 *
 * Instead of:
 *
 * struct task my_task = TASK(TASK_P0, TASK_N10, 20);
 *
 * But I'm not willing to port to C++ only for that.
 *
 * @note It would be great if the static assertations are put before
 * the members initializer.
 */
#define TASK(PRIO, NICE, SIZE)                                          \
	{								\
		.self = {NULL, NULL},					\
		.stack_pointer = NULL,					\
		.running = TASK_SLEEP,					\
		.priority = PRIO,					\
		.nice = NICE,						\
		.size = SIZE + MIN_STACK_SIZE,				\
		.stack[0 ... (SIZE + MIN_STACK_SIZE - 1)] = 0		\
	};								\
	static_assert( PRIO < TASK_MAX_PRIORITY, "Priority value is too big."); \
	static_assert( NICE < TASK_MAX_NICE, "Nice value is too big.");	\
	static_assert( SIZE >= 0, "Stack size must be greater or equal to 0."); \
	static_assert( (SIZE + MIN_STACK_SIZE) <= MAX_STACK_SIZE, "Stack size if too big.")

#define IS_RUNNABLE(TASK) (( TASK )->running != 0)
#define IS_SLEEPING(TASK) (!IS_RUNNABLE( TASK ))

#define TASK_SLEEP 0b0
#define TASK_RUN 0b1

enum task_priority {
	TASK_P0 = 0b000,
	TASK_P1,
	TASK_P2,
	TASK_P3,
	TASK_P4,
	TASK_P5,
	TASK_P6,
	TASK_P7,
	TASK_MAX_PRIORITY
};

enum task_nice {
	TASK_N0 = 0b0000,
	TASK_N1,
	TASK_N2,
	TASK_N3,
	TASK_N4,
	TASK_N5,
	TASK_N6,
	TASK_N7,
	TASK_N8,
	TASK_N9,
	TASK_N10,
	TASK_N11,
	TASK_N12,
	TASK_N13,
	TASK_N14,
	TASK_N15,
	TASK_MAX_NICE
};

/**
 * @struct task
 *
 * A task is a kernel thread. It has it own stack and therefore is
 * schedulable. When a task die, i.e. when it returned, it return to
 * the kill_self function which do the clean-up. Once a task die, it
 * will never run again until it is scheduled again.
 *
 * Tasks are put in different lists depending on their status. A task
 * can only be in one list at a time:
 *
 * - running_queue is the list of "ready for scheduling" tasks.
 *
 * - sleeping_queue is the list of "waiting for event" tasks.
 *
 * Each task has a time slice. Once that time slice has expired, the
 * task is put at the tail of the running queue according to its
 * priority and its time slice is renew according to its nice value.
 *
 * @var task::self Intrusive double linked list. This is what we use
 * to put the task in the running or sleeping queue for example.
 *
 * @var task::stack_pointer This is the saved SP of the task. Every
 * time the task is scheduled, its SP is restored in the SP'
 * registers. When the task has expired, the SP' registers are saved
 * into the task SP member.
 *
 * @var task::status Represented the overall status of the task. e.g
 * sleeping, running or dying.
 *
 * @var task::stack This is the actual stack of the task. The bottom
 * of the stack will always have the kill_self function.
 *
 * @note It's very important that the first member of the task is
 * task::self. This is because that way we can say that the task is
 * "this". Therefore, you will often see some casting from a pointer
 * of struct list_head to a pointer of struct task. Basic inheritance.
 */
struct task {
	struct list_head self;
	u8 *volatile stack_pointer;
	u8 running:1;
	u8 priority:3;
	u8 nice:4;
	sstack_t size;		/* TODO: Rename my type */
	u8 stack[];
};

/**
 * task_init() - Initiliaze a task before scheduling.
 * @task:  The context.
 * @func:  The function entry point.
 * @data:  Data passed to @func.
 *
 * Always use this function before calling to resume_task().  Or use
 * the macro run_task().
 */
extern void task_init(struct task *task, taskfunc func, void *data);

/**
 * kill_self() - Kill the current task.
 *
 * This function is the end of time for a task.  Once called, it never
 * return to the caller.  It's put at the end of each task stack and
 * called upon returning their entry point.
 *
 * A task can kill itself prematurely by calling kill_self() at any
 * time.  This is equivalent to calling exit(), but there's no error
 * code associated with it.
 */
extern void kill_self(void);

/**
 * @brief kill a specific task.
 *
 * This function will kill the desired task. Once called, the task
 * will never run until it's initialized and wake up again.
 *
 * @task The task to kill
 */
extern void kill_task(struct task *task);

/**
 * @brief Initialize and wake up a task.
 */
#define run_task(TASK, FUNC, DATA)              \
	({					\
		task_init(TASK, FUNC, DATA);	\
		resume_task(TASK);		\
	})


/**
 * @brief Put a task in the sleeping queue
 *
 * @task The task to put the in the sleeping queue, or NULL for the
 * current task.
 *
 * @note Putting the current task in sleep will reschedule to the next
 * task.
 */
void suspend_task(struct task *task);

/**
 * @brief Wake up an initialized task.
 *
 * Move the task from the sleeping queue to the running queue.
 *
 * @param T The task to wake up.
 */
__notnull void resume_task(struct task *task);

#endif
