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
 * PplSrchModel.hh
 *
 *  Created on: 2009-09-16
 *      Author: athantor
 */

#ifndef PPLSRCHMODEL_HH_
#define PPLSRCHMODEL_HH_

#include <QAbstractTableModel>
#include <QHash>
#include <QVector>
#include <QSqlQuery>
#include <QVariant>
#include <QModelIndex>

#include <boost/shared_ptr.hpp>

#include "db/FarecDb.hh"
#include "db/Person.hh"

using boost::shared_ptr;

class PplSrchModel : public QAbstractTableModel
{
	public:
		PplSrchModel( const FarecDb::searchres_t, FarecDb& );
		virtual ~PplSrchModel();

		int rowCount( const QModelIndex &parent = QModelIndex() ) const;
		int columnCount( const QModelIndex & parent = QModelIndex() ) const;
		QVariant headerData  ( int section, Qt::Orientation orientation, int role  = Qt::DisplayRole ) const;
		QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

	private:
		//              id             ppl           score
		typedef QHash<uint64_t, QPair<shared_ptr<Person> , double> > p_cache_t;
		//             id            id        ctr
		typedef QHash<QUuid, QPair<uint64_t, uint64_t> > u_cache_t;
		//idx: row;     id
		typedef QVector<uint64_t> row_cache_t;

		p_cache_t cache;
		u_cache_t ucache;
		row_cache_t row_cache; 
		
		FarecDb& fdb;
		FarecDb::searchres_t sres;

		void Prep_cache_from_sres();
		void Get_image_from_db( QImage& im, const QString& id );
};

#endif /* PPLSRCHMODEL_HH_ */
