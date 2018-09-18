/*	wii.cpp stores input data from the guitar.

	Copyright (C) 2018  Stephen Barrack

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	For any issues, please contact stephen.barrack12@yahoo.com.
*/

#include "revolution.h"

void Nunchuck::updateReport() {
	report.sx = raw[0];
	report.sy = raw[1];

	report.ax = raw[2] << 2 | (raw[5] >> 2 & 3);
	report.ay = raw[3] << 2 | (raw[5] >> 4 & 3);
	report.az = raw[4] << 2 | raw[5] >> 6;

	report.c = ~raw[5] >> 1;
	report.z = ~raw[5];
}

void ClassicController::updateReport() {
	report.sx = raw[0];
	report.sy = raw[1];
	report.cx = raw[0] >> 3 | raw[1] >> 5 | raw[2] >> 7;
	report.cy = raw[2];

	report.lt = raw[2] >> 2 | raw[3] >> 5;
	report.rt = raw[3];

	report.buttons[0] = ~raw[4];
	report.buttons[1] = ~raw[5];
}

void GuitarWii::updateReport() {
	//inverted for left-hand, do somewhere else
	report.sx = ~raw[0];
	report.sy = ~raw[1];

	report.cy = raw[2];
	report.rt = raw[3];

	report.buttons[0] = ~raw[4];
	report.buttons[1] = ~raw[5];
}

/*void DrumsWii::updateReport() {
	report.sx = raw[0];
	report.sy = raw[1];

	report.velocity[0] = ~raw[2];
	report.velocity[1] = ~raw[3];

	report.buttons[0] = ~raw[4];
	report.buttons[1] = ~raw[5];
}

void TaikoWii::updateReport() {
	report.buttons = ~raw[0];
}*/
