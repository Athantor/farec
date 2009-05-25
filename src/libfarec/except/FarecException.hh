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
 * FarecException.hh
 *
 *  Created on: 2009-05-25
 *      Author: athantor
 */

#ifndef FARECEXCEPTION_HH_
#define FARECEXCEPTION_HH_

#include <stdexcept>
#include <string>

class FarecException :public std::runtime_error
{
	public:
		virtual ~FarecException() throw();
	protected:
		FarecException() = delete;
		FarecException(const std::string&);
	
};

#endif /* FARECEXCEPTION_HH_ */
