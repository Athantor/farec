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

#include <algorithm>
#include <iterator>

#include <boost/shared_ptr.hpp>

#include "ImgOp.hh"
#include "ImgData.hh"
#include "ImgPrep.hh"

using boost::shared_ptr;

class FeatExtract : private ImgOp
{
	public:
		
		typedef shared_ptr<QRect> region_t;  
		
		FeatExtract(QWidget *, const QImage&);
		virtual ~FeatExtract();
		
		region_t Get_face_from_grads();
		
};

#endif /* FEATEXTRACT_HH_ */
