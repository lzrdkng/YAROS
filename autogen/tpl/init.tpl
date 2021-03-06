[< AutoGen5 template c >]
[< HEADER >]
#include "kernel/def.h"
#include "kernel/global.h"
#include "kernel/panic.h"
#include "kernel/task.h"
#include "kernel/sched.h"
#include "devices/irq.h"

#include "init/init.h"

#include "util/dlist.h"

[< FOR init "" >]
extern [< (get "retval") >] [< (get "name") >]([< FOR params > ", " >][< params >][< ENDFOR params >]);
[< ENDFOR init >]

void
init_kernel()
{
[< FOR init "" >]
     [< (get "name") >]([< FOR params ", " >][< params >][< ENDFOR params >]);
[< ENDFOR init >]
}


[< define HEADER >]
/*
 * This file was generated by AutoGen [< (emit autogen-version) >]
 * on [< (shell "date") >]
 */
[< enddef >]
