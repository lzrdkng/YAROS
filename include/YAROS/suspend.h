/*
 * Copyright (C) Olivier Dion <olivier.dion@polymtl.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SLEEP_H
#define SLEEP_H

#include "YAROS/def.h"
#include "YAROS/event.h"

struct task;

/**
 * @brief Put a task in the sleeping queue
 *
 * @task The task to put the in the sleeping queue, or NULL for the
 * current task.
 *
 * @note Putting the current task in sleep will reschedule to the next
 * task.
 */
void
suspend(struct task *task);

/**
 * @brief Wake up an initialized task.
 *
 * Move the task from the sleeping queue to the running queue.
 *
 * @param T The task to wake up.
 */
NON_NULL() void
resume(struct task *task);
#endif /* SLEEP_H */
