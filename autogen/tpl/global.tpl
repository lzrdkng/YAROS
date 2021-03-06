[< AutoGen5 template -*- mode:c -*- h c>]
[< HEADER >]
[< CASE (suffix) >]
[< == h >]
#ifndef YAROS_GLOBAL_H
#define YAROS_GLOBAL_H

#include "kernel/def.h"
#include "util/dlist.h"
[< FOR variables "" >]
extern [< (get "name") >];
[< ENDFOR variables >]
#endif /* YAROS_GLOBAL_H */
[< == c >]
#include "kernel/global.h"
#include "kernel/sysclk.h"
[< FOR variables "" >]
[< (get "name") >] = [< (get "value") >];
[< ENDFOR variables >]
[< ESAC >]

[< define HEADER >]
/*
 * This file was generated by AutoGen [< (emit autogen-version) >]
 * on [< (shell "date") >]
 */
[< enddef >]
