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
 * Classifier.hh
 *
 *  Created on: 2009-06-14
 *      Author: athantor
 */

#ifndef CLASSIFIER_HH_
#define CLASSIFIER_HH_

#include <QImage>
#include <QHash>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>

#include <cfloat>

#ifdef DEBUG_KRZYS
#include <iostream>
#include <iomanip>
#endif

#include "ImgOp.hh"
#include "FeatExtract.hh"

using boost::any;
using boost::any_cast;
using boost::tuple;
using boost::make_tuple;

class Classifier : private ImgOp
{
	public:

		enum segments
		{
			_BASE = 0,
			eye_wdh,
			eye_hgt,
			nose_wdh,
			mouth_wdh,

			face_wgh_at_mouth,
			face_wdh_at_eyes,

			inner_corners_leye_to_reye,
			outer_corners_leye_to_reye,

			ctrleye_to_lnose,
			ctrleye_to_rnose,
			ctrreye_to_lnose,
			ctrreye_to_rnose,

			ctrleye_to_lmouth,
			ctrleye_to_ctrmouth,
			ctrleye_to_rmouth,
			ctrreye_to_lmouth,
			ctrreye_to_ctrmouth,
			ctrreye_to_rmouth,
			
			ctrleye_to_lfacee,
			ctrleye_to_rfacee,
			ctrleye_to_lfacem,
			ctrleye_to_rfacem,
			ctrreye_to_lfacee,
			ctrreye_to_rfacee,
			ctrreye_to_lfacem,
			ctrreye_to_rfacem,
			
			ctrleye_to_face_btm,
			ctrreye_to_face_btm,
			
			lnose_to_lmouth,
			lnose_to_ctrmouth,
			lnose_to_rmouth,
			rnose_to_lmouth,
			rnose_to_ctrmouth,
			rnose_to_rmouth,
			
			lnose_to_lfacem,
			lnose_to_rfacem,
			rnose_to_lfacem,
			rnose_to_rfacem,
			lnose_to_rfacee,
			rnose_to_lfacee,
			rnose_to_rfacee,
			
			
			ctrmouth_to_face_btm,
			ctrmouth_to_lfacee,
			ctrmouth_to_rfacee,
			ctrmouth_to_rfacem,
			ctrmouth_to_lfacem,
			
			lfacem_to_btm,
			rfacem_to_btm,
			lfacee_to_btm,
			rfacee_to_btm,
			
			lfacee_to_lfacem,
			rfacee_to_rfacem,
			lfacee_to_rfacem,
			rfacee_to_lfacem
		};

		//                       type      ratio   weight   start   end
		typedef shared_ptr<tuple<segments, long double, long double, QPoint, QPoint> > segdata_t;
		typedef QHash<segments, segdata_t> segments_t;

		Classifier( QWidget *p, const QImage& );Classifier() = delete;
		Classifier(const Classifier&) = delete;
		Classifier& operator=(const Classifier&) = delete;
		virtual ~Classifier();

		const segments_t& Get_segs() const;
		void Classify();

		size_t Make_base_segment_length();
	private:
		enum cacheid
		{
			___CTL,
			cht_el,
			vpf,
			vpf_el,
			eyewin,
			nosewin,
			mouthwin,
			noseloc,
			mouthloc,
			facereg,
			grads,
			dirgrads,
			edgee,
			edgem
		};

		segments_t the_segs;

		QHash<cacheid, any> cache;
		inline bool chkcache() const
		{
			if(not cache.contains(___CTL) or not myimg)
				return false;

			qint64 cv = any_cast<decltype( cv )> (cache[___CTL]);
			return cv == myimg->cacheKey();
		}

		inline bool has_type( cacheid ci ) const
		{
			return (chkcache() and cache.contains(ci));
		}

		void Prep_cache();
		segdata_t Prep_data( segments, const QPoint&, const QPoint&, long double = DBL_MAX, long double = 1.0 ) const;
};

#endif /* CLASSIFIER_HH_ */
