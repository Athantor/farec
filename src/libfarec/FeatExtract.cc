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

#include <iostream>

#include "FeatExtract.hh"

FeatExtract::FeatExtract( QWidget * p, const QImage& im ) :
	ImgOp(p, im)
{
}

FeatExtract::~FeatExtract()
{
}

FeatExtract::region_t FeatExtract::Get_face_from_grads() const
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

	const size_t XOFF = 3, YOFF = 3;
	
	{ // x block
		auto stit = gx.begin() + XOFF*5;
		ret->setTop(std::distance(stit, std::max_element(stit, stit + static_cast<int>((myimg -> height() / 5.5)))));

		stit = gx.begin() + static_cast<int>(((myimg -> height() / 7) * 6.5));
		ret->setBottom(std::distance(stit, std::max_element(stit, gx.end() - XOFF)) + std::distance(gx.begin(),
				stit));
	}

	{ //y block
		auto stit = gy.begin() + YOFF;
		ret->setLeft(std::distance(stit, std::max_element(stit, gy.begin() + (myimg -> width() / 5))));

		stit = gy.begin() + ((myimg -> width() / 3) * 2);
		ret->setRight(std::distance(stit, std::max_element(stit, gy.end() - YOFF)) + std::distance(gy.begin(),
				stit));
	}

	return ret;
}

FeatExtract::cht_eyeloc_t FeatExtract::Get_irises_from_cht( size_t radsnum ) const
{
	typedef QList<tuple<QRegion, uint64_t, uint64_t, uint64_t> > buf_t; //reg, x, y, ctr

	region_t facereg = Get_face_from_grads();

	QImage tmpi = ImgPrep(pnt, *myimg).Batch_prepare(true)->copy(*facereg);
	ImgData::gradret_t grd =
			ImgData(pnt, ImgPrep(pnt, *myimg).Batch_prepare(false)->copy(*facereg)).Make_gradients();

	QVector<double> gx = QVector<double> (tmpi.height(), 0.0);
	uint64_t avg = 0;
	for(int y = 0; y < tmpi.height(); ++y)
	{
		for(int x = 0; x < tmpi.width(); ++x)
		{
			gx[y] += ImgData::norm_rgb_val((*grd.get<0> ())[x][y]);
			avg += gx[y];
		}
	}

	std::fill(gx.begin(), gx.begin() + 3, 0);
	std::fill(gx.end() - 3, gx.end(), 0);

	const size_t maxgradthd = avg / (tmpi.height() * tmpi.width() * 1.0) * 2.5;

	//eye width is equal to five times of face width, and height is equal to ≈⅓ of its width;
	const size_t RAD = round((0.305 * (facereg -> width() / 5.0))
			* (static_cast<double> (facereg ->width()) / facereg ->height()));
	const float CLEARRAD = 4.5;

	ImgData::houghret_t ht = ImgData(pnt, tmpi).Hough_tm(RAD, radsnum);
	buf_t buf;

	for(auto it = ht->begin(); it != ht->end(); ++it)
	{
		if((it->first.y()) <= ((facereg -> height() / 5.0) * 3) and (it->first.y()) >= ((facereg -> height()
				/ 5.0) * 1)) // if circle is in upper half of face region
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
				const size_t alen = hypot(static_cast<double> (off), static_cast<double> (off)); // a; Pythagorean theorem

				buf.push_back(make_tuple(QRegion(ctr.x() - off, ctr.y() - off, alen, alen, QRegion::Ellipse),
						ctr.x() * mod, ctr.y() * mod, mod));
			}
		}
	}

	if(buf.size() < 1)
		throw FENoData(
				QString("Amount of circles found by CHT is not sufficient: %1").arg(buf.size()).toStdString());

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
			/ buf[pb2].get<3> (), buf[pb2].get<2> () / buf[pb2].get<3> ()), RAD)));
	
	//make sure 0 has the left eye
	if(ret->get<0> ().x() > ret->get<1> ().x())
		std::swap(ret->get<1> (), ret->get<0> ());

	if( (buf.size() < 2) or (buf[0].get<0>().contains( QPoint( buf[1].get<1>(), buf[1].get<2>())) ) ) 
	{
		ret->get<1>() = QPoint( facereg->width() - ret->get<0>().x() , ret->get<0>().y() );
	}
	
	//map to global
	ret->get<0> () += QPoint(facereg->left(), facereg->top());
	ret->get<1> () += QPoint(facereg->left(), facereg->top());

	return ret;
}

FeatExtract::eyewin_t FeatExtract::Make_eye_windows( size_t cn ) const
{
	cht_eyeloc_t el;
	try
	{
		el = Get_irises_from_cht(cn);
	}
	catch(const FENoData &ex)
	{
		el = Get_irises_from_cht(cn * 2);
	}

	const int16_t esize = el->get<2> () * 3.3;
	const int16_t evsize = el->get<2> () * 1.6;

	eyewin_t ret = eyewin_t(new eyewin_t::value_type());
	*ret = make_tuple(QRect(el->get<0> () + QPoint(-esize, -evsize), el->get<0> () + QPoint(esize, evsize)),
			QRect(el->get<1> () + QPoint(-esize, -evsize), el->get<1> () + QPoint(esize, evsize)), el);

	return ret;
}

FeatExtract::vpf_eyeloc_t FeatExtract::Get_eyes_from_vpf( int32_t vic, size_t circs ) const
{
	vpf_eyeloc_t ret = vpf_eyeloc_t(new vpf_eyeloc_t::value_type(vpf_eyeloc_t::value_type::first_type(),
			vpf_eyeloc_t::value_type::second_type()));

	eyewin_t ew = Make_eye_windows(circs);
	cht_eyeloc_t el = ew->get<2> ();

	size_t srchrng = 1;
	if(vic < 0)
	{
		srchrng = round(el->get<2> () / 3.5);
	}
	else
	{
		srchrng = vic;
	}

	ImgData id = ImgData(pnt, *ImgPrep(pnt, *ImgPrep(pnt, *myimg).To_gray()).Gaussian_blur(9));
	ImgData::Vpf_t vpf[2][2] = { { id.Vpf(ew->get<0> (), ImgData::Vpf_dir::HOR), id.Vpf(ew->get<0> (),
			ImgData::Vpf_dir::VERT) }, { id.Vpf(ew->get<1> (), ImgData::Vpf_dir::HOR), id.Vpf(ew->get<1> (),
			ImgData::Vpf_dir::VERT) } };

	Perform_vpf_search(ret->first, Make_approx_eye_pts(el->get<0> (), el->get<2> ()), vpf[0], srchrng,
			ew->get<0> ());
	Perform_vpf_search(ret->second, Make_approx_eye_pts(el->get<1> (), el->get<2> ()), vpf[1], srchrng,
			ew->get<0> ());

	return ret;
}

int32_t FeatExtract::Vpf_search( const QPoint & cnt, size_t vic, ImgData::Vpf_t vpf, int32_t off ) const
{
	ImgData::Vpf_dir vdir = vpf->get<1> ();

	if(vdir != ImgData::Vpf_dir::HOR and vdir != ImgData::Vpf_dir::VERT)
		throw FEInvalidParameter(__PRETTY_FUNCTION__ + std::string(": Unknown direction"));

	ImgData::Vpf_critpnt_t cpnts = vpf->get<5> ();

	QPoint apx(cnt);
	int32_t theval = (vdir == ImgData::Vpf_dir::HOR) ? apx.ry() : apx.rx();

	QVector<ImgData::Vpf_critpnt_t::value_type::value_type> tmp;

	for(auto it = cpnts->begin(); it != cpnts->end(); ++it)
	{
		auto val = (*it + off);
		if((val >= ((theval) - vic)) and ((val <= ((theval) + vic))))
		{
			tmp.push_back(val);
		}
	}

	decltype(tmp) tmp1(tmp.size());

	//substracts approx coord from critical point's
	std::transform(tmp.begin(), tmp.end(), tmp1.begin(), std::bind2nd(std::minus<
			ImgData::Vpf_critpnt_t::value_type::value_type>(),
			static_cast<ImgData::Vpf_critpnt_t::value_type::value_type> (theval)));
	//makes absolute values
	std::transform(tmp1.begin(), tmp1.end(), tmp1.begin(), std::ptr_fun(
			static_cast<long int(*)( long int )> (std::abs)));
	//sort to take smallest difference from approx
	std::sort(tmp1.begin(), tmp1.end());

	//take the smallest diff and check if it's present in crit points, converting it back to coords
	if(tmp.size() > 0 and tmp.contains((theval) + tmp1[0]))
	{
		return (theval) + tmp1[0];
	}
	else if(tmp.size() > 0 and tmp.contains((theval) - tmp1[0]))
	{
		return theval - tmp1[0];
	}
	else
	{
		//if not, return approx coord
		return theval;
	}
}

shared_ptr<FeatExtract::eyeloc_t> FeatExtract::Make_approx_eye_pts( const QPoint& ctr, size_t rad ) const
{
	shared_ptr<eyeloc_t> ret = decltype(ret)(new eyeloc_t());

	ret->push_back(QPoint(ctr.x(), ctr.y() - rad)); //top
	ret->push_back(QPoint(ctr.x(), ctr.y() + rad)); //btm
	ret->push_back(QPoint(ctr.x() - rad, ctr.y())); //i left
	ret->push_back(QPoint(ctr.x() + rad, ctr.y())); //i right
	ret->push_back(QPoint(ctr.x() - rad * 2, ctr.y())); //e left
	ret->push_back(QPoint(ctr.x() + 2 * rad, ctr.y())); //e left

	return ret;
}

void FeatExtract::Perform_vpf_search( eyeloc_t& dst, shared_ptr<eyeloc_t> src, ImgData::Vpf_t* vpf,
		size_t vic, const eyewin_t::value_type::head_type& reg ) const
{

	int32_t offt = reg.top();
	int32_t offl = reg.left();

	dst.push_back(QPoint(src->at(0).x(), Vpf_search(src->at(0), vic, vpf[0], offt)));
	dst.push_back(QPoint(src->at(1).x(), Vpf_search(src->at(1), vic, vpf[0], offt)));
	dst.push_back(QPoint(Vpf_search(src->at(2), vic, vpf[1], offl), src->at(2).y()));
	dst.push_back(QPoint(Vpf_search(src->at(3), vic, vpf[1], offl), src->at(3).y()));
	dst.push_back(QPoint(Vpf_search(src->at(4), vic, vpf[1], offl), src->at(4).y()));
	dst.push_back(QPoint(Vpf_search(src->at(5), vic, vpf[1], offl), src->at(5).y()));
}

FeatExtract::region_t FeatExtract::Get_nostrils( region_t tfr, vpf_eyeloc_t tvel ) const
{
	decltype(tfr) fr(tfr);
	decltype(tvel) vel(tvel);

	if(not tfr)
	{
		fr = Get_face_from_grads();
	}

	if(not tvel)
	{
		vel = Get_eyes_from_vpf();
	}

	uint32_t hhgt = (vel->first.at(5).x() - vel->first.at(4).x()) * 7; //head height is equal to 7 widths of the eye

	region_t ret = region_t(new region_t::value_type(QPoint(vel->first.at(3).x(), vel->first.at(1).y()
			+ ((hhgt / 5.0) * 0.7)), QPoint(vel->second.at(2).x(), vel->first.at(1).y() + ((hhgt / 5.0)
			* 1.65))));
	return ret;
}

FeatExtract::noseloc_t FeatExtract::Get_nose_from_grads() const
{
	noseloc_t ret = FeatExtract::noseloc_t(new FeatExtract::noseloc_t::value_type());

	region_t reg = Get_face_from_grads();
	vpf_eyeloc_t vel = Get_eyes_from_vpf();
	region_t nreg = Get_nostrils(reg, vel);

	QImage noseimg(*ImgPrep(pnt, myimg->copy(*nreg)).Batch_prepare(true));
	ImgData::dirgrads_t dg = ImgData(pnt, noseimg).Make_directional_gradients();

	size_t tmp_y = std::distance(dg->get<0> ()->begin(), std::max_element(dg->get<0> ()->begin(),
			dg->get<0> ()->end()));
	/*size_t tmp_lx = std::distance(dg->get<1>()->begin(), std::max_element(dg->get<1>()->begin(), dg->get<1>()->begin() + (noseimg.width() / 2) ) );
	 size_t tmp_rx = std::distance(dg->get<1>()->begin() + (noseimg.width() / 2), std::max_element(dg->get<1>()->begin() + (noseimg.width() / 2), dg->get<1>()->end() ) );*/

	QRgb * px = reinterpret_cast<decltype( px )> (noseimg . scanLine(tmp_y));
	size_t tmp_lx = 0, tmp_rx = (noseimg.width());
	for(; tmp_lx < static_cast<decltype( tmp_lx )> ((noseimg.width() / 2)); ++tmp_lx)
	{
		if(qRed(px[tmp_lx]) != 255)
			break;
	}

	for(; tmp_rx > static_cast<decltype( tmp_rx )> ((noseimg.width() / 2)); --tmp_rx)
	{
		if(qRed(px[tmp_rx]) != 255)
			break;
	}

	ret->get<0> () = QPoint(nreg->x() + tmp_lx, nreg->y() + tmp_y);
	ret->get<1> () = QPoint(nreg->x() + tmp_rx, nreg->y() + tmp_y);
	return ret;
}

FeatExtract::region_t FeatExtract::Get_mouth( region_t tfr, region_t tnr, vpf_eyeloc_t tvel ) const
{
	decltype(tfr) fr(tfr);
	decltype(tnr) nr(tnr);
	decltype(tvel) vel(tvel);

	if(not tfr)
	{
		fr = Get_face_from_grads();
	}

	if(not tvel)
	{
		vel = Get_eyes_from_vpf();
	}

	if(not nr)
	{
		nr = Get_nostrils(fr, vel);
	}

	const size_t EWTH = vel->first.at(5).x() - vel->first.at(4).x();

	QImage mimg(*ImgPrep(pnt, myimg->copy(QRect(fr->left(), nr->top()+ (EWTH /2.0) , fr->width(), ( fr->bottom() - (EWTH /2.0)) - (nr->top()+ (EWTH /2.0))
			- 3))).To_gray());
	ImgData::dirgrads_t dg = ImgData(pnt, mimg).Make_directional_gradients();

	size_t y = nr->top() + std::distance(dg->get<0> ()->begin(), std::max_element(dg->get<0> ()->begin(),
			dg->get<0> ()->end())) + (EWTH /2.0);

	region_t ret = region_t(new region_t::value_type(QPoint(vel->first.at(3).x(), y - EWTH / 1.9), QPoint(
			vel->second.at(2).x(), y + EWTH / 1.5)));
	return ret;
}

FeatExtract::mouthloc_t FeatExtract::Get_mouth_from_grads() const
{
	mouthloc_t ret = mouthloc_t(new mouthloc_t::value_type);

	region_t reg = Get_face_from_grads();
	vpf_eyeloc_t vel = Get_eyes_from_vpf();
	region_t nreg = Get_nostrils(reg, vel);

	region_t mreg = Get_mouth(reg, nreg, vel);
	size_t midy = mreg-> top() + ((vel->first.at(5).x() - vel->first.at(4).x()) / 1.9);

	QImage mimg(*ImgPrep(pnt, myimg->copy(*mreg)).To_gray());
	ImgData::dirgrads_t dg = ImgData(pnt, mimg).Make_directional_gradients();

	size_t left = std::distance(dg->get<1> ()->begin(), std::max_element(dg->get<1> ()->begin(),
			dg->get<1> ()->begin() + (mreg->width() / 2)));
	size_t right = std::distance(dg->get<1> ()->begin() + (mreg->width() / 2), std::max_element(
			dg->get<1> ()->begin() + (mreg->width() / 2), dg->get<1> ()->end()));

	size_t nleft = mreg->left() + left;
	size_t nright = mreg->left() + reg->left() + (mreg->width() / 2) + right;

	ret->get<0> () = QPoint(nleft, midy);
	ret->get<1> () = QPoint(nleft + ((nright - nleft) / 2.0), midy);
	ret->get<2> () = QPoint(nright, midy);

	return ret;
}

FeatExtract::fedge_t FeatExtract::Get_face_edge_at(int32_t y, region_t fr) const
{
	fedge_t ret = fedge_t(new fedge_t::value_type());
	decltype(fr) freg(fr);
	
	if(not freg)
		freg = Get_face_from_grads();

	QImage etmpi( *ImgPrep(pnt, myimg->copy(freg->left(), y-5, freg->width(), 5) ).Batch_prepare(true) );
	
	int32_t left = -1, right = -1;
	QRgb * px = reinterpret_cast<decltype( px )> (etmpi . scanLine(3));
	
	for(int32_t i = 0; i < etmpi.width()/10; ++i)
	{
		if(qRed(px[i]) == 0)
		{
			left = i;
			break;
		}
	}
	if(left < 0)
	{
		//throw FENoData(QString::fromUtf8("Can't find left face edge @y = %1").arg(y).toStdString());
		left = freg->left();
	}
	
	for(int32_t i = etmpi.width(); i > etmpi.width()/10 * 9; --i)
	{
		if(qRed(px[i]) == 0)
		{
			right = i;
			break;
		}
	}
	
	if(right < 0)
	{
		//throw FENoData(QString::fromUtf8("Can't find right face edge @y = %1").arg(y).toStdString());
		right = freg->width();
	}
	
	ret->get<0>() = QPoint(freg->left() + left, y);
	ret->get<1>() = QPoint(freg->left() + right, y);
	
	return ret;
}
