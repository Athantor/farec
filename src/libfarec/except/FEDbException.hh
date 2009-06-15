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
 * DbException.hh
 *
 *  Created on: 2009-06-15
 *      Author: athantor
 */

#ifndef DBEXCEPTION_HH_
#define DBEXCEPTION_HH_

#include <string>

#include "FarecException.hh"

class FEDbException : public FarecException
{
	public:
		virtual ~FEDbException() throw();
	protected:
		FEDbException(const std::string&);
};

#endif /* DBEXCEPTION_HH_ */
