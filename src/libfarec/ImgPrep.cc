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
 * ImgPrep.cc
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#include "ImgPrep.hh"

ImgPrep::ImgPrep( QWidget * p, const QImage & qi ) :
	ImgOp(p, qi)
{
}

ImgPrep::~ImgPrep()
{
}

ImgPrep::ret_t ImgPrep::Batch_prepare( bool full ) const
{
	ret_t rr = ImgPrep(pnt, *myimg).To_gray();
	rr = ImgPrep(pnt, *rr).Gaussian_blur(5);
	rr = ImgPrep(pnt, *rr).Sobel_ed();
	if(full)
	{
		rr = ImgPrep(pnt, *rr).Otsu_bin();
		rr = ImgPrep(pnt, *rr).Median_filter();
	}

	return rr;
}

ImgPrep::ret_t ImgPrep::To_gray() const
{
	ret_t rr = ret_t(new QImage(*myimg));

	cops->Start_processing(QString::fromUtf8("Konwersja na skalę szarości"), (rr->height())
			* (rr->bytesPerLine() / sizeof(QRgb)));
	size_t ctr = 0;

	for(int i = 0; i < rr->height(); ++i)
	{
		uchar *sline = rr->scanLine(i);
		QRgb *pdata = reinterpret_cast<QRgb *> (sline);

		for(size_t j = 0; j < rr->bytesPerLine() / sizeof(QRgb); ++j)
		{
			++ctr;
			int v = qGray(pdata[j]);
			pdata[j] = QColor(v, v, v).rgb();
		}

		if(ctr % 1000 == 0)
		{
			cops->Get_pdialog()->setValue(ctr);
		}

	}

	cops->Stop_processing();

	return rr;
}

ImgPrep::ret_t ImgPrep::Sobel_ed() const
{
	ret_t rr = ret_t(new QImage(*myimg));

	rr = ImgPrep(pnt, *rr).To_gray();

	cops->Start_processing(QString::fromUtf8("Wykrywanie krawędzi [sobel]"), (rr->height()) * (rr->width()));
	size_t ctr = 0;

	ImgData::gradret_t grads = ImgData(pnt, *myimg).Make_gradients();

	int sumx = 0, sumy = 0, sum = 0;

	for(int y = 1; y < rr -> height() - 1; ++y)
	{
		for(int x = 1; x < rr -> width() - 1; ++x)
		{
			sumx = sumy = 0;

			if((y == 0) or (y + 1 >= rr -> height()))
			{
				sum = 0;
			}
			else if((x == 0) or (x + 1 >= rr -> width()))
			{
				sum = 0;
			}
			else
			{
				sumx = (*grads.get<0> ())[x][y];
				sumy = (*grads.get<1> ())[x][y];

				//  sum = std::abs(sumx) + std::abs(sumy);
				//sum = static_cast<int> (255 - std::sqrt(std::pow(sumx, 2.0) + std::pow(sumy, 2.0)));
				sum = static_cast<int> (255 - std::hypot(static_cast<double> (sumx),
						static_cast<double> (sumy)));
				sum = sum > 255 ? 255 : sum;
				sum = sum < 0 ? 0 : sum;
			}
			ctr++;
			sum = static_cast<uchar> (sum);

			//here new image is created:
			reinterpret_cast<QRgb *> (rr -> scanLine(y))[x] = QColor(sum, sum, sum).rgb();

			if(ctr % 1000 == 0)
			{
				cops->Get_pdialog()->setValue(ctr);
			}
		}
	}

	cops->Stop_processing();

	return rr;
}

/**
 * If given more than percent of 3x3 neighbours of current pixel are 
 * 255, then pixel is set to 255, otherwise it's left alone as is
 * @param pct percent
 * @return image
 * 
 * @warning designed to work on binary images
 * 
 */
ImgPrep::ret_t ImgPrep::Average_bin_blur( double pct ) const
{
	if((pct < (0/* + DBL_EPSILON*/)) or (pct > (1.0/* - DBL_EPSILON*/)))
	{
		throw FEInvalidParameter(QString::fromUtf8("Blur factor out of range (0.0-1.0): %1 ").arg(pct, 0,
				'f', 2).toStdString());
	}

	ret_t rr = ret_t(new QImage(*myimg));
	uint16_t sum = 0, curr = 0;

	cops->Start_processing(QString::fromUtf8("Rozmywanie średnie"), (rr->height()) * (rr->width()));
	size_t ctr = 0;

	const double mypct = (9.0 * 255) * pct;

	for(int y = 0; y < myimg -> height(); y++)
	{
		for(int x = 0; x < myimg -> width(); x++)
		{
			QRgb * origpxarr = reinterpret_cast<QRgb *> (myimg -> scanLine(y));

			if((y == 0) or (y + 1 >= myimg -> height()))
			{
				sum = 255;
			}
			else if((x == 0) or (x + 1 >= myimg -> width()))
			{
				sum = 255;
			}
			else
			{
				sum = 0;
				curr = qRed(origpxarr[x]);

				for(int i = -1; i <= 1; i++)
				{
					for(int j = -1; j <= 1; j++)
					{
						sum += qRed(reinterpret_cast<QRgb *> (myimg -> scanLine(y + j))[x + i]);
					}
				}
			}

			if((sum) > mypct)
			{
				reinterpret_cast<QRgb *> (rr -> scanLine(y))[x] = QColor(255, 255, 255).rgb();
			}
			else
			{
				reinterpret_cast<QRgb *> (rr -> scanLine(y))[x] = QColor(curr, curr, curr).rgb();
			}
		}

		if(ctr % 1000 == 0)
		{
			cops->Get_pdialog()->setValue(ctr);
		}
	}

	cops->Stop_processing();

	return rr;
}

ImgPrep::ret_t ImgPrep::Otsu_bin() const
{

	ret_t rr = ret_t(new QImage(*myimg));

	const uint16_t GLVL = 256;
	const uint64_t IMSIZE = myimg -> height() * myimg -> width();

	uint thd = 0;

	double prob[GLVL];
	std::fill(prob, prob + GLVL, 0.0);

	double mean[GLVL];
	std::fill(mean, mean + GLVL, 0.0);

	double omega[GLVL];
	std::fill(omega, omega + GLVL, 0.0);

	double max_sigma = 0;
	double sigma[GLVL];
	std::fill(sigma, sigma + GLVL, 0.0);

	cops->Start_processing(QString::fromUtf8("Binaryzacja Otsu"), IMSIZE + (GLVL * 3));
	size_t ctr = 0;

	ImgData::histret_t hist = ImgData(pnt, *rr).Make_histogram();

	for(uint16_t i = 0; i < GLVL; ++i)
	{
		prob[i] = static_cast<double> ((*hist.get<0> ())[i]) / IMSIZE;

		ctr++;
		if(ctr % 1000 == 0)
		{
			cops->Get_pdialog()->setValue(ctr);
		}

	}

	mean[0] = 0;
	omega[0] = prob[0];
	for(uint16_t i = 1; i < GLVL; ++i)
	{
		mean[i] = mean[i - 1] + i * prob[i];
		omega[i] = omega[i - 1] + prob[i];

		ctr++;
		if(ctr % 1000 == 0)
		{
			cops->Get_pdialog()->setValue(ctr);
		}
	}

	for(uint16_t i = 0; i < GLVL - 1; ++i)
	{
		if((omega[i] != 0.0) and (omega[i] != 1.0))
		{
			sigma[i] = std::pow(mean[GLVL - 1] * omega[i] - mean[i], 2) / (omega[i] * (1.0 - omega[i]));
		}
		else
		{
			sigma[i] = 0.0;
		}

		if(sigma[i] > max_sigma)
		{
			thd = i;
			max_sigma = sigma[i];
		}

		ctr++;
		if(ctr % 1000 == 0)
		{
			cops->Get_pdialog()->setValue(ctr);
		}
	}

	for(int y = 0; y < myimg -> height(); ++y)
	{
		for(int x = 0; x < myimg -> width(); ++x)
		{
			QRgb * pxv = reinterpret_cast<QRgb *> (myimg -> scanLine(y));
			QRgb * npxv = reinterpret_cast<QRgb *> (rr -> scanLine(y));

			if((unsigned) qRed(pxv[x]) > thd)
			{
				npxv[x] = QColor(255, 255, 255).rgb();
			}
			else
			{
				npxv[x] = QColor(0, 0, 0).rgb();
			}

			ctr++;
			if(ctr % 1000 == 0)
			{
				cops->Get_pdialog()->setValue(ctr);
			}
		}
	}

	cops->Stop_processing();

	return rr;
}

ImgPrep::ret_t ImgPrep::Gaussian_blur( uint8_t ksize ) const
{

	if(ksize % 2 == 0)
		throw FEInvalidParameter("Kernel size has to be an odd number");

	ret_t rr = ret_t(new QImage(*myimg));

	const size_t KERNMID = static_cast<size_t> (ksize / 2.0);
	QVector<QVector<uint64_t> > kern(ksize, QVector<uint64_t> (ksize, 0));
	uint64_t sum = 0;

	//uint kern[][5] = { { 2, 4, 5, 4, 2 }, { 4, 9, 12, 9, 4 }, { 5, 12, 15, 12, 5 }, { 2, 4, 5, 4, 2 }, { 4, 9, 12, 9, 4 } };
	//uint64_t sum = 159;

	cops->Start_processing(QString::fromUtf8("Rozmywanie met. Gaussa"), (rr->height() * (rr->width()))
			+ (std::pow(static_cast<double> ((ksize - KERNMID)), 2)));
	size_t ctr = 0;

	for(size_t i = 0; i < (ksize - KERNMID); ++i) // y
	{
		for(size_t j = 0; j < (ksize - KERNMID); ++j) // x
		{
			/*const int16_t X = j - (ksize - KERNMID), Y = i - (ksize - KERNMID);
			 kern[j][i] = (1 / (2 * M_PI * std::pow(sigma, 2))) * std::exp(-((std::pow(static_cast<const double> (X), 2)
			 * std::pow(static_cast<const double> (Y), 2)) / (2 * std::pow(sigma, 2))));
			 sum += kern[j][i];*/

			kern[j][i] = kern[(ksize - j) - 1][i] = kern[j][(ksize - i) - 1] = kern[(ksize - j) - 1][(ksize
					- i) - 1] = std::pow(2.0, static_cast<int> (j + i));
		}
	}

	BOOST_FOREACH(QVector<uint64_t> vct, kern)
	{
		BOOST_FOREACH	(uint64_t vval, vct)
		{
			sum += vval;
		}
	}

	int64_t px[3];
	for(size_t y = 0; y < static_cast<size_t>(myimg -> height()); y++)
	{
		for(size_t x = 0; x < static_cast<size_t>(myimg -> width()); x++)
		{
			QRgb currpx = reinterpret_cast<QRgb *> (myimg -> scanLine(y))[x];

			std::fill(px, px + 3, 0);
			if((y < (ksize - KERNMID)) or ((y + (ksize - KERNMID)) >= static_cast<size_t>(myimg -> height())))
			{
				px[0] = qRed(currpx);
				px[1] = qGreen(currpx);
				px[2] = qBlue(currpx);
			}
			else if((x < (ksize - KERNMID)) or ((x + (ksize - KERNMID)) >= static_cast<size_t>(myimg -> width())))
			{
				px[0] = qRed(currpx);
				px[1] = qGreen(currpx);
				px[2] = qBlue(currpx);
			}
			else
			{

				for(int16_t i = -static_cast<int64_t>(KERNMID); i <= static_cast<int64_t>(KERNMID); i++)
				{
					for(int16_t j = -static_cast<int64_t>(KERNMID); j <= static_cast<int64_t>(KERNMID); j++)
					{
						px[0] += qRed(reinterpret_cast<QRgb *> (myimg -> scanLine(y + j))[x + i]) * kern[j + KERNMID][i
						+ KERNMID];
						px[1] += qGreen(reinterpret_cast<QRgb *> (myimg -> scanLine(y + j))[x + i])
						* kern[j + KERNMID][i + KERNMID];
						px[2] += qBlue(reinterpret_cast<QRgb *> (myimg -> scanLine(y + j))[x + i])
						* kern[j + KERNMID][i + KERNMID];
					}
				}

				std::transform(px, px + 3, px, std::bind2nd(std::divides<double>(), sum));
			}

			uint8_t pxvals[3];
			std::copy(px, px + 3, pxvals);

			reinterpret_cast<QRgb *> (rr -> scanLine(y))[x] = QColor(pxvals[0], pxvals[1], pxvals[2]).rgb();

			ctr++;
			if(ctr % 1000 == 0)
			{
				cops->Get_pdialog()->setValue(ctr);
			}
		}
	}

	cops->Stop_processing();

	return rr;
}

ImgPrep::ret_t ImgPrep::Median_filter() const
{
	ret_t rr = ret_t(new QImage(*myimg));

	cops->Start_processing(QString::fromUtf8("Rozmywanie średnie"), (rr->height()) * (rr->width()));
	size_t pctr = 0;

	for(int y = 0; y < myimg -> height(); y++)
	{
		for(int x = 0; x < myimg -> width(); x++)
		{
			//QVector<QVector<int> > pxbuf(3, QVector<int> (9, 0));
			uint8_t pxbuf[3][9];

			if((y == 0) or (y + 1 >= myimg -> height()))
			{
				//std::fill(px, px+3, 255);
			}
			else if((x == 0) or (x + 1 >= myimg -> width()))
			{
				//std::fill(px, px+3, 255);
			}
			else
			{

				size_t ctr = 0;

				for(int i = -1; i <= 1; i++)
				{
					for(int j = -1; j <= 1; j++)
					{
						QRgb currpx = reinterpret_cast<QRgb *> (rr -> scanLine(y + j))[x + i];

						pxbuf[0][ctr] = qRed(currpx);
						pxbuf[1][ctr] = qGreen(currpx);
						pxbuf[2][ctr] = qBlue(currpx);
						ctr++;
					}
				}

				for(uint8_t it = 0; it < 3; ++it)
				{
					std::sort(pxbuf[it], pxbuf[it] + 9);
				}
			}

			reinterpret_cast<QRgb *> (rr -> scanLine(y))[x]
					= QColor(pxbuf[0][4], pxbuf[1][4], pxbuf[2][4]).rgb();

			pctr++;
			if(pctr % 5000 == 0)
			{
				cops->Get_pdialog()->setValue(pctr);
			}
		}
	}

	cops->Stop_processing();

	return rr;
}
