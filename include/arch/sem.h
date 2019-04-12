#ifndef YR_ARCH_SEM_H
#define YR_ARCH_SEM_H

#include "kernel/def.h"
#include "util/list.h"

struct sem {
	void *list;
	u8 counter;
};

#define SEMAPHORE(name)				\
	struct sem name = {0}

extern void sem_up(struct sem *sem);
extern void sem_down (struct sem *sem);

#endif
