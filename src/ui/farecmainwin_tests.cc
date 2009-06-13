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

#include <iostream>

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

	outimg.reset(new QImage(*(ImgPrep(this, *inimg).Batch_prepare(QMessageBox::question(this,
			"Przygotowanie", QString::fromUtf8("Zrobić pełne?"), QMessageBox::Yes | QMessageBox::No,
			QMessageBox::Yes) == QMessageBox::Yes))));
	Set_label_img(ui.PviewImgLbl, *outimg);
}

void FarecMainWin::Test_eyes_cht( bool )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	bool ok = false;
	uint64_t sz = QInputDialog::getInt(this, "Haf", QString::fromUtf8("Ilość okręgów"), 50, 0, 999, 1, &ok);

	if(ok)
	{
		FeatExtract::cht_eyeloc_t el = FeatExtract(this, *inimg).Get_irises_from_cht(sz);

		outimg.reset(new QImage(*inimg));

		QPainter qpt(outimg.get());
		qpt.setPen(QPen("red"));

		qpt.drawRect(el->get<0> ().x() - 7, el->get<0> ().y() - 7, 14, 14);
		qpt.drawLine(el->get<0> ().x(), el->get<0> ().y() - 10, el->get<0> ().x(), el->get<0> ().y() + 5);
		qpt.drawLine(el->get<0> ().x() - 10, el->get<0> ().y(), el->get<0> ().x() + 10, el->get<0> ().y());

		qpt.drawRect(el->get<1> ().x() - 7, el->get<1> ().y() - 7, 14, 14);
		qpt.drawLine(el->get<1> ().x(), el->get<1> ().y() - 10, el->get<1> ().x(), el->get<1> ().y() + 5);
		qpt.drawLine(el->get<1> ().x() - 10, el->get<1> ().y(), el->get<1> ().x() + 10, el->get<1> ().y());

		qpt.drawEllipse(el->get<0> (), el->get<2> (), el->get<2> ());
		qpt.drawEllipse(el->get<1> (), el->get<2> (), el->get<2> ());

		qpt.save();

		qpt.setFont(QFont("monospace", 16));
		qpt.drawText(0, 16, "r = " + QString::number(el->get<2> ()) + "px");

		qpt.restore();

		qpt.end();

		Set_label_img(ui.PviewImgLbl, *outimg);
	}
}

void FarecMainWin::Test_eyes_apx( bool )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	bool ok = false;
	uint64_t sz = QInputDialog::getInt(this, "Haf", QString::fromUtf8("Ilość okręgów"), 50, 0, 999, 1, &ok);

	if(ok)
	{
		FeatExtract fe(this, *inimg);
		FeatExtract::cht_eyeloc_t el = fe.Get_irises_from_cht(sz);
		shared_ptr<FeatExtract::eyeloc_t> ael[2] = { fe.Make_approx_eye_pts(el->get<0> (), el->get<2> ()),
				fe.Make_approx_eye_pts(el->get<1> (), el->get<2> ()) };

		outimg.reset(new QImage(*inimg));

		QPainter qpt(outimg.get());
		Draw_eye_shape(ael, qpt, el);
		qpt.end();

		Set_label_img(ui.PviewImgLbl, *outimg);
	}
}

void FarecMainWin::Draw_eye_shape( shared_ptr<FeatExtract::eyeloc_t> *ael, QPainter & qpt,
		FeatExtract::cht_eyeloc_t el )
{
	qpt.save();
	int er = 0;

	for(size_t i = 0; i < 2; ++i)
	{

		if(static_cast<bool> (el))
		{
			er = el->get<2> ();
		}
		else
		{
			er = (ael[i]->at(3).x() - ael[i]->at(2).x()) / 2.0;
		}

		qpt.setPen(QPen("yellow"));
		qpt.setBrush(QBrush(qpt.pen().color(), Qt::SolidPattern));
		for(auto it = ael[i]->begin(); it != ael[i]->end(); ++it)
		{
			qpt.drawEllipse(*it, 2, 2);
		}

		qpt.setPen(QPen("lime"));
		qpt.drawArc(ael[i]->at(4).x(), ael[i]->at(0).y(), ael[i]->at(5).x() - ael[i]->at(4).x(), er * 2, 0,
				180 * 16);
		qpt.drawArc(ael[i]->at(4).x(), ael[i]->at(0).y(), ael[i]->at(5).x() - ael[i]->at(4).x(), er * 2, 180
				* 60, 360 * 60);
		
		qpt.save();
		qpt.setPen(QPen(QBrush("pink"), 1, Qt::DashLine));
		qpt.drawLine(ael[i]->at(0), ael[i]->at(2) );				
		qpt.drawLine(ael[i]->at(2), ael[i]->at(1) );
		qpt.drawLine(ael[i]->at(1), ael[i]->at(3) );
		qpt.drawLine(ael[i]->at(3), ael[i]->at(0) );
		
		qpt.restore();
		
	}
	qpt.restore();

	qpt.save();
	qpt.setPen(QPen("red"));

	if(static_cast<bool> (el))
	{
		qpt.drawEllipse(el->get<0> (), el->get<2> (), el->get<2> ());
		qpt.drawEllipse(el->get<1> (), el->get<2> (), el->get<2> ());
	} 
	
	qpt.restore();
}

void FarecMainWin::Test_eyes_vpf( bool )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	bool ok = false;
	uint64_t sz = QInputDialog::getInt(this, "Oczy vpf", QString::fromUtf8("Max okolice punktu px\n(-1: auto)"), -1, -1,
			999, 1, &ok);

	if(ok)
	{
		FeatExtract::vpf_eyeloc_t vel = FeatExtract(this, *inimg).Get_eyes_from_vpf(sz, ImgData::CHT_CIRCNUM);
		shared_ptr<FeatExtract::eyeloc_t> e1(new FeatExtract::eyeloc_t(vel->first));
		shared_ptr<FeatExtract::eyeloc_t> e2(new FeatExtract::eyeloc_t(vel->second));

		shared_ptr<FeatExtract::eyeloc_t> ael[2] = { e1, e2 };

		outimg.reset(new QImage(*inimg));

		QPainter qpt(outimg.get());
		FeatExtract::cht_eyeloc_t tmp;
		Draw_eye_shape(ael, qpt, tmp);

		qpt.end();

		Set_label_img(ui.PviewImgLbl, *outimg);
	}
}

void FarecMainWin::Test_Vpf_hori( bool )
{
	Test_vpf(ImgData::Vpf_dir::HOR);
}

void FarecMainWin::Test_Vpf_vert( bool )
{
	Test_vpf(ImgData::Vpf_dir::VERT);
}

void FarecMainWin::Test_vpf( ImgData::Vpf_dir vd )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	ImgData id = ImgData(this, *ImgPrep(this, *ImgPrep(this, *inimg).To_gray()).Gaussian_blur(9));
	FeatExtract::eyewin_t ew = FeatExtract(this, *inimg).Make_eye_windows();

	outimg.reset(new QImage(*inimg));

	Test_vpf_eye(vd, ew->get<0> (), id);
	Test_vpf_eye(vd, ew->get<1> (), id);

	Set_label_img(ui.PviewImgLbl, *outimg);
}

void FarecMainWin::Test_vpf_eye( ImgData::Vpf_dir vd, const QRect & reg, ImgData& id )
{

	ImgData::Vpf_t ret = id.Vpf(reg, vd);

	double scale_dev = 0;
	uint32_t startcnd = 0, stopcnd = 0, substcnd = 0, subspcnd = 0, divval = 1, *coordx = 0, *coordy = 0;
	uint32_t i = 0, n = 0;

	if(ret->get<1> () == ImgData::Vpf_dir::VERT)
	{
		startcnd = reg.left();
		stopcnd = reg.right();
		substcnd = reg.top();
		subspcnd = reg.bottom();

		divval = reg.height();

		coordy = &i;
		coordx = &n;
	}
	else if(ret->get<1> () == ImgData::Vpf_dir::HOR)
	{
		startcnd = reg.top();
		stopcnd = reg.bottom();
		substcnd = reg.left();
		subspcnd = reg.right();

		divval = reg.width();

		coordy = &n;
		coordx = &i;
	}

	scale_dev = sqrt(ret->get<3> ());
	while(ret->get<3> () / scale_dev > outimg->height())
		scale_dev++;
	QPainter qpt(outimg.get());

	for(i = startcnd; i < stopcnd; i++)
	{
		n = (int) (subspcnd - ret->get<0> ()[i - startcnd] / scale_dev);
		if(n < substcnd)
			n = 0;
		if(n >= subspcnd)
			n = subspcnd - 1;

		if(ret->get<4> ()->at(i - startcnd) < 0)
			qpt.setPen(QPen("red"));
		else if(ret->get<4> ()->at(i - startcnd) > 0)
			qpt.setPen(QPen("lime"));
		else
			qpt.setPen(QPen("yellow"));

		qpt.drawPoint(*coordy, *coordx);
	}

	qpt.setPen(QPen("blue"));
	qpt.drawRect(reg);

	qpt.setPen(QPen("orange"));
	for(size_t myi = 0; myi < static_cast<decltype( myi )> (ret->get<5> ()->size()); ++myi)
	{
		if(ret->get<1> () == ImgData::Vpf_dir::VERT)
		{
			qpt.drawLine(reg.x() + ret->get<5> ()->at(myi), substcnd, reg.x() + ret->get<5> ()->at(myi),
					subspcnd);
		}
		else if(ret->get<1> () == ImgData::Vpf_dir::HOR)
		{
			qpt.drawLine(substcnd, reg.y() + ret->get<5> ()->at(myi), subspcnd, reg.y() + ret->get<5> ()->at(
					myi));
		}
	}

	qpt.setPen(QPen("red"));
	if(ret->get<1> () == ImgData::Vpf_dir::VERT)
	{
		qpt.drawLine(startcnd, reg.top() - ret->get<2> (), stopcnd, reg.top() - ret->get<2> ());
	}
	else if(ret->get<1> () == ImgData::Vpf_dir::HOR)
	{
		qpt.drawLine(reg.left() - ret->get<2> (), startcnd, reg.left() - ret->get<2> (), stopcnd);
	}

	qpt.end();
}

void FarecMainWin::Test_nos(bool)
{
	
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}
	
	FeatExtract::noseloc_t nl =  FeatExtract(this, *inimg).Get_nose_from_grads();
	outimg.reset(new QImage(*inimg));

	QPainter qpt(outimg.get());
	qpt.setPen(QPen("red"));
	
	
	qpt.drawEllipse(nl->get<0>(),2,2);
	qpt.drawEllipse(nl->get<1>(),2,2);
	
	qpt.end();
	
	Set_label_img(ui.PviewImgLbl, *outimg);
}

#endif

