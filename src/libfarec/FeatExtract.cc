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
 * FeatExtract.cc
 *
 *  Created on: 2009-06-03
 *      Author: athantor
 */

#include "FeatExtract.hh"

#include <iostream>

FeatExtract::FeatExtract( QWidget * p, const QImage& im ) :
	ImgOp(p, im)
{
}

FeatExtract::~FeatExtract()
{
}

FeatExtract::region_t FeatExtract::Get_face_from_grads()
{
	QVector<uint64_t> gx(myimg->height(), 0), gy(myimg->width(), 0);
	region_t ret = region_t(new region_t::value_type);

	cops->Start_processing("Wyszukiwanie twarzy", myimg -> height() * myimg -> width());

	ImgData::gradret_t grd = ImgData(pnt, *ImgPrep(pnt, *myimg).Batch_prepare(false)).Make_gradients();
	//size_t pts[4] = { 0, 0, 0, 0 }; //top, bottom, left, right

	for(size_t y = 0; y < static_cast<decltype( y )> (myimg -> height()); ++y)
	{
		for(size_t x = 0; x < static_cast<decltype( x )> (myimg -> width()); ++x)
		{
			gx[y] += ImgData::norm_rgb_val((*grd.get<0> ())[x][y]);
			gy[x] += ImgData::norm_rgb_val((*grd.get<1> ())[x][y]);
		}
	}

	{ // x block
		auto stit = gx.begin() + 5;
		ret->setTop(std::distance(stit, std::max_element(stit, stit + (myimg -> height() / 6))));

		stit = gx.begin() + ((myimg -> height() / 6) * 5);
		ret->setBottom(std::distance(stit, std::max_element(stit, gx.end() - 5)) + std::distance(gx.begin(),
				stit));
	}

	{ //y block
		auto stit = gy.begin() + 3;
		ret->setLeft(std::distance(stit, std::max_element(stit, gy.begin() + (myimg -> width() / 5))));

		stit = gy.begin() + ((myimg -> width() / 5) * 3);
		ret->setRight(std::distance(stit, std::max_element(stit, gy.end() - 3)) + std::distance(gy.begin(),
				stit));
	}

	return ret;
}

FeatExtract::cht_eyeloc_t FeatExtract::Get_eyes_from_cht()
{

	typedef QList<tuple<QRegion, uint64_t, uint64_t, uint64_t> > buf_t; //reg, x, y, ctr

	region_t facereg = Get_face_from_grads();

	QImage tmpi = ImgPrep(pnt, *myimg).Batch_prepare(true)->copy(facereg->top(), facereg->left(), facereg->width(), facereg->height() / 2);

	const size_t RAD = tmpi.width() / 20;

	ImgData::houghret_t ht = ImgData(pnt, tmpi).Hough_tm(RAD, 100);
	buf_t buf;

	for(auto it = ht->begin(); it != ht->end(); ++it)
	{
		bool hasit = false;
		auto hit = buf.begin();
		for(; hit != buf.end(); ++hit)
		{
			if(hit->get<0> ().contains(it->first))
			{
				hasit = true;
				break;
			}
		}

		if(hasit)
		{
			hit->get<1> () += it->first.x();
			hit->get<2> () += it->first.y();
			hit->get<3> ()++;
		}
		else
		{
			QPoint ctr = it->first;
			const size_t off = (RAD * 1.5) * M_SQRT2; // R
			const size_t alen = std::hypot(static_cast<double> (off), static_cast<double> (off)); // a; Pythagorean theorem

			buf.push_back(make_tuple(QRegion(ctr.x() - off, ctr.y() - off, alen, alen, QRegion::Ellipse),
					ctr.x(), ctr.y(), 1));
		}
	}

	size_t pb1 = 0, pb2 = 0, pctr = 0;
	for(auto it = buf.begin(); it != buf.end(); ++it)
	{
		const uint64_t val = it->get<3> ();
		if(val > pb2 and val > pb1)
		{
			pb1 = pctr;
		}
		else if(val < pb1 and val > pb2)
		{
			pb2 = pctr;
		}

		++pctr;
	}

	//makes pair with averaged 2 first max els
	cht_eyeloc_t ret = cht_eyeloc_t(new cht_eyeloc_t::value_type(make_tuple(QPoint(buf[pb1].get<1> ()
			/ buf[pb1].get<3> (), buf[pb1].get<2> () / buf[pb1].get<3> ()), QPoint(buf[pb2].get<1> ()
			/ buf[pb2].get<3> (), buf[pb2].get<2> () / buf[pb2].get<3> ()), RAD)) );
	
	//map to global
	ret->get<0>() += QPoint( facereg->left() , facereg->top());
	ret->get<1>() += QPoint( facereg->left() , facereg->top());;
	
	
	return ret;
}
