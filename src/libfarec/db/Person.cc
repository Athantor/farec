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
 * Person.cc
 *
 *  Created on: 2009-06-16
 *      Author: athantor
 */

#include "Person.hh"

Person::Person() :
	id(0)
{
}

Person::Person( uint64_t i, const QUuid& ui, const QDateTime& dt, uint64_t stt, const QString& na,
		const QString& su, const QString& adr, const QString& cmt, const QImage& im ) :
	id(i), uuid(ui), ptime(dt), status(stt), name(na), surname(su), addr(adr), comments(cmt), img(im)
{

}

Person::Person(const QSqlQuery& que, const QImage& img):
		id(que.value(0).toULongLong(0)), 
		uuid(QUuid(que.value(5).toString())), 
		ptime(que.value(6).toDateTime()), 
		status(que.value(7).toULongLong()), 
		name(que.value(1).toString()), 
		surname(que.value(2).toString()), 
		addr(que.value(3).toString()), 
		comments(que.value(4).toString()), 
		img(img)
{
	
}

Person::~Person()
{
}

uint64_t Person::getId() const
{
	return id;
}

QString Person::getName() const
{
	return name;
}

uint64_t Person::getStatus() const
{
	return status;
}

QDateTime Person::getTime() const
{
	return ptime;
}

QUuid Person::getUuid() const
{
	return uuid;
}

void Person::setId( uint64_t id )
{
	this->id = id;
}

void Person::setName( QString name )
{
	this->name = name;
}

void Person::setStatus( uint64_t status )
{
	this->status = status;
}

void Person::setTime( QDateTime ptime )
{
	this->ptime = ptime;
}

void Person::setUuid( QUuid uuid )
{
	this->uuid = uuid;
}

QImage Person::getImg() const
{
	return img;
}

void Person::setImg( const QImage & img )
{
	this->img = img;
}

QString Person::getAddr() const
{
	return addr;
}

QString Person::getComments() const
{
	return comments;
}

QString Person::getSurname() const
{
	return surname;
}

void Person::setAddr( QString addr )
{
	this->addr = addr;
}

void Person::setComments( QString comments )
{
	this->comments = comments;
}

void Person::setSurname( QString surname )
{
	this->surname = surname;
}
