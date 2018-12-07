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


#include "YAROS/def.h"

/*
 * This function is a helper for the create_task function. It push on
 * the task's stack the kill_self function and the starting function
 * of the task.
 *
 * This is legacy code and will be remove in further version.
 */
NAKED void
arch_prepare_stack(U8 *stack,       /* r25 r24 */
                   taskfunc func)   /* r23 r22 */
{
    /*
     * Prologue.
     *
     * r30-31 : Z register
     */
    asm volatile (
        "push r16\n\t"
        "push r17\n\t"
        "push r30\n\t"
        "push r31"
        );


    /*
     * Z <- stack
     */
    asm volatile (
        "movw r30, r24"
        );

    /*
     * This will push the kill_self on the stack and decrement the
     * pointer.
     *
     * We do this by putting the adress of kill_self in register r16
     * and r17. We then put the result in *stack by deferencing the Z
     * register and decrement it so we point to the next location in
     * the stack.
     *
     * *Z <- lo8(kill_self) : --Z; *Z <- hi8(kill_self) : --Z;
     *
     * The stack is now the following:
     *
     * Top: Low Address
     * |       0        |
     * |       0        |
     * |       0        |
     * |       0        |
     * |       .        |
     * |       .        |
     * |       .        | <- Z
     * | hi8(kill_self) |
     * | lo8(kill_self) |
     * Bottom: High Address
     *
     */
    asm volatile (
        "ldi r16, lo8(gs(kill_self))\n\t"
        "ldi r17, hi8(gs(kill_self))\n\t"
        "st -Z, r16\n\t"
        "st -Z, r17"
        );

    /*
     * We do the same thing has before but instead of using the
     * kill_self, we put in r16 and r17 func.
     *
     * The stack is now:
     *
     * Top: Low Address
     * |       0        |
     * |       0        |
     * |       .        |
     * |       .        |
     * |       .        | <- Z
     * |    hi8(func)   |
     * |    lo8(func)   |
     * | hi8(kill_self) |
     * | lo8(kill_self) |
     * Bottom: High Address
     */
    asm volatile (
        "movw r16, r22\n\t"
        "st -Z, r16\n\t"
        "st -Z, r17"
        );

    /*
     * Epilogue
     */
    asm volatile (
        "pop r31\n\t"
        "pop r30\n\t"
        "pop r17\n\t"
        "pop r16\n\t"
        "ret"
        );
}
