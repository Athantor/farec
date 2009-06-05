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
 * FeatExtract.hh
 *
 *  Created on: 2009-06-03
 *      Author: athantor
 */

#ifndef FEATEXTRACT_HH_
#define FEATEXTRACT_HH_

#include <QRect>
#include <QVector>
#include <QRegion>
#include <QList>
#include <QHash>

#include <algorithm>
#include <iterator>
#include <cstdint>
#include <cmath>

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "ImgOp.hh"
#include "ImgData.hh"
#include "ImgPrep.hh"

using boost::shared_ptr;
using boost::tuple;
using boost::make_tuple;

class FeatExtract : private ImgOp
{
	public:
		
		typedef shared_ptr<QRect> region_t;
		typedef shared_ptr<tuple<QPoint, QPoint, uint16_t> > cht_eyeloc_t; // left, right, radius
		
		FeatExtract(QWidget *, const QImage&);
		virtual ~FeatExtract();
		
		region_t Get_face_from_grads();
		cht_eyeloc_t Get_eyes_from_cht();
		
		
};

#endif /* FEATEXTRACT_HH_ */
