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
#include <QImage>

#include <algorithm>
#include <iterator>
#include <cstdint>
#include <cmath>
#include <utility>
#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include "ImgOp.hh"
#include "ImgData.hh"
#include "ImgPrep.hh"

#include "except/FENoData.hh"

using boost::shared_ptr;
using boost::tuple;
using boost::make_tuple;

class FeatExtract : private ImgOp
{
	public:
		typedef shared_ptr<QRect> region_t;
		typedef shared_ptr<tuple<QPoint, QPoint, uint16_t> > cht_eyeloc_t; ///< left, right, radius
		typedef QVector<QPoint> eyeloc_t; ///< top, bottom, iris left, iris right, eye left, eye right
		typedef shared_ptr<std::pair<eyeloc_t, eyeloc_t> > vpf_eyeloc_t; ///< left, right
		typedef shared_ptr<tuple<QRect, QRect, cht_eyeloc_t> > eyewin_t; ///<< eye windows + CHT cache
		typedef shared_ptr<tuple<QPoint, QPoint> > noseloc_t;
		typedef shared_ptr<tuple<QPoint, QPoint, QPoint> > mouthloc_t; ///<left mid right

		FeatExtract( QWidget *, const QImage& );
		virtual ~FeatExtract();

		eyewin_t Make_eye_windows( size_t = ImgOp::CHT_CIRCNUM ) const;
		shared_ptr<eyeloc_t> Make_approx_eye_pts( const QPoint&, size_t ) const;

		region_t Get_face_from_grads() const;
		region_t Get_nostrils(region_t = region_t(), vpf_eyeloc_t = vpf_eyeloc_t()) const;
		region_t Get_mouth(region_t = region_t(), region_t = region_t(), vpf_eyeloc_t = vpf_eyeloc_t()) const;
		cht_eyeloc_t Get_irises_from_cht( size_t ) const;
		vpf_eyeloc_t Get_eyes_from_vpf( int32_t = -1, size_t = ImgOp::CHT_CIRCNUM ) const;
		noseloc_t Get_nose_from_grads() const;
		mouthloc_t Get_mouth_from_grads() const;
		

	private:		
		int32_t Vpf_search( const QPoint &, size_t, ImgData::Vpf_t,int32_t ) const;
		void Perform_vpf_search( eyeloc_t&, shared_ptr<eyeloc_t> , ImgData::Vpf_t*, size_t, const eyewin_t::value_type::head_type& ) const;
		

};

#endif /* FEATEXTRACT_HH_ */
