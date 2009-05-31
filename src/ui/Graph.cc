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
 * Graph.cc
 *
 *  Created on: 2009-05-31
 *      Author: athantor
 */

#include "Graph.hh"

Graph::Graph( QWidget *pnt, QWidget * bud, Graph_dir gd, QPen * qp, QBrush * qb ) :
	QFrame(pnt), mydir(gd), buddy(bud), mypen(qp), mybrush(qb)
{
}

Graph::Graph( const Graph& gr ) :
	QFrame(gr.parentWidget()), mydir(gr.Get_gdir()), buddy(gr.buddy), data(new QVector<double> (*gr.data)),
			mypen(new QPen(*gr.mypen)), mybrush(new QBrush(*gr.mybrush))
{
}

Graph& Graph::operator=( const Graph& gr )
{
	if(&gr == this)
		return *this;

	setParent(gr.parentWidget());
	mydir = gr.Get_gdir();
	buddy = gr.buddy;
	data.reset(new QVector<double> (*gr.data));
	mypen.reset(new QPen(*gr.mypen));
	mybrush.reset(new QBrush(*gr.mybrush));

	return *this;
}

Graph::~Graph()
{
}

void Graph::paintEvent( QPaintEvent * )
{

	QPainter pnr(this);

	pnr.setBrush(Get_brush());
	pnr.setPen(Get_pen());
	pnr.setBackground(Get_brush());
	size_t max = 0;
	int8_t mod = 1;
	double idxmod = 1;

	if(static_cast<bool> (data) and data->size() > 0)
	{
		pnr.save();

		switch(mydir)
		{
			case LTR:
				pnr.rotate(-90);
				pnr.scale(height() / static_cast<double> (buddy->height()), width()
						/ static_cast<double> (buddy->width()));
				max = buddy->width();
				idxmod = buddy->height() / static_cast<double> (data->size());
				mod = -1;
				break;
			case UTD:
				pnr.scale(width() / static_cast<double> (buddy->width()), height()
						/ static_cast<double> (buddy->height()));
				max = buddy->height();
				idxmod = buddy->width() / static_cast<double> (data->size());
				mod = 1;
				break;
			default:
				break;
		}

		//	std::transform(data->begin(), data->end(), data->begin(), std::ptr_fun(static_cast<double(*)(double)>(std::fabs)) );
		const double maxgrad = max / (*std::max_element(data->begin(), data->end()));

		for(int64_t idx = 0; idx < data-> size(); ++idx)
		{
			pnr.drawLine(static_cast<int64_t> (idx * mod * idxmod), 0, static_cast<int64_t> (idx * mod
					* idxmod), static_cast<int64_t> (((*data)[idx] * maxgrad)));
		}

		pnr.restore();
	}
	pnr.end();

}

const QVector<double>& Graph::Get_data() const
{
	return *data;
}

const QBrush& Graph::Get_brush() const
{
	return *mybrush;
}

Graph::Graph_dir Graph::Get_gdir() const
{
	return mydir;
}

const QPen& Graph::Get_pen() const
{
	return *mypen;
}

void Graph::Set_data( QVector<double> * d )
{
	data.reset(d);
}

void Graph::Set_brush( const QBrush& b )
{
	mybrush.reset(new QBrush(b));
}

void Graph::Set_gdir( Graph_dir mydir )
{
	this->mydir = mydir;
}

void Graph::Set_pen( const QPen& p )
{
	mypen.reset(new QPen(p));
}

QSize Graph::minimumSizeHint() const
{
	switch(Get_gdir())
	{
		case LTR:
			return QSize(15, 5);
		case UTD:
			return QSize(5, 15);
		default:
			return QSize(-1, -1);
	}
}

const QWidget * Graph::Get_buddy() const
{
	return buddy;
}

void Graph::Set_buddy( QWidget * b )
{
	buddy = b;
}
