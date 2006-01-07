//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Instantiation of linear combination types
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include "linrcomb.imp"
#include <libgambit/vector.imp>
#include <libgambit/matrix.imp>
#include "double.h"

template Gambit::Vector<gDouble> Gambit::operator*(const Gambit::Vector<gDouble>&,
					      const Gambit::Matrix<gDouble>&);

template class LinearCombination<double>;
template class LinearCombination<gDouble>;
template class LinearCombination<Gambit::Rational>;
