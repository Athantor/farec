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
 * ImgPrep.hh
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#ifndef IMGPREP_HH_
#define IMGPREP_HH_

#include <QImage>
#include <QColor>
#include <QWidget>
#include <QProgressDialog>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>

#include <cmath>
#include <cfloat>
#include <numeric>

#include "ImgOp.hh"
#include "ImgData.hh"
#include "except/FEInvalidParameter.hh"

using boost::shared_ptr;
using boost::scoped_ptr;

class ImgPrep : public ImgOp
{
	public:

		ImgPrep( QWidget *, const QImage & );
		virtual ~ImgPrep();

		ret_t Batch_prepare(bool) const;
		
		//color
		ret_t To_gray() const;
		ret_t Otsu_bin() const;
		ret_t Contrast(int8_t) const;
		
		//details
		ret_t Average_bin_blur(double) const;
		ret_t Gaussian_blur(uint8_t) const;
		ret_t Median_filter() const;
		
		//edge det
		ret_t Sobel_ed() const;
	private:
		shared_ptr<QVector<uint8_t> > Make_LUT(int8_t) const;

};

#endif /* IMGPREP_HH_ */
