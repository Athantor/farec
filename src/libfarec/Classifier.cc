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
 * Classifier.cc
 *
 *  Created on: 2009-06-14
 *      Author: athantor
 */

#include "Classifier.hh"

#include <iostream>

Classifier::Classifier( QWidget *p, const QImage& im ) :
	ImgOp(p, im)
{
	cache[___CTL] = im.cacheKey();
}

Classifier::~Classifier()
{
}

const Classifier::segments_t& Classifier::Get_segs() const
{
	return the_segs;
}

size_t Classifier::Make_base_segment_length()
{
	FeatExtract::cht_eyeloc_t el;

	if(has_type(cht_el))
	{
		el = any_cast<decltype( el )> (cache[cht_el]);
	}
	else
	{
		el = FeatExtract(pnt, *myimg).Get_irises_from_cht(ImgData::CHT_CIRCNUM);
		cache[cht_el] = el;
	}

	the_segs[_BASE] = segdata_t(new segdata_t::value_type(make_tuple(_BASE, el->get<1> ().x()
			- el->get<0> ().x(), 1.0, el->get<1> (), el->get<0> ())));

	std::cout << the_segs[_BASE]->get<1>() << std::endl; 
	
	return static_cast<size_t>(the_segs[_BASE]->get<1>()) ;
	
}
