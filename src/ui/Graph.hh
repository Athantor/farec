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
 * Graph.hh
 *
 *  Created on: 2009-05-31
 *      Author: athantor
 */

#ifndef GRAPH_HH_
#define GRAPH_HH_

#include <QFrame>
#include <QBrush>
#include <QPen>
#include <QPainter>
#include <QSize>

#include <algorithm>
#include <cmath>
#include <functional>

#include <boost/scoped_ptr.hpp>

using boost::scoped_ptr;

class Graph : public QFrame
{

	Q_OBJECT

	public:

		enum Graph_dir
		{
			LTR, UTD
		};

		Graph( QWidget * = 0, QWidget * = 0, Graph_dir = LTR, QPen * = new QPen("black"), QBrush * = new QBrush("white") );
		Graph( const Graph& );
		Graph& operator=( const Graph& );
		virtual ~Graph();

		virtual void paintEvent( QPaintEvent * );

		const QVector<double>& Get_data() const;
		const QBrush& Get_brush() const;
		Graph_dir Get_gdir() const;
		const QPen& Get_pen() const;
		const QWidget * Get_buddy() const;
		void Set_buddy(QWidget *);
		void Set_data( QVector<double> * );
		void Set_brush( const QBrush& );
		void Set_gdir( Graph_dir );
		void Set_pen( const QPen& );
		
		virtual QSize minimumSizeHint () const;

	protected:
		Graph_dir mydir;
		QWidget * buddy;
		scoped_ptr<QVector<double> > data;
		scoped_ptr<QPen> mypen;
		scoped_ptr<QBrush> mybrush;

};

#endif /* GRAPH_HH_ */
