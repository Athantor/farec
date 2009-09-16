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
 * PplSrchModel.cc
 *
 *  Created on: 2009-09-16
 *      Author: athantor
 */

#include "PplSrchModel.hh"

PplSrchModel::PplSrchModel( const FarecDb::searchres_t res, FarecDb& db ) :
	fdb(db), sres(res)
{
	Prep_cache_from_sres();
}

PplSrchModel::~PplSrchModel()
{
}

int PplSrchModel::rowCount( __attribute__((unused))  const QModelIndex &parent ) const
{
	return cache.size();
}

void PplSrchModel::Prep_cache_from_sres()
{
	if(not fdb.Get_dbconn().isOpen())
	{
		return;
	}

	for(auto it = sres->begin(); it != sres->end(); ++it)
	{
		QSqlQuery que(fdb);

		if(que.exec(QString::fromUtf8("SELECT * FROM \"People\" AS ppl WHERE ppl.\"ID\" = "
			"( SELECT pf.\"Person\" FROM \"Ppl_faces\" AS pf WHERE pf.\"Data_serie\" = %1) ").arg(it.key())))
		{
			while(que.next())
			{
				QUuid uuid = QUuid(que.value(5).toString());
				if(ucache.contains(uuid))
				{
					cache[ucache[uuid].first].second += it.value();
					ucache[uuid].second++;
				}
				else
				{
					QImage pic;
					shared_ptr<Person> tmpp(new Person(que));

					Get_image_from_db(pic, QString::fromUtf8("%1").arg(tmpp->getId()));
					tmpp->setImg(pic);

					ucache[uuid].first = tmpp->getId();
					ucache[uuid].second = 1;

					cache[tmpp->getId()].first = tmpp;
					cache[tmpp->getId()].second = it.value();
					
					row_cache.push_back(tmpp->getId());
				}
			}

		}
	}

}

void PplSrchModel::Get_image_from_db( QImage& im, const QString& id )
{
	QSqlQuery query(fdb);
	query.prepare("SELECT \"photo\" FROM \"Ppl_miniphotos\" WHERE \"Person\" = ?;");
	query.bindValue(0, id);

	if(query.exec() and query.first())
	{
		im = QImage::fromData(QByteArray::fromBase64(query.value(0).toByteArray()));
	}
}

int PplSrchModel::columnCount  ( __attribute__((unused)) const QModelIndex & parent ) const
{
	return 4;
}

QVariant PplSrchModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation == Qt::Horizontal)
	{
		switch (section) {
			case 0:
				return QString::fromUtf8("Punkty");
				break;
			case 1:
				return QString::fromUtf8("Nazwisko");
				break;
			case 2:
				return QString::fromUtf8("Imię");
				break;
			case 3:
				return QString::fromUtf8("Adres");
				break;
			default:
				return QString::fromUtf8("?");
				break;
		}
	}
	else
		return QString("%1").arg(section);

}

QVariant PplSrchModel::data( const QModelIndex & index, int role ) const
{
	if(!index.isValid())
		return QVariant();

	std::cerr << index.row() << ", " <<  rowCount() << ": " << row_cache.size() << std::endl; 
	
	if(index.row() >= rowCount() or index.column() >= columnCount())
	{
		return QVariant();
	}

	p_cache_t::mapped_type::first_type buf(cache[row_cache[index.row()]].first);

	if(role == Qt::DisplayRole)
	{

		//score, surname, name, addr
		switch(index.column())
		{
			case 0:
				return QVariant(cache[row_cache[index.row()]].second / (ucache[buf->getUuid()].second == 0 ? 1 :  ucache[buf->getUuid()].second) );
				break;
			case 1:
				return QVariant(buf->getSurname());
				break;
			case 2:
				return QVariant(buf->getName());
				break;
			case 3:
				return QVariant(buf->getAddr());
				break;
			default:
				return QVariant();
		}
	}
	else if(role == Qt::ToolTipRole)
	{
		if(index.column() == 1)
		{
			return QVariant(buf->getComments());
		}
		else
		{
			return QVariant();
		}
	}
	else if(role == Qt::DecorationRole)
	{
		if(index.column() == 1)
		{
			return QVariant(buf->getImg());
		}
		else
		{
			return QVariant();
		}
	}
	else
	{
		return QVariant();
	}

	return QVariant();

}
