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

ImgData::ImgData( QWidget *p, const QImage& im ) : ImgOp(p, im)
{

}

ImgData::~ImgData()
{
	// XXX Auto-generated destructor stub
}

ImgData::gradret_t ImgData::Make_gradients() const
{
	const int8_t Gx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
	const int8_t Gy[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	cops->Start_processing(QString::fromUtf8("Obliczanie gradientów"), myimg->height() * myimg->width());

	int sumx = 0, sumy = 0;
	gradarr_t gx(new gradarr_t::element_type(myimg->width(), gradarr_t::element_type::value_type(myimg->width(), 0))); //pointer to a vector of vectors filled with 0
	gradarr_t gy(new gradarr_t::element_type(myimg->width(), gradarr_t::element_type::value_type(myimg->width(), 0))); //pointer to a vector of vectors filled with 0
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
						QRgb mspx = myimg -> pixel(x + i, y + j);

						sumx += qRed(mspx) * Gx[i + 1][j + 1];
						sumy += qRed(mspx) * Gy[i + 1][j + 1];

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

	return boost::make_tuple(gx, gy);
}
