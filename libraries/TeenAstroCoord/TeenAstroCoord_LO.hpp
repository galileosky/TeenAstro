
/*
 * Title       TeenAstro coordinate
 * by          Charles Lemaire
 *
 * Copyright (C) Charles Lemaire
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
 *
 *
 *
 * Revision History, see GitHub
 *
 *
 * Lib  Authors: Charles Lemaire
 *
 * Description:
 *
 * Object oriented coordinate definition
 *
 */
#pragma once

#ifndef __TeenAstroCoord_LO_hpp__
#define __TeenAstroCoord_LO_hpp__

#ifndef M_PI
#include <math.h> // for M_PI
#endif

#include "TeenAstroCoord.hpp"

class Coord_EQ;

class Coord_LO : protected Coord {
public:
	Coord_LO(double Axis3, double Axis2, double Axis1);
	Coord_EQ To_Coord_EQ(const double(&trafoinv)[3][3]);
	double Axis3();
	double Axis2();
	double Axis1();
};


#endif // __TeenAstroCoord_LO_hpp__
