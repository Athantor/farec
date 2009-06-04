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

	ImgData::gradret_t grd = ImgData(pnt, *myimg).Make_gradients();
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
		

		/*std::reverse_iterator<decltype( stit )> revsp(stit), revst(gx.end() - ((myimg -> height() / 6) * 1));
		ret->setBottom(std::distance(revst, std::max_element(revst, revsp)));*/
		stit = gx.begin() + ((myimg -> height() / 6) * 5);
		ret->setBottom(std::distance(stit, std::max_element(stit , gx.end() - 5)) + std::distance(gx.begin(), stit));
	}

	{ //y block
		auto stit = gy.begin() + 3;
		ret->setLeft(std::distance(stit, std::max_element(stit, gy.begin() + (myimg -> width() / 5))));

		/*std::reverse_iterator<decltype( stit )> revsp(stit), revst(gy.end() - ((myimg -> width() / 4) * 1));
		ret->setRight(std::distance(revst, std::max_element(revst, revsp)));*/
		stit = gy.begin() + ((myimg -> width() / 5) * 3);
		ret->setRight(std::distance(stit, std::max_element(stit, gy.end() - 3)) + std::distance(gy.begin(), stit));
	}
	
	return ret;
}
