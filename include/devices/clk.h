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


#ifndef CLOCK_H
#define CLOCK_H

#include "kernel/def.h"

#if defined (__AVR_ATmega324PA__)
#  include "drivers/clk/clk_atmega324pa.h"
#endif


error_t
init_clk(clk_t clk,
		 clk_m mode,
		 clk_s src,
		 clk_o out);

error_t
fini_clk(clk_t clk);

error_t
clk_mode(clk_t clk,
		 clk_m mode);

error_t
clk_src(clk_t clk,
		clk_s src);

error_t
clk_out(clk_t clk,
		clk_o out);

clk_m
get_clk_mode(clk_t clk);

clk_s
get_clk_src(clk_t clk);

clk_o
get_clk_out(clk_t clk);

error_t
read_clk(clk_t clk, void *buff);

error_t
write_clk(clk_t clk, const void *buff);
#endif /* CLOCK_H */
