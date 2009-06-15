/*
 *  This file is part of farec.
 *
 *  farec is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  farec is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *   along with farec.  If not, see <http://www.gnu.org/licenses/>.
 *
 * FEDbInvalidState.hh
 *
 *  Created on: 2009-06-15
 *      Author: athantor
 */

#ifndef FEDBINVALIDSTATE_HH_
#define FEDBINVALIDSTATE_HH_

#include <string>

#include "FEDbException.hh"

class FEDbInvalidState : public FEDbException
{
	public:
		FEDbInvalidState( const std::string& = std::string("Invalid state of conn. with DB") );
		virtual ~FEDbInvalidState() throw();
};

#endif /* FEDBINVALIDSTATE_HH_ */
