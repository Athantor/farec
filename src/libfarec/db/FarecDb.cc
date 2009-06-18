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

	dbconn.setDatabaseName("farec");
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

bool FarecDb::Insert_facedata( const Classifier::segments_t& segs, uint64_t persid )
{
	QSqlQuery quer(dbconn);
	QUuid id = QUuid::createUuid();
	QVariant serid;
	QVariantList ft, fd, fds;

	dbconn.transaction();

	quer.prepare("INSERT INTO \"Data_series\" (\"UUID\") VALUES ( ? ); ");
	quer.bindValue(0, id.toString());

	if(not quer.exec())
	{
		QMessageBox::critical(0, QString::fromUtf8("Błąd"), QString::fromUtf8("Błąd zapisu do DB:\n%1").arg(
				quer.lastError().text()));

		dbconn.rollback();
		return false;
	}

	if(not quer.exec("SELECT COUNT(*), MAX(\"ID\") FROM \"Data_series\";") or not quer.first()
			or (quer.value(0).toInt() > 0 and not (serid = quer.value(1)).isValid()))
	{
		QMessageBox::critical(0, QString::fromUtf8("Błąd"), QString::fromUtf8("Błąd zapisu do DB:\n%1").arg(
				quer.lastError().text()));

		dbconn.rollback();
		return false;
	}

	quer.prepare(
			"INSERT INTO \"Face_data\" (\"Type\", \"Data\", \"Data_serie\") VALUES ( (SELECT \"ID\" FROM \"Segments\" WHERE \"Farecprog_id\" = ?) , ?, ? ) ");

	for(Classifier::segments_t::const_iterator it = segs.begin(); it != segs.end(); it++)
	{
		ft << it.key();
		fd << it.value()->get<1> ();
		fds << serid;
	}

	quer.addBindValue(ft);
	quer.addBindValue(fd);
	quer.addBindValue(fds);

	if(not quer.execBatch())
	{
		QMessageBox::critical(0, QString::fromUtf8("Błąd"), QString::fromUtf8("Błąd zapisu do DB:\n%1").arg(
				quer.lastError().text()));

		dbconn.rollback();
		return false;
	}

	//QMessageBox::critical(0, QString::fromUtf8("Błąd"), QString::fromUtf8("id:\n%1\n%2\n%3 - %4 - %5").arg(serid.toInt()).arg(segs.size()).arg(ft.size()).arg(fd.size()).arg(fds.size()));

	quer.prepare("INSERT INTO \"Ppl_faces\" (\"Person\", \"Data_serie\" ) VALUES ( ? , ?) ");
	quer.bindValue(0, static_cast<qulonglong> (persid));
	quer.bindValue(1, serid);

	if(not quer.exec())
	{
		QMessageBox::critical(0, QString::fromUtf8("Błąd"), QString::fromUtf8("Błąd zapisu do DB:\n%1").arg(
				quer.lastError().text()));

		dbconn.rollback();
		return false;
	}

	return dbconn.commit();
}

FarecDb::searchres_t FarecDb::Find_faces( const Classifier::segments_t& segs, double tol )
{
	searchres_t ret(new searchres_t::value_type);
	searchres_t::value_type totsum, score;

	QSqlQuery que(dbconn);

	que.prepare(
			"SELECT fd.\"Data_serie\" AS ds, s.\"Farecprog_id\" AS fpid, CAST(\"Data\" AS text) AS dt, CAST(\"Data\" - (\"Data\" * ?) AS TEXT) AS dtmin,  CAST(\"Data\" + "
				"(\"Data\" * ?) AS TEXT) AS dtmax  FROM \"Face_data\" fd LEFT JOIN \"Segments\" s ON s.\"ID\" = fd.\"Type\" ;");
	que.bindValue(0, tol);
	que.bindValue(1, tol);

	if(not que.exec())
	{
		QMessageBox::critical(0, QString::fromUtf8("Błąd"), QString::fromUtf8("Błąd odczytu z DB:\n%1").arg(
				que.lastError().text()));

		return searchres_t();
	}

	while(que.next())
	{
		const Classifier::segments wat = static_cast<decltype(wat)>(que.value(1).toInt());
		const uint64_t theds = que.value(0).toULongLong();
		const double theval = segs[wat]->get<1>();
	
		if(not totsum.contains(theds))
		{
			totsum[theds] = 0;
		}
		
		
		if(wat != Classifier::_BASE )
		{
			
			totsum[theds] += theval;
			
			if(not score.contains(theds))
			{
				score[theds] = 0;
			}
			
			if(theval > que.value(3).toDouble() and theval < que.value(4).toDouble())
			{
				score[theds] += que.value(2).toDouble();
			}
		}
	}
	
	/*
	 * Decresing score value for from total because this way skipping lines is penalized, since lower score is better 
	 */
	for(searchres_t::value_type::iterator it = score.begin(); it != score.end(); it++)
	{
		(*ret)[it.key()] = std::abs(totsum[it.key()] - *it);
	}
	
	return ret;
}
