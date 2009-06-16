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
 * Graph.hh
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#include "farecmainwin.hh"

FarecMainWin::FarecMainWin( QWidget *parent ) :
	QMainWindow(parent), db_sb_lbl(new QLabel(this)), inimg(new QImage), outimg(new QImage)
{
	ui.setupUi(this);

	Populate_toolbar();
	Connect_slots();
}

FarecMainWin::~FarecMainWin()
{

}

void FarecMainWin::Connect_slots()
{
	connect(ui.splitter, SIGNAL(splitterMoved ( int, int )), this, SLOT( Resize_labels_imgs(int, int) ));
	connect(ui.splitter_2, SIGNAL(splitterMoved ( int, int )), this, SLOT( Resize_labels_imgs(int, int) ));

	connect(ui.actionOtw_rz, SIGNAL(triggered(bool)), this, SLOT(Load_file(bool)));

	connect(ui.actionU_yj_podgl_du, SIGNAL(triggered(bool)), this, SLOT(Make_preview_the_source(bool)));

	connect(ui.actionGradPvwWej_cie, SIGNAL(triggered(bool)), this, SLOT(Show_grads_src(bool)));
	connect(ui.actionGradPvwPodgl_d, SIGNAL(triggered(bool)), this, SLOT(Show_grads_pview(bool)));
	connect(ui.actionPoka_odcinki, SIGNAL(triggered(bool)), this, SLOT(Show_segments(bool)));

	connect(ui.actionDbPo_cz, SIGNAL(triggered(bool)), this, SLOT(Connect_to_db(bool)));
	connect(ui.actionDbRoz_cz, SIGNAL(triggered(bool)), this, SLOT(Disconnect_from_db(bool)));

	connect(ui.actionW_a_ciel, SIGNAL(triggered(bool)), this, SLOT(Add_person(bool)));
	
	
#ifdef DEBUG_KRZYS

	ui.menuOpsy->setEnabled(true);

	connect(ui.actionAutoprep, SIGNAL(triggered(bool)), this, SLOT(Test_autoprep(bool)));
	connect(ui.actionNa_szaro, SIGNAL(triggered(bool)), this, SLOT(To_gray(bool)));
	connect(ui.actionKontrast, SIGNAL(triggered(bool)), this, SLOT(Contrast(bool)));
	connect(ui.actionSobel, SIGNAL(triggered(bool)), this, SLOT(Test_sobel(bool)));
	connect(ui.actionOtsu, SIGNAL(triggered(bool)), this, SLOT(Test_otsu(bool)));
	connect(ui.actionGauss, SIGNAL(triggered(bool)), this, SLOT(Test_gauss(bool)));
	connect(ui.actionMedianowy, SIGNAL(triggered(bool)), this, SLOT(Test_median(bool)));
	connect(ui.actionHough, SIGNAL(triggered(bool)), this, SLOT(Test_hough(bool)));
	connect(ui.action_redni, SIGNAL(triggered(bool)), this, SLOT(Test_avg(bool)));
	connect(ui.actionTwarz, SIGNAL(triggered(bool)), this, SLOT(Test_face_reg(bool)));
	connect(ui.actionOczyHough, SIGNAL(triggered(bool)), this, SLOT(Test_eyes_cht(bool)));
	connect(ui.actionVPF, SIGNAL(triggered(bool)), this, SLOT(Test_eyes_vpf(bool)));
	connect(ui.actionOczyApprox, SIGNAL(triggered(bool)), this, SLOT(Test_eyes_apx(bool)));
	connect(ui.actionHori, SIGNAL(triggered(bool)), this, SLOT(Test_Vpf_hori(bool)));
	connect(ui.actionVert, SIGNAL(triggered(bool)), this, SLOT(Test_Vpf_vert(bool)));
	connect(ui.actionNos, SIGNAL(triggered(bool)), this, SLOT(Test_nos(bool)));
	connect(ui.actionUsta, SIGNAL(triggered(bool)), this, SLOT(Test_usta(bool)));

#else
	ui.menuOpsy->setEnabled(false);
#endif

	ui.statusbar->addPermanentWidget(db_sb_lbl);
	db_sb_lbl->setPixmap(ui.actionDbRoz_cz->icon().pixmap(16, ui.statusbar->height()));
	db_sb_lbl->setToolTip(QString::fromUtf8("Nie połączono z BD"));

}

void FarecMainWin::Populate_toolbar()
{

	ui.ActionsToolbar -> addAction(ui.actionOtw_rz);
	ui.ActionsToolbar -> addAction(ui.actionU_yj_podgl_du);
	ui.OpstoolBar->addSeparator();
	ui.ActionsToolbar -> addAction(ui.actionPoka_odcinki);

#ifdef DEBUG_KRZYS
	ui.OpstoolBar->addAction(ui.actionAutoprep);
	ui.OpstoolBar->addSeparator();
	ui.OpstoolBar->addAction(ui.actionNa_szaro);
	ui.OpstoolBar->addAction(ui.actionKontrast);
	ui.OpstoolBar->addSeparator();
	ui.OpstoolBar->addAction(ui.actionSobel);
	ui.OpstoolBar->addAction(ui.actionOtsu);
	ui.OpstoolBar->addAction(ui.actionGauss);
	ui.OpstoolBar->addAction(ui.actionMedianowy);
	ui.OpstoolBar->addAction(ui.actionHough);
	ui.OpstoolBar->addAction(ui.action_redni);
	ui.OpstoolBar->addSeparator();
	ui.OpstoolBar->addAction(ui.actionTwarz);
	ui.OpstoolBar->addAction(ui.actionOczyHough);
	ui.OpstoolBar->addAction(ui.actionVPF);
	ui.OpstoolBar->addAction(ui.actionOczyApprox);
	ui.OpstoolBar->addSeparator();
	ui.OpstoolBar->addAction(ui.actionNos);
	ui.OpstoolBar->addAction(ui.actionUsta);
#else
	ui.OpstoolBar->setVisible(false);
#endif

	ui.DbToolbar->addAction(ui.actionDbPo_cz);
	ui.DbToolbar->addAction(ui.actionDbRoz_cz);

}

void FarecMainWin::Load_file( bool )
{
	QString fn = QFileDialog::getOpenFileName(this, QString::fromUtf8("Wybór pliku"), QDir::homePath(), tr(
			"Obrazy (*.png *.jpg *.jpeg *.bmp *.gif *.xpm *.xbm *.tif *.tiff);;Wszystko (*) "));

	QStringList fname = fn.split(QRegExp("[\\\\/]"), QString::SkipEmptyParts);

	if(not fn.isNull())
	{
		inimg.reset(new QImage());
		if(inimg->load(fn))
		{
			Set_label_img(ui.InputImgLbl, *inimg);
			if(fname.size() > 0)
			{
				ui.groupBox->setTitle(QString::fromUtf8("Obraz wejściowy [") + *(fname.end() - 1) + "] {"
						+ QString::number(inimg->width()) + QString::fromUtf8("×") + QString::number(
						inimg->height()) + "}");
			}

			ui.PviewImgLbl->setPixmap(QPixmap());
			outimg.reset();
		}
		else
		{
			QMessageBox::critical(this, QString::fromUtf8("Błąd"), QString::fromUtf8(
					"Błąd otwierania pliku:\n") + fn);
		}
	}
}

void FarecMainWin::Set_label_img( QLabel * lbl, QImage & img, bool pad )
{
	if(not img.isNull())
	{
		Qt::TransformationMode tm = Qt::SmoothTransformation;
		if(((img.width() * img.height()) / 1e6) > 0.7) // if img has more than 0.7Mpx (1024×768) then use fast transform & stuff
		{
			tm = Qt::FastTransformation;
		}

		lbl->setPixmap(QPixmap::fromImage(img).scaled(lbl->width() - (pad ? 5 : 0), lbl->height() - (pad ? 5
				: 0), Qt::KeepAspectRatio, tm));
	}
}

void FarecMainWin::Resize_labels_imgs( int p, int )
{
	bool pad = false;
	if(p != 0)
	{
		pad = true;
	}

	if(static_cast<bool> (inimg))
	{
		Set_label_img(ui.InputImgLbl, *inimg, pad);
	}

	if(static_cast<bool> (outimg))
	{
		Set_label_img(ui.PviewImgLbl, *outimg, pad);
	}
}

void FarecMainWin::Show_grads_src( bool )
{
	if(static_cast<bool> (inimg))
	{
		Show_grads(new QImage(*inimg));
	}
}

void FarecMainWin::Show_grads_pview( bool )
{
	if(static_cast<bool> (outimg))
	{
		Show_grads(new QImage(*outimg));
	}
}

void FarecMainWin::Show_grads( QImage * qi )
{
	scoped_ptr<GradientDisplay> gdsp(new GradientDisplay(qi, this));

	gdsp->exec();
}

void FarecMainWin::resizeEvent( QResizeEvent * )
{
	Resize_labels_imgs(0, 0);
}

void FarecMainWin::Make_preview_the_source( bool )
{
	if((not static_cast<bool> (outimg)) or (QMessageBox::question(this, "Zamiana", QString::fromUtf8(
			"Na pewno ustawić podgląd jako źródło?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)
			== QMessageBox::No))
	{
		return;
	}

	inimg.reset(new QImage(*outimg));
	Set_label_img(ui.InputImgLbl, *inimg);

	ui.groupBox->setTitle(QString::fromUtf8("Obraz wejściowy [Z podglądu] {") + QString::number(
			inimg->width()) + QString::fromUtf8("×") + QString::number(inimg->height()) + "}");

	ui.PviewImgLbl->setPixmap(QPixmap());
	outimg.reset();

}

void FarecMainWin::Show_segments( bool )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	Classifier cls(this, *inimg);
	cls.Classify();

	outimg.reset(new QImage(*inimg));

	QPainter qpt(outimg.get());
	srand(time(0));

	for(auto it = cls.Get_segs().constBegin(); it != cls.Get_segs().constEnd(); ++it)
	{
		qpt.setPen(QPen(QBrush(QColor(frand(255), frand(255), frand(255))), 2));
		qpt.drawLine(it.value()->get<3> (), it.value()->get<4> ());
	}

	qpt.end();

	Set_label_img(ui.PviewImgLbl, *outimg);
}

void FarecMainWin::Connect_to_db( bool )
{
	scoped_ptr<DbConnect> dbc(new DbConnect(fdb, this));
	dbc->exec();
	if(dbc->result() == 1)
	{
		if(fdb.Get_dbconn().isOpen() and fdb.Get_dbconn().isValid() and fdb.Get_dbconn().lastError().type()
				== 0)
		{
			ui.actionDbRoz_cz -> setEnabled(true);
			ui.actionDbPo_cz -> setEnabled(false);
			ui.actionDbPo_cz -> setChecked(true);
			ui.actionDbRoz_cz -> setChecked(false);

			db_sb_lbl->setPixmap(ui.actionDbPo_cz->icon().pixmap(16, ui.statusbar->height()));
			db_sb_lbl->setToolTip(
					QString::fromUtf8("Połączono z: %1://%2@%3:%4/%5").arg(fdb.Get_dbconn().driverName()).arg(fdb.Get_dbconn().userName()).arg(
							fdb.Get_dbconn().hostName()).arg(fdb.Get_dbconn().port()).arg(fdb.Get_dbconn().databaseName()));

		}
		else
		{
			QMessageBox::critical(this, QString::fromUtf8("Błąd połączenia"), QString::fromUtf8(
					"Błąd połączenia z BD:\n%1").arg(fdb.Get_dbconn().lastError().text()));

			ui.actionDbPo_cz -> setChecked(false);
			ui.actionDbRoz_cz -> setChecked(false);
			ui.actionDbRoz_cz -> setEnabled(false);
			ui.actionDbPo_cz -> setEnabled(true);
		}
	}
}

void FarecMainWin::Disconnect_from_db( bool )
{
	fdb.close();

	ui.actionDbPo_cz -> setChecked(false);
	ui.actionDbRoz_cz -> setChecked(false);
	ui.actionDbRoz_cz -> setEnabled(false);
	ui.actionDbPo_cz -> setEnabled(true);
	
	db_sb_lbl->setPixmap(ui.actionDbRoz_cz->icon().pixmap(16, ui.statusbar->height()));
	db_sb_lbl->setToolTip(QString::fromUtf8("Nie połączono z BD"));
}


void FarecMainWin::Add_person(bool)
{
	Person pn;
	PersonAdder pa(pn, PersonAdder::New, this);
	pa.exec();
}
