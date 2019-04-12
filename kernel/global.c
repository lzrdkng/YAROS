/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * kernel/global.c - All global variables should go there.
 *
 * Copyright (C) 2019 Olivier Dion <olivier.dion@polymtl.ca>
 */

#include "kernel/type.h"
#include "util/list.h"

#undef X
#define X(T, name, value) T name = value;
#include "kernel/global.list"
