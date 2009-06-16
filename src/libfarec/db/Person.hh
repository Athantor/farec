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
 * Person.hh
 *
 *  Created on: 2009-06-16
 *      Author: athantor
 */

#ifndef PERSON_HH_
#define PERSON_HH_

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QImage>

#include <cstdint>

class Person
{
	public:
		Person();
		Person( uint64_t, const QUuid&, const QDateTime&, uint64_t = 0, const QString& = QString(),
				const QString& = QString(), const QString& = QString(), const QString& = QString(),
				const QImage& = QImage() );
		virtual ~Person();
		uint64_t getId() const;

		QString getName() const;

		uint64_t getStatus() const;
		QDateTime getTime() const;
		QUuid getUuid() const;
		void setId( uint64_t id );
		void setName( QString name );
		void setStatus( uint64_t status );
		void setTime( QDateTime time );
		void setUuid( QUuid uuid );
		QImage getImg() const;
		void setImg( QImage img );
		QString getAddr() const;
		QString getComments() const;
		QString getSurname() const;
		void setAddr( QString addr );
		void setComments( QString comments );
		void setSurname( QString surname );
	protected:
		uint64_t id;
		QString name;
		QString surname;
		QString addr;
		QString comments;
		QUuid uuid;
		QDateTime ptime;
		uint64_t status;
		QImage img;

};

#endif /* PERSON_HH_ */
