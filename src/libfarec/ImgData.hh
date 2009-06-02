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
 * ImgData.hh
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#ifndef IMGDATA_HH_
#define IMGDATA_HH_

#include <QImage>
#include <QVector>
#include <QList>
#include <QPoint>
#include <QPair>

#include <boost/tuple/tuple.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <cstdint>
#include <algorithm>
#include <list>

#include "ImgOp.hh"

using boost::tuple;
using boost::scoped_ptr;
using boost::shared_ptr;

class ImgData : public ImgOp
{
	public:

		static inline uchar norm_rgb_val( int16_t val )
		{
			if(val >= 255)
				return 255;
			if(val <= 0)
				return 0;

			return val;
		}

		typedef shared_ptr<QVector<QVector<int64_t> > > gradarr_t;
		typedef tuple<gradarr_t, gradarr_t> gradret_t;
		typedef tuple<shared_ptr<QVector<int64_t> > , shared_ptr<QVector<int64_t> > , shared_ptr<QVector<
				int64_t> > , shared_ptr<QVector<int64_t> > > histret_t; ///< RGBA histogram
		typedef shared_ptr<std::list<QPair<QPoint, uint64_t> > > houghret_t; // (x,y), val


		ImgData( QWidget *, const QImage& );
		virtual ~ImgData();

		gradret_t Make_gradients() const;
		histret_t Make_histogram() const;
		
		houghret_t Hough_tm(size_t, size_t);

};

#endif /* IMGDATA_HH_ */
