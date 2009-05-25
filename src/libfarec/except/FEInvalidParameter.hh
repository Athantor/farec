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
 * FEInvalidParameter.hh
 *
 *  Created on: 2009-05-25
 *      Author: athantor
 */

#ifndef FEINVALIDPARAMETER_HH_
#define FEINVALIDPARAMETER_HH_

#include "FarecException.hh"

class FEInvalidParameter : public FarecException, public std::range_error
{
	public:
		FEInvalidParameter( const std::string& = std::string("Invalid parameter") );
		virtual ~FEInvalidParameter() throw ();
};

#endif /* FEINVALIDPARAMETER_HH_ */
