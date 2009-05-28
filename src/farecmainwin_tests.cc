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
 * farecmainwin_tests.cc
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#ifdef DEBUG_KRZYS

#include "farecmainwin.hh"

void FarecMainWin::To_gray( bool )
{
	ImgPrep ip(this, *inimg);
	outimg.reset(new QImage(*ip.To_gray()));

	Set_label_img(ui.PviewImgLbl, *outimg);
}

void FarecMainWin::Test_sobel( bool )
{
	outimg.reset(new QImage(*(ImgPrep(this, *inimg).Sobel_ed())));
	Set_label_img(ui.PviewImgLbl, *outimg);
}

void FarecMainWin::Test_otsu( bool )
{
	outimg.reset(new QImage(*(ImgPrep(this, *inimg).Otsu_bin())));
	Set_label_img(ui.PviewImgLbl, *outimg);
}

void  FarecMainWin::Test_gauss(bool)
{
	outimg.reset(new QImage(*(ImgPrep(this, *inimg).Gaussian_blur(5))));
	Set_label_img(ui.PviewImgLbl, *outimg);
}

void FarecMainWin::Test_median(bool)
{
	outimg.reset(new QImage(*(ImgPrep(this, *inimg).Median_filter())));
    Set_label_img(ui.PviewImgLbl, *outimg);
}

#endif
