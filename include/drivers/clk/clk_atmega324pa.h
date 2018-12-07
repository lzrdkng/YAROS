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

#ifndef CLK_M324PA_H
#define CLK_M324PA_H

#include <avr/io.h>

typedef enum __attribute__((packed)) clk_type {
	CLK_0,
	CLK_1,
	CLK_2,
	CLK_TYPE_MAX
} clk_t;

typedef enum __attribute__((packed)) clk_mode {
	CLK_NORMAL = 0b0000,
	CLK_CTC,					/* CTC with OCRnA compare */
	CLK_CTCI, 					/* CTC with ICRn Compare (Input Capture) */
	CLK_PWM,					/* Fast PWM */
	CLK_PWMA,					/* Fast PWM with OCRnA Compare */
	CLK_PWMI,					/* Fast PWM with IRCRn Compare (Input Capture) */
	CLK_PWM8,					/* Fast PWM, 8-bit */
	CLK_PWM9,					/* Fast PWM, 9-bit */
	CLK_PWM10,					/* Fast PWM, 10-bit */
	CLK_PWMP,					/* Phase Correct PWM */
	CLK_PWMPA,					/* Phase Correct PWM with OCRnA Compare */
	CLK_PWMPI,					/* Phase Correct PWM with ICRn Compare (Input Capture) */
	CLK_PWMP8,					/* Phase Correct PWM, 8-bit */
	CLK_PWMP9,					/* Phase Correct PWM, 9-bit */
	CLK_PWMP10,					/* Phase Correct PWM, 10-bits*/
	CLK_PWMPF,					/* Phase & Frequency Correct PWM */
	CLK_PWMPFA,					/* Phase & Frequency Correct PWM with OCRnA Compare */
	CLK_PWMPFI					/* Phase & Frequency Correct PWM with ICRn Compare */
} clk_m;

typedef enum __attribute__((packed)) clk_src {
	CLK_STOP,
	CLK_P1,						/* CPU from prescalers 1 to 1024 */
	CLK_P8,
	CLK_P32,
	CLK_P64,
	CLK_P128,
	CLK_P256,
	CLK_P1024,
	CLK_EXTF,					/* External source falling */
	CLK_EXTR					/* External source rising */
} clk_s;


typedef enum __attribute__((packed)) clk_o {
	/* Non PWM mode */
	CLK_DB = 0x00,	   /*< Normal port operation, OCnB disconnected */
	CLK_TB = 0x10,	   /*< Toggle OCnB on Compare Match */
	CLK_CB = 0x20,	   /*< Clear OCnB on Compare Match */
	CLK_SB = 0x30,	   /*< Set OCnB on Compare Match */
	CLK_DA = 0x00,	   /*< Normal port operation, OCnA disconnected */
	CLK_TA = 0x40,	   /*< Toggle OCnA on Compare Match */
	CLK_CA = 0x80,	   /*< Clear OCnA on Compare Match */
	CLK_SA = 0xC0	   /*< Set OCnA on Compare Match */
} clk_o;





#endif /* CLK_M324PA_H */
