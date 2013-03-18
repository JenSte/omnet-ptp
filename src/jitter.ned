//
//  Clock Synchronization Simulation in OMNet++
//  Copyright (C) 2012 Jens Steinhauser
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

package ptp;

simple Jitter
{
	parameters:
		volatile double delay @unit(s) = default(min(fabs(normal(10e-6s, 20e-6s)), 50e-6s));

	gates:
		inout upperLayer;
		inout lowerLayer;
}
