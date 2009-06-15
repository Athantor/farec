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

Classifier::Classifier( QWidget *p, const QImage& im ) :
	ImgOp(p, im)
{
	cache[___CTL] = im.cacheKey();
	Prep_cache();
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
	FeatExtract::cht_eyeloc_t el = any_cast<decltype( el )> (cache[cht_el]);

	the_segs[_BASE] = segdata_t(new segdata_t::value_type(make_tuple(_BASE, el->get<1> ().x()
			- el->get<0> ().x(), 1.0, el->get<1> (), el->get<0> ())));

#ifdef DEBUG_KRZYS
	std::cout << "ref len: " << the_segs[_BASE]->get<1> () << std::endl;
#endif

	return static_cast<size_t> (the_segs[_BASE]->get<1> ());

}

void Classifier::Prep_cache()
{
	if(not has_type(cht_el))
		cache[cht_el] = FeatExtract(pnt, *myimg).Get_irises_from_cht(ImgData::CHT_CIRCNUM);
	FeatExtract::cht_eyeloc_t el = any_cast<decltype( el )> (cache[cht_el]);

	if(not has_type(vpf_el))
		cache[vpf_el] = FeatExtract(pnt, *myimg).Get_eyes_from_vpf();

	if(not has_type(noseloc))
		cache[noseloc] = FeatExtract(pnt, *myimg).Get_nose_from_grads();

	if(not has_type(mouthloc))
		cache[mouthloc] = FeatExtract(pnt, *myimg).Get_mouth_from_grads();
	FeatExtract::mouthloc_t ml = any_cast<decltype( ml )> (cache[mouthloc]);

	if(not has_type(facereg))
		cache[facereg] = FeatExtract(pnt, *myimg).Get_face_from_grads();
	FeatExtract::region_t freg = any_cast<decltype( freg )> (cache[facereg]);

	if(not has_type(edgem))
		cache[edgem] = FeatExtract(pnt, *myimg).Get_face_edge_at(ml->get<1> ().y(), freg);

	if(not has_type(edgee))
		cache[edgee] = FeatExtract(pnt, *myimg).Get_face_edge_at((el->get<0> ().y() + el->get<1> ().y())
				/ 2.0, freg);

}

void Classifier::Classify()
{
	if(not the_segs.contains(_BASE))
		Make_base_segment_length();

	FeatExtract::vpf_eyeloc_t vel = any_cast<decltype( vel )> (cache[vpf_el]);
	FeatExtract::cht_eyeloc_t el = any_cast<decltype( el )> (cache[cht_el]);
	FeatExtract::noseloc_t nl = any_cast<decltype( nl )> (cache[noseloc]);
	FeatExtract::mouthloc_t ml = any_cast<decltype( ml )> (cache[mouthloc]);
	FeatExtract::region_t freg = any_cast<decltype( freg )> (cache[facereg]);
	FeatExtract::fedge_t fedgee = any_cast<decltype( fedgee )> (cache[edgee]);
	FeatExtract::fedge_t fedgem = any_cast<decltype( fedgem )> (cache[edgem]);

	const QPoint btm(ml->get<1> ().x(), freg->bottom()); //  assumes that horiz half of face is under middle of the mouth
	const QPoint top((el->get<1> ().x() + el->get<0> ().x()) / 2.0, freg->top()); //between the eyes

	//---

	//
	{
		double wdh = (((vel->first[5].x() - vel->first[4].x()) + (vel->second[5].x() - vel->second[4].x()))
				/ 2.0) + (((vel->first[5].y() - vel->first[4].y())
				+ (vel->second[5].y() - vel->second[4].y())) / 2.0);
		the_segs[eye_wdh] = Prep_data(eye_wdh, vel->first[4], vel->first[5], wdh);
	}

	//
	{
		double hgt = ((vel->first[1].y() - vel->first[0].y()) + (vel->second[1].y() - vel->second[0].y()))
				/ 2.0;
		the_segs[eye_hgt] = Prep_data(eye_hgt, vel->first[0], vel->first[1], hgt);
	}

	//
	{
		the_segs[nose_wdh] = Prep_data(nose_wdh, nl->get<0> (), nl->get<1> ());
	}

	//
	{
		the_segs[mouth_wdh] = Prep_data(mouth_wdh, ml->get<0> (), ml->get<2> ());
	}

	//~~
	{
		the_segs[face_wgh_at_mouth] = Prep_data(face_wgh_at_mouth, fedgem->get<0> (), fedgem->get<1> ());
	}

	//
	{
		the_segs[face_wdh_at_eyes] = Prep_data(face_wdh_at_eyes, fedgee->get<0> (), fedgee->get<1> ());
	}

	//~~~
	{
		the_segs[inner_corners_leye_to_reye] = Prep_data(inner_corners_leye_to_reye, vel->first[5],
				vel->second[4]);
	}

	//
	{
		the_segs[outer_corners_leye_to_reye] = Prep_data(outer_corners_leye_to_reye, vel->first[4],
				vel->second[5]);
	}

	// ~~~
	{
		the_segs[ctrleye_to_lnose] = Prep_data(ctrleye_to_lnose, el->get<0> (), nl->get<0> ());
	}
	//
	{
		the_segs[ctrleye_to_rnose] = Prep_data(ctrleye_to_rnose, el->get<0> (), nl->get<1> ());
	}

	//
	{
		the_segs[ctrreye_to_lnose] = Prep_data(ctrreye_to_lnose, el->get<1> (), nl->get<0> ());
	}

	// 
	{
		the_segs[ctrreye_to_rnose] = Prep_data(ctrreye_to_rnose, el->get<1> (), nl->get<1> ());
	}

	// ~~~
	{
		the_segs[ctrleye_to_lmouth] = Prep_data(ctrleye_to_lmouth, el->get<0> (), ml->get<0> ());
	}
	//
	{
		the_segs[ctrleye_to_ctrmouth] = Prep_data(ctrleye_to_ctrmouth, el->get<0> (), ml->get<1> ());
	}
	//
	{
		the_segs[ctrleye_to_rmouth] = Prep_data(ctrleye_to_rmouth, el->get<0> (), ml->get<2> ());
	}
	//
	{
		the_segs[ctrreye_to_lmouth] = Prep_data(ctrreye_to_lmouth, el->get<1> (), ml->get<0> ());
	}
	//
	{
		the_segs[ctrreye_to_ctrmouth] = Prep_data(ctrreye_to_ctrmouth, el->get<1> (), ml->get<1> ());
	}
	//
	{
		the_segs[ctrreye_to_rmouth] = Prep_data(ctrreye_to_rmouth, el->get<1> (), ml->get<2> ());
	}

	//~~~
	{
		the_segs[ctrleye_to_lfacee] = Prep_data(ctrleye_to_lfacee, el->get<0> (), fedgee->get<0> ());
	}
	//
	{
		the_segs[ctrleye_to_rfacee] = Prep_data(ctrleye_to_rfacee, el->get<0> (), fedgee->get<1> ());
	}
	//
	{
		the_segs[ctrleye_to_lfacem] = Prep_data(ctrleye_to_lfacem, el->get<0> (), fedgem->get<0> ());
	}
	//
	{
		the_segs[ctrleye_to_rfacem] = Prep_data(ctrleye_to_rfacem, el->get<0> (), fedgem->get<1> ());
	}
	//
	{
		the_segs[ctrreye_to_lfacee] = Prep_data(ctrreye_to_lfacee, el->get<1> (), fedgee->get<0> ());
	}
	//
	{
		the_segs[ctrreye_to_rfacee] = Prep_data(ctrreye_to_rfacee, el->get<1> (), fedgee->get<1> ());
	}
	//
	{
		the_segs[ctrreye_to_lfacem] = Prep_data(ctrreye_to_lfacem, el->get<1> (), fedgem->get<0> ());
	}
	//
	{
		the_segs[ctrreye_to_rfacem] = Prep_data(ctrreye_to_rfacem, el->get<1> (), fedgem->get<1> ());
	}

	//~~~
	{
		the_segs[ctrleye_to_face_btm] = Prep_data(ctrleye_to_face_btm, el->get<0> (), btm);
	}
	//
	{
		the_segs[ctrreye_to_face_btm] = Prep_data(ctrreye_to_face_btm, el->get<1> (), btm);
	}
	//~~~
	{
		the_segs[lnose_to_lmouth] = Prep_data(lnose_to_lmouth, nl->get<0> (), ml->get<0> ());
	}
	//
	{
		the_segs[lnose_to_ctrmouth] = Prep_data(lnose_to_ctrmouth, nl->get<0> (), ml->get<1> ());
	}
	//
	{
		the_segs[lnose_to_rmouth] = Prep_data(lnose_to_rmouth, nl->get<0> (), ml->get<2> ());
	}
	//
	{
		the_segs[rnose_to_lmouth] = Prep_data(rnose_to_lmouth, nl->get<1> (), ml->get<0> ());
	}
	//
	{
		the_segs[rnose_to_ctrmouth] = Prep_data(rnose_to_ctrmouth, nl->get<1> (), ml->get<1> ());
	}
	//
	{
		the_segs[rnose_to_rmouth] = Prep_data(rnose_to_rmouth, nl->get<1> (), ml->get<2> ());
	}

	//~~~
	{
		the_segs[lnose_to_lfacem] = Prep_data(lnose_to_lfacem, nl->get<0> (), fedgem->get<0> ());
	}
	//
	{
		the_segs[lnose_to_rfacem] = Prep_data(lnose_to_rfacem, nl->get<0> (), fedgem->get<1> ());
	}
	//
	{
		the_segs[rnose_to_lfacem] = Prep_data(rnose_to_lfacem, nl->get<1> (), fedgem->get<0> ());
	}
	//
	{
		the_segs[rnose_to_rfacem] = Prep_data(rnose_to_rfacem, nl->get<1> (), fedgem->get<1> ());
	}
	//
	{
		the_segs[lnose_to_rfacee] = Prep_data(lnose_to_rfacee, nl->get<0> (), fedgee->get<1> ());
	}
	//
	{
		the_segs[rnose_to_lfacee] = Prep_data(rnose_to_lfacee, nl->get<1> (), fedgee->get<0> ());
	}
	//
	{
		the_segs[rnose_to_rfacee] = Prep_data(rnose_to_rfacee, nl->get<1> (), fedgee->get<1> ());
	}
	//~~~
	{
		the_segs[ctrmouth_to_face_btm] = Prep_data(ctrmouth_to_face_btm, ml->get<1> (), btm);
	}
	//
	{
		the_segs[ctrmouth_to_lfacee] = Prep_data(ctrmouth_to_lfacee, ml->get<1> (), fedgee->get<0> ());
	}
	//
	{
		the_segs[ctrmouth_to_rfacee] = Prep_data(ctrmouth_to_rfacee, ml->get<1> (), fedgee->get<1> ());
	}
	//
	{
		the_segs[ctrmouth_to_rfacem] = Prep_data(ctrmouth_to_rfacem, ml->get<1> (), fedgem->get<1> ());
	}
	//
	{
		the_segs[ctrmouth_to_lfacem] = Prep_data(ctrmouth_to_lfacem, ml->get<1> (), fedgem->get<0> ());
	}
	//~~~
	{
		the_segs[lfacem_to_btm] = Prep_data(lfacem_to_btm, fedgem->get<0> (), btm);
	}
	//
	{
		the_segs[rfacem_to_btm] = Prep_data(rfacem_to_btm, fedgem->get<1> (), btm);
	}

	//~~
	{
		the_segs[lfacee_to_btm] = Prep_data(lfacee_to_btm, fedgee->get<0> (), btm);
	}
	//
	{
		the_segs[rfacee_to_btm] = Prep_data(rfacee_to_btm, fedgee->get<1> (), btm);
	}

	//~~
	{
		the_segs[lfacee_to_lfacem] = Prep_data(lfacee_to_lfacem, fedgee->get<0> (), fedgem->get<0> ());
	}
	//
	{
		the_segs[rfacee_to_rfacem] = Prep_data(rfacee_to_rfacem, fedgee->get<1> (), fedgem->get<1> ());
	}
	//
	{
		the_segs[lfacee_to_rfacem] = Prep_data(lfacee_to_rfacem, fedgee->get<0> (), fedgem->get<1> ());
	}
	//
	{
		the_segs[rfacee_to_lfacem] = Prep_data(rfacee_to_lfacem, fedgee->get<0> (), fedgem->get<0> ());
	}
	//~~~
	{
		the_segs[lfacee_to_top] = Prep_data(lfacee_to_top, fedgee->get<0> (), top);
	}
	//
	{
		the_segs[rfacee_to_top] = Prep_data(rfacee_to_top, fedgee->get<1> (), top);
	}
	//
	{
		the_segs[lfacem_to_top] = Prep_data(lfacem_to_top, fedgem->get<0> (), top);
	}
	//
	{
		the_segs[rfacem_to_top] = Prep_data(rfacem_to_top, fedgem->get<1> (), top);
	}
	//~~
	{
		the_segs[top_to_btm] = Prep_data(top_to_btm, btm, top);
	}
	//~~
	{
		the_segs[ctrleye_to_top] = Prep_data(ctrleye_to_top, el->get<0> (), top);
	}
	//
	{
		the_segs[top_to_btm] = Prep_data(top_to_btm, el->get<0> (), top);
	}

	//~~
	{
		the_segs[ctrmouth_to_top] = Prep_data(ctrmouth_to_top, ml->get<1> (), top);
	}

	//~~
	{
		the_segs[lnose_to_top] = Prep_data(lnose_to_top, nl->get<0> (), top);
	}
	//
	{
		the_segs[rnose_to_top] = Prep_data(rnose_to_top, nl->get<1> (), top);
	}

}

Classifier::segdata_t Classifier::Prep_data( segments sn, const QPoint& start, const QPoint& end, double val,
		double wgt ) const
{
	segdata_t ret = segdata_t(new segdata_t::value_type());

	ret->get<0> () = sn;
	if(val == DBL_MAX)
	{
		//ret->get<1> () = ((end.x() - start.x()) + (end.y() - start.y())) / the_segs[_BASE]->get<1> ();
		ret->get<1> () = veclen(start, end) / the_segs[_BASE]->get<1> ();
	}
	else
	{
		ret->get<1> () = val / the_segs[_BASE]->get<1> ();
	}
	ret->get<2> () = wgt;
	ret->get<3> () = start;
	ret->get<4> () = end;

#ifdef DEBUG_KRZYS
	std::cout << sn << ": " << std::fixed << std::setprecision(24) << ret->get<1> () << std::endl;
#endif

	return ret;
}
