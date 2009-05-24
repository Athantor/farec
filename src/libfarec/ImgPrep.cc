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

ImgPrep::ret_t ImgPrep::To_gray() const
{
	ret_t rr = ret_t(new QImage(*myimg));

	cops->Start_processing(QString::fromUtf8("Konwersja na skalę szarości"), (rr->height()) * (rr->bytesPerLine()
			/ sizeof(QRgb)));
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

	//TODO fix fail
	//TODO make gray

	cops->Start_processing(QString::fromUtf8("Konwersja na skalę szarości"), (rr->height()) * (rr->bytesPerLine()));
	size_t ctr = 0;

	ImgData::gradret_t grads = ImgData(pnt, *myimg).Make_gradients();

	int sumx = 0, sumy = 0, sum = 0;

	for(int y = 1; y < myimg -> height() - 1; y++)
	{
		for(int x = 1; x < myimg -> width() - 1; x++)
		{
			sumx = sumy = 0;

			if((y == 0) or (y + 1 >= myimg -> height()))
			{
				sum = 0;
			}
			else if((x == 0) or (x + 1 >= myimg -> width()))
			{
				sum = 0;
			}
			else
			{

				sumx = (*grads.get<0> ())[x][y];
				sumy = (*grads.get<1> ())[x][y];

				//  sum = std::abs(sumx) + std::abs(sumy);
				sum = static_cast<int> (255 - std::sqrt(std::pow(sumx, 2.0) + std::pow(sumy, 2.0)));
				sum = sum > 255 ? 255 : sum;
				sum = sum < 0 ? 0 : sum;
			}
			ctr++;
			sum = static_cast<uchar> (sum);

			rr->setPixel(x, y, qRgb(sum, sum, sum));

		}
	}

	cops->Stop_processing();

	return rr;
}
