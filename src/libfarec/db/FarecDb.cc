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
 * FarecDb.cc
 *
 *  Created on: 2009-06-15
 *      Author: athantor
 */

#include "FarecDb.hh"

FarecDb::FarecDb() :
	dbconn(QSqlDatabase::addDatabase("QPSQL"))
{
}

FarecDb::~FarecDb()
{
	// TODO Auto-generated destructor stub
}

bool FarecDb::open( const QString & uname, const QString & pass, const QString & host, uint16_t port )
{
	dbconn.setConnectOptions("connect_timeout=20");
	
	dbconn.setHostName(host);
	dbconn.setPort(port);
	
	return dbconn.open(uname, pass);
}

void FarecDb::close()
{
	dbconn.close();
}

const QSqlDatabase & FarecDb::Get_dbconn() const
{
	return dbconn;
}





