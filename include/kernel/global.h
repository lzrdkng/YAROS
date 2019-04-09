#ifndef YR_KERNEL_GLOBAL_H
#define YR_KERNEL_GLOBAL_H

#ifdef X
#undef X
#endif

#define X(T, name, value) extern T name
#include "kernel/global.list"
#undef X
#endif
