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

ImgData::gradret_t ImgData::Make_gradients() const
{
	const int8_t Gx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
	const int8_t Gy[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	cops->Start_processing(QString::fromUtf8("Obliczanie gradientów"), myimg->height() * myimg->width());

	int sumx = 0, sumy = 0;
	gradarr_t gx(new gradarr_t::element_type(myimg->width(), gradarr_t::element_type::value_type(myimg->height(), 0))); //pointer to a vector of vectors filled with 0
	gradarr_t gy(new gradarr_t::element_type(myimg->width(), gradarr_t::element_type::value_type(myimg->height(), 0))); //pointer to a vector of vectors filled with 0
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
	
	cops->Start_processing(QString::fromUtf8("CHT"), std::pow(static_cast<double>(myimg -> height() * myimg -> width()), 4));
	size_t ctr = 0;

	acc_t acc(myimg -> width(), acc_t::value_type(myimg -> height(), 0));
	ImgData::houghret_t res = ImgData::houghret_t(new ImgData::houghret_t::value_type(cics, ImgData::houghret_t::value_type::value_type() ));

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
