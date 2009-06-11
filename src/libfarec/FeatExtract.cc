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

		stit = gy.begin() + ((myimg -> width() / 3) * 2);
		ret->setRight(std::distance(stit, std::max_element(stit, gy.end() - 3)) + std::distance(gy.begin(),
				stit));
	}

	return ret;
}

FeatExtract::cht_eyeloc_t FeatExtract::Get_irises_from_cht( size_t radsnum )
{

	typedef QList<tuple<QRegion, uint64_t, uint64_t, uint64_t> > buf_t; //reg, x, y, ctr

	region_t facereg = Get_face_from_grads();

	QImage tmpi = ImgPrep(pnt, *myimg).Batch_prepare(true)->copy(*facereg);
	ImgData::gradret_t grd = ImgData(pnt, ImgPrep(pnt, *myimg).Batch_prepare(false)->copy(*facereg)).Make_gradients();

	QVector<double> gx = QVector<double> (tmpi.height(), 0.0);
	uint64_t avg= 0;
	for(int y = 0; y < tmpi.height(); ++y)
	{
		for(int x = 0; x < tmpi.width(); ++x)
		{
			gx[y] += ImgData::norm_rgb_val((*grd.get<0> ())[x][y]);
			avg +=gx[y]; 
		}
	}

	std::fill(gx.begin(), gx.begin() + 3, 0);
	std::fill(gx.end() - 3, gx.end(), 0);
	
	const size_t maxgradthd = avg / (tmpi.height() *tmpi.width() * 1.0) * 2.5 ;
	
	//eye width is equal to five times of face width, and height is equal to ≈⅓ of its width;  
	const size_t RAD = std::round((0.305 * (facereg -> width() / 5.0)) * (static_cast<double>(facereg ->width()) / facereg ->height()));
	const float CLEARRAD = 2.5;

	ImgData::houghret_t ht = ImgData(pnt, tmpi).Hough_tm(RAD, radsnum);
	buf_t buf;

	for(auto it = ht->begin(); it != ht->end(); ++it)
	{
		if( (it->first.y()) < ( (facereg -> height() / 4.0) * 3) ) // if circle is in upper half of face region
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

			const uint8_t mod = gx[it->first.y()] >= maxgradthd ? 2 : 1;

			if(hasit)
			{
				hit->get<1> () += it->first.x() * mod;
				hit->get<2> () += it->first.y() * mod;
				hit->get<3> () += mod;
			}
			else
			{
				QPoint ctr = it->first;
				const size_t off = (RAD * CLEARRAD) * M_SQRT2; // R
				const size_t alen = std::hypot(static_cast<double> (off), static_cast<double> (off)); // a; Pythagorean theorem

				buf.push_back(make_tuple(QRegion(ctr.x() - off, ctr.y() - off, alen, alen, QRegion::Ellipse),
						ctr.x() * mod, ctr.y() * mod, mod));
			}
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
	
	//make sure 0 has the left eye
	if(ret->get<0>().x() < ret->get<1>().x())
		std::swap(ret->get<1>(), ret->get<0>());
	
	//map to global
	ret->get<0>() += QPoint( facereg->left()  , facereg->top());
	ret->get<1>() += QPoint( facereg->left()  , facereg->top() );
	
	return ret;
}
