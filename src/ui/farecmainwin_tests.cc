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

void FarecMainWin::Test_gauss( bool )
{

	bool ok = false;
	uint64_t sz = QInputDialog::getInt(this, "Gauss", "Rozmiar macierzy", 5, 0, 999, 2, &ok);

	if(ok)
	{
		try
		{
			outimg.reset(new QImage(*(ImgPrep(this, *inimg).Gaussian_blur(sz))));
			Set_label_img(ui.PviewImgLbl, *outimg);
		}
		catch(const FEInvalidParameter & e)
		{
			QMessageBox::critical(this, QString::fromUtf8("Błąd"), polymorphic_downcast<
					const FarecException *> (&e)->what());
		}
	}
}

void FarecMainWin::Test_median( bool )
{
	outimg.reset(new QImage(*(ImgPrep(this, *inimg).Median_filter())));
	Set_label_img(ui.PviewImgLbl, *outimg);
}

void FarecMainWin::Test_hough( bool )
{

	if(not static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	bool ok = false;
	uint64_t sz = QInputDialog::getInt(this, QString::fromUtf8("/ˈhʌf/"), QString::fromUtf8("Promień"), 20,
			0, 999, 1, &ok);
	if(ok)
	{
		ok = false;
		uint64_t sz2 = QInputDialog::getInt(this, QString::fromUtf8("/ˈhʌf/ 2"),
				QString::fromUtf8("Wyników"), 10, 0, 999, 1, &ok);
		if(ok)
		{
			ImgData::houghret_t ht = ImgData(this, *inimg).Hough_tm(sz, sz2);

			outimg.reset(new QImage(*inimg));

			QPainter qpt(outimg.get());
			qpt.setPen(QPen("red"));

			for(auto it = ht->begin(); it != ht->end(); ++it)
			{
				qpt.setPen(QPen("red"));
				qpt.drawEllipse(it->first, sz, sz);
				qpt.drawRect(it->first.x() - 2, it->first.y() - 2, 4, 4);
				qpt.drawLine(it->first.x(), it->first.y() - 5, it->first.x(), it->first.y() + 5);
				qpt.drawLine(it->first.x() - 5, it->first.y(), it->first.x() + 5, it->first.y());

				qpt.setPen(QPen("lime"));
				qpt.drawText(it->first, QString::fromUtf8("(%1, %2), %3").arg(it->first.x()).arg(
						it->first.y()).arg(sz));
			}

			qpt.end();

			Set_label_img(ui.PviewImgLbl, *outimg);
		}

	}

}

void FarecMainWin::Test_avg( bool )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	bool ok = false;
	double sz = QInputDialog::getDouble(this, QString::fromUtf8("średni"), QString::fromUtf8("% białych:"),
			0.75, 0, 1.0, 2, &ok);
	if(ok)
	{
		outimg.reset(new QImage(*(ImgPrep(this, *inimg).Average_bin_blur(sz))));
		Set_label_img(ui.PviewImgLbl, *outimg);
	}
}

void FarecMainWin::Test_face_reg( bool )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	FeatExtract::region_t freg = FeatExtract(this, *inimg).Get_face_from_grads();
	outimg.reset(new QImage(*inimg));

	QPainter qpt(outimg.get());
	qpt.setPen("red");

	qpt.drawRect(*freg);

	qpt.end();

	Set_label_img(ui.PviewImgLbl, *outimg);

}

void FarecMainWin::Test_autoprep( bool )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	outimg.reset(new QImage(*(ImgPrep(this, *inimg).Batch_prepare(QMessageBox::question(this, "Przygotowanie", QString::fromUtf8("Zrobić pełne?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes ))));
	Set_label_img(ui.PviewImgLbl, *outimg);
}

void FarecMainWin::Test_eyes_cht(bool)
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}
	FeatExtract::cht_eyeloc_t el = FeatExtract(this, *inimg).Get_eyes_from_cht();
	
	outimg.reset(new QImage(*inimg));
	
	QPainter qpt(outimg.get());
	qpt.setPen(QPen("red"));

	
	qpt.drawRect(el->get<0>().x() - 7, el->get<0>().y() - 7, 14, 14);
	qpt.drawLine(el->get<0>().x(), el->get<0>().y() - 10, el->get<0>().x(), el->get<0>().y() + 5);
	qpt.drawLine(el->get<0>().x() - 10, el->get<0>().y(), el->get<0>().x() + 10, el->get<0>().y());
	
	qpt.drawRect(el->get<1>().x() - 7, el->get<1>().y() - 7, 14, 14);
	qpt.drawLine(el->get<1>().x(), el->get<1>().y() - 10, el->get<1>().x(), el->get<1>().y() + 5);
	qpt.drawLine(el->get<1>().x() - 10, el->get<1>().y(), el->get<1>().x() + 10, el->get<1>().y());
	
	qpt.drawEllipse(el->get<0>(), el->get<2>(), el->get<2>());
	qpt.drawEllipse(el->get<1>(), el->get<2>(), el->get<2>());
	
	qpt.end();

	Set_label_img(ui.PviewImgLbl, *outimg);

	
}

#endif
