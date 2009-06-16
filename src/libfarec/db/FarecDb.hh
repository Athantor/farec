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
 * FarecDb.hh
 *
 *  Created on: 2009-06-15
 *      Author: athantor
 */

#ifndef FARECDB_HH_
#define FARECDB_HH_

#include <QSqlDatabase>
#include <QString>


class FarecDb
{
	public:
		FarecDb();
		virtual ~FarecDb();
		
		const static size_t CONN_TIMEOUT = 20;
	
		bool open(const QString &, const QString&, const QString&, uint16_t);
		void close();
		
		
		
		const QSqlDatabase& Get_dbconn() const;
		
	private:
		QSqlDatabase dbconn;
	
};

#endif /* FARECDB_HH_ */
