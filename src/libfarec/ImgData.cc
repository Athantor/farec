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
 * ImgData.cc
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#include "ImgData.hh"

ImgData::ImgData( QWidget *p, const QImage& im ) :
	ImgOp(p, im)
{
}

ImgData::~ImgData()
{
}

//not so precise
/*ImgData::gradret_t ImgData::Make_gradients_sse() const
 {
 
 const int16_t __attribute__((aligned(16))) Gxx[8] = { 2, 1, 0, 0, 0, -1, -2, -1 }; //1
 const int16_t __attribute__((aligned(16))) Gyy[8] = { 0, 1, -2, 0, 2, -1, 0, 1 }; // -1
 const __m128i GGxx = _mm_load_si128(reinterpret_cast<const __m128i *> (Gxx));
 const __m128i GGyy = _mm_load_si128(reinterpret_cast<const __m128i *> (Gyy));

 cops->Start_processing(QString::fromUtf8("Obliczanie gradientów"), myimg->height() * myimg->width());

 int sumx = 0, sumy = 0;
 gradarr_t gx(new gradarr_t::element_type(myimg->width(), gradarr_t::element_type::value_type(
 myimg->height(), 0))); //pointer to a vector of vectors filled with 0
 gradarr_t gy(new gradarr_t::element_type(myimg->width(), gradarr_t::element_type::value_type(
 myimg->height(), 0))); //pointer to a vector of vectors filled with 0
 size_t ctr = 0;

 for(int y = 0; y < myimg->height(); ++y)
 {
 for(int x = 0; x < myimg->width(); ++x)
 {
 sumx = sumy = 0;

 if((y == 0) or (y + 1 >= myimg -> height()))
 {
 //sum = 0;
 }
 else if((x == 0) or (x + 1 >= myimg -> width()))
 {
 //sum = 0;
 }
 else
 {

 QRgb * px_1 = reinterpret_cast<decltype( px_1 )> (myimg -> scanLine(y - 1));
 QRgb * px_2 = reinterpret_cast<decltype( px_2 )> (myimg -> scanLine(y));
 QRgb * px_3 = reinterpret_cast<decltype( px_3 )> (myimg -> scanLine(y + 1));

 const __m128i px = _mm_set_epi16(qRed(px_3[x]), qRed(px_3[x - 1]), qRed(px_2[x + 1]), qRed(
 px_2[x]), qRed(px_3[x - 1]), qRed(px_1[x + 1]), qRed(px_1[x]), qRed(px_3[x - 1]));

 __m128i pxx = _mm_mullo_epi16(px, GGxx);
 __m128i pxy = _mm_mullo_epi16(px, GGyy);

 int16_t * xvals = reinterpret_cast<decltype( xvals )> (&pxx);
 int16_t * yvals = reinterpret_cast<decltype( xvals )> (&pxy);

 for(size_t i = 0; i < 8; ++i)
 {
 sumx += xvals[i];
 sumy += yvals[i];
 }

 sumx += qRed(px_3[x + 1]) * 1;
 sumy += qRed(px_3[x + 1]) * -1;

 }

 ctr++;

 (*gx)[x][y] = sumx;
 (*gy)[x][y] = sumy;

 if(ctr % 1000 == 0)
 {
 cops->Get_pdialog()->setValue(ctr);
 }
 }

 }

 cops->Stop_processing();

 return boost::make_tuple(gx, gy);
 }*/

ImgData::gradret_t ImgData::Make_gradients() const
{
	const int8_t Gx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
	const int8_t Gy[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	cops->Start_processing(QString::fromUtf8("Obliczanie gradientów"), myimg->height() * myimg->width());

	int sumx = 0, sumy = 0;
	gradarr_t gx(new gradarr_t::element_type(myimg->width(), gradarr_t::element_type::value_type(
			myimg->height(), 0))); //pointer to a vector of vectors filled with 0
	gradarr_t gy(new gradarr_t::element_type(myimg->width(), gradarr_t::element_type::value_type(
			myimg->height(), 0))); //pointer to a vector of vectors filled with 0
	size_t ctr = 0;

	for(int y = 0; y < myimg->height(); ++y)
	{
		for(int x = 0; x < myimg->width(); ++x)
		{
			sumx = sumy = 0;

			if((y == 0) or (y + 1 >= myimg -> height()))
			{
				//sum = 0;
			}
			else if((x == 0) or (x + 1 >= myimg -> width()))
			{
				//sum = 0;
			}
			else
			{
				for(int i = -1; i <= 1; i++)
				{
					for(int j = -1; j <= 1; j++)
					{
						QRgb * px = reinterpret_cast<decltype( px )> (myimg -> scanLine(y + j));

						sumx += qRed(px[x + i]) * Gx[i + 1][j + 1];
						sumy += qRed(px[x + i]) * Gy[i + 1][j + 1];
					}
				}
			}

			ctr++;

			(*gx)[x][y] = sumx;
			(*gy)[x][y] = sumy;

			if(ctr % 1000 == 0)
			{
				cops->Get_pdialog()->setValue(ctr);
			}
		}

	}

	cops->Stop_processing();

	return boost::make_tuple(gx, gy);
}

ImgData::dirgrads_t ImgData::Make_directional_gradients() const
{
	
	dirgrads_t ret = dirgrads_t(new dirgrads_t::value_type(make_tuple(new dirgrads_t::value_type::head_type::value_type(
			myimg->height(), 0.0), new dirgrads_t::value_type::head_type::value_type(myimg->width(), 0.0))));
	
	gradret_t grd = Make_gradients();

	for(int32_t y = 0; y < myimg -> height(); ++y)
	{
		for(int32_t x = 0; x < myimg -> width(); ++x)
		{
			(*ret->get<0>())[y] += ImgData::norm_rgb_val((*grd.get<0> ())[x][y]);
			(*ret->get<1>())[x] += ImgData::norm_rgb_val((*grd.get<1> ())[x][y]);
		}
	}

	std::fill(ret->get<0>()->begin(), ret->get<0>()->begin() + 3, 0);
	std::fill(ret->get<1>()->begin(),ret->get<1>()->begin() + 3, 0);
	std::fill(ret->get<0>()->end() - 3, ret->get<0>()->end(), 0);
	std::fill(ret->get<1>()->end() - 3, ret->get<1>()->end(), 0);

	return ret;
}

ImgData::histret_t ImgData::Make_histogram() const
{
	cops->Start_processing(QString::fromUtf8("Obliczanie histogramów"), myimg->height() * myimg->width());
	size_t ctr = 0;

	//             tuple   -> shared_ptr           ->               vector
	auto hist_r = histret_t::head_type(new histret_t::head_type::value_type(256));
	auto hist_g = histret_t::head_type(new histret_t::head_type::value_type(256));
	auto hist_b = histret_t::head_type(new histret_t::head_type::value_type(256));
	auto hist_a = histret_t::head_type(new histret_t::head_type::value_type(256));

	std::fill(hist_r->begin(), hist_r->end(), 0);
	std::fill(hist_g->begin(), hist_g->end(), 0);
	std::fill(hist_b->begin(), hist_b->end(), 0);
	std::fill(hist_a->begin(), hist_a->end(), 0);

	for(int y = 0; y < myimg -> height(); ++y)
	{
		for(int x = 0; x < myimg -> width(); ++x)
		{
			QRgb * px = reinterpret_cast<decltype( px )> (myimg -> scanLine(y));

			(*hist_r)[norm_rgb_val(qRed(px[x]))] += 1;
			(*hist_g)[norm_rgb_val(qGreen(px[x]))] += 1;
			(*hist_b)[norm_rgb_val(qBlue(px[x]))] += 1;
			if(myimg->hasAlphaChannel())
				(*hist_a)[norm_rgb_val(qAlpha(px[x]))] += 1;

			ctr++;
			if(ctr % 1000 == 0)
			{
				cops->Get_pdialog()->setValue(ctr);
			}

		}
	}

	cops->Stop_processing();

	return boost::make_tuple(hist_r, hist_g, hist_b, hist_a);
}

ImgData::houghret_t ImgData::Hough_tm( size_t rad, size_t cics )
{
	//const size_t IMSIZE = myimg -> height() * myimg -> width();
	typedef QVector<QVector<uint64_t> > acc_t;

	cops->Start_processing(QString::fromUtf8("CHT"), std::pow(static_cast<double> (myimg -> height()
			* myimg -> width()), 4));
	size_t ctr = 0;

	acc_t acc(myimg -> width(), acc_t::value_type(myimg -> height(), 0));
	ImgData::houghret_t res = ImgData::houghret_t(new ImgData::houghret_t::value_type(cics,
			ImgData::houghret_t::value_type::value_type()));

	double x0 = 0, y0 = 0, rth = 0;
	uint64_t max = 0;

	for(int y = 0; y < myimg -> height(); y++)
	{
		for(int x = 0; x < myimg -> width(); x++)
		{
			QRgb * px = reinterpret_cast<decltype( px )> (myimg -> scanLine(y));

			if(qRed(px[x]) == 0)
			{
				for(uint16_t theta = 0; theta < 360; ++theta)
				{
					rth = (theta * M_PI) / 180.0;
					x0 = rint(x - rad * std::cos(rth));
					y0 = rint(y - rad * std::sin(rth));

					if((x0 > 0) and (x0 < myimg -> width()) and (y0 > 0) and (y0 < myimg -> height()))
					{
						acc[static_cast<size_t> (x0)][static_cast<size_t> (y0)]++;
					}
				}
			}

			ctr++;
			if(ctr % 1000 == 0)
			{
				cops->Get_pdialog()->setValue(ctr);
			}
		}
	}

	//max
	for(int y = 0; y < myimg -> height(); y++)
	{
		for(int x = 0; x < myimg -> width(); x++)
		{
			max = acc[x][y] > max ? acc[x][y] : max;
		}

		ctr++;
		if(ctr % 1000 == 0)
		{
			cops->Get_pdialog()->setValue(ctr);
		}
	}

	//norm
	for(int y = 0; y < myimg -> height(); y++)
	{
		for(int x = 0; x < myimg -> width(); x++)
		{
			acc[x][y] = static_cast<acc_t::value_type::value_type> ((acc[x][y] / (max * 1.0)) * 255.0);
		}

		ctr++;
		if(ctr % 1000 == 0)
		{
			cops->Get_pdialog()->setValue(ctr);
		}
	}

	//vals
	for(int y = 0; y < myimg -> height(); y++)
	{
		for(int x = 0; x < myimg -> width(); x++)
		{
			if(acc[x][y] > res -> front().second)
			{
				houghret_t::value_type::iterator it;
				bool broke = false;

				for(it = res -> begin(); it != res-> end(); it++)
				{
					if(acc[x][y] < (it -> second))
					{
						broke = true;
						break;
					}
				}

				if(broke)
				{
					res -> insert(it, qMakePair(QPoint(x, y), acc[x][y]));
				}
				else if(it == res->end())
				{
					res -> push_back(qMakePair(QPoint(x, y), acc[x][y]));
				}

				while(static_cast<size_t> (res->size()) > cics)
				{
					res->pop_front();
				}

			}

			ctr++;
			if(ctr % 1000 == 0)
			{
				cops->Get_pdialog()->setValue(ctr);
			}

		}
	}

	cops->Stop_processing();

	return res;
}

ImgData::Vpf_t ImgData::Vpf( const QRect& reg, Vpf_dir vd )
{
	double sum = 0, mean = 0;
	uint32_t startcnd = 0, stopcnd = 0, substcnd = 0, subspcnd = 0, divval = 1, *coordx = 0, *coordy = 0;
	uint32_t i = 0, n = 0;

	if(vd == Vpf_dir::VERT)
	{
		startcnd = reg.left();
		stopcnd = reg.right();
		substcnd = reg.top();
		subspcnd = reg.bottom();

		divval = reg.height();

		coordy = &i;
		coordx = &n;

	}
	else if(vd == Vpf_dir::HOR)
	{
		startcnd = reg.top();
		stopcnd = reg.bottom();
		substcnd = reg.left();
		subspcnd = reg.right();

		divval = reg.width();

		coordy = &n;
		coordx = &i;
	}

	Vpf_t ret = Vpf_t(new Vpf_t::value_type(Vpf_t::value_type::head_type(stopcnd - startcnd), vd, 0));
	ret->get<3> () = 0;

	for(i = startcnd; i < stopcnd; ++i)
	{
		mean = 0;
		for(n = substcnd; n < subspcnd; ++n)
			mean += qRed(reinterpret_cast<QRgb *> (myimg->scanLine(*coordy))[*coordx]);

		mean /= divval;

		sum = 0;
		for(n = substcnd; n < subspcnd; ++n)
		{
			int16_t intv = qRed(reinterpret_cast<QRgb *> (myimg->scanLine(*coordy))[*coordx]);
			sum += (intv - mean) * (intv - mean);
		}

		ret->get<0> ()[i - startcnd] = sum / reg.height();

		if(ret->get<0> ()[i - startcnd] >= ret->get<3> ())
		{
			ret->get<2> () = i * 1.0;
			ret->get<3> () = ret->get<0> ()[i - startcnd];
		}
	}

	ret->get<4> () = Approx_1st_derivat(ret->get<0> ());
	ret->get<5> () = Find_critical_points(ret->get<4> ());

	return ret;

}

//Finite difference method + sse2
ImgData::Vpf_derivat_t ImgData::Approx_1st_derivat( const Vpf_t::value_type::head_type& vals )
{
	Vpf_derivat_t dervs(new Vpf_derivat_t::value_type(vals.size(), 0));

	const double h = 1.0;
	//	(*dervs)[0] = (-3.0*vals[0]+4.0*vals[1]-vals[2])/(2.0*h);

	/*const Vpf_derivat_t::value_type::value_type c[2] __attribute__ ((aligned (16))) = { 2 * h, 2 * h };
	const __m128d cs = _mm_load_pd(c);
	for(int32_t i = 1; i < vals.size() - 2; i += 2)
	{
		Vpf_derivat_t::value_type::value_type a[2] __attribute__ ((aligned (16))) = { vals[i + 1],
				vals[i + 2] };
		Vpf_derivat_t::value_type::value_type
				b[2] __attribute__ ((aligned (16))) = { -vals[i - 1], -vals[i] };
		__m128d as = _mm_load_pd(a);
		__m128d bs = _mm_load_pd(b);

		_mm_store_pd(a, _mm_div_pd(_mm_add_pd(as, bs), cs));

		(*dervs)[i] = a[0];
		(*dervs)[i + 1] = a[1];
		(*dervs)[i] = (-vals[i-1]+vals[i+1])/(2.0*h);
	}*/

	for(int32_t i = 1; i < vals.size()-1; i ++)
		{
			(*dervs)[i] = (-vals[i-1]+vals[i+1])/(2.0*h);
		}


	//	(*dervs)[vals.size()-1] = (vals[vals.size()-3] - 4.0*vals[vals.size()-2] + 3.0*vals[vals.size()-1])/(2.0*h);

	return dervs;
}

ImgData::Vpf_critpnt_t ImgData::Find_critical_points( Vpf_derivat_t dervs )
{
	Vpf_critpnt_t ret(new Vpf_critpnt_t::value_type);

	for(size_t i = 1; i < static_cast<decltype( i )> (dervs->size()); ++i)
	{
		if((*dervs)[i - 1] >= 0 and (*dervs)[i] < 0)
		{
			ret->push_back(i);
		}
	}

	return ret;
}

/*ImgData::Vpf_critpnt_t ImgData::Find_critical_points( Vpf_derivat_t dervs )
 {
 Vpf_critpnt_t ret(new Vpf_critpnt_t::value_type);

 QList<ImgData::Vpf_critpnt_t::value_type::value_type> tmp;

 for ( int32_t i=dervs->size()-1; i > 1;  )
 {
 if ( dervs->at(i-1) > dervs->at(i) )
 {
 for ( ; i > 0  and dervs->at(i-1) > dervs->at(i); i-- );
 tmp.push_back(i);
 }
 for ( ; i > 0 and dervs->at(i-1) <= dervs->at(i); i-- );
 }

 for ( int32_t i=1; i< dervs->size()-1 ;  )
 {
 if ( dervs->at(i-1) < dervs->at(i) )
 {
 for ( ; i < dervs->size()  and dervs->at(i-1) <= dervs->at(i); i++ );
 if ( tmp.contains(i-1) )
 {
 ret->push_back(i-1);
 }
 }
 for ( ; i < dervs->size() and dervs->at(i-1) >= dervs->at(i); i++ );
 }
 

 return ret;
 }*/

