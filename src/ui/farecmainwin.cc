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

#include <iostream>

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
	
	connect(ui.actionZapiszWej_cie, SIGNAL(triggered(bool)), this, SLOT(Save_file_in(bool)));
	connect(ui.actionZapiszPodgl_d, SIGNAL(triggered(bool)), this, SLOT(Save_file_out(bool)));

	connect(ui.action_Wyj_cie, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionU_yj_podgl_du, SIGNAL(triggered(bool)), this, SLOT(Make_preview_the_source(bool)));

	connect(ui.actionGradPvwWej_cie, SIGNAL(triggered(bool)), this, SLOT(Show_grads_src(bool)));
	connect(ui.actionGradPvwPodgl_d, SIGNAL(triggered(bool)), this, SLOT(Show_grads_pview(bool)));
	connect(ui.actionPoka_odcinki, SIGNAL(triggered(bool)), this, SLOT(Show_segments(bool)));

	connect(ui.actionDbPo_cz, SIGNAL(triggered(bool)), this, SLOT(Connect_to_db(bool)));
	connect(ui.actionDbRoz_cz, SIGNAL(triggered(bool)), this, SLOT(Disconnect_from_db(bool)));

	connect(ui.actionDodajTwarz, SIGNAL(triggered(bool)), this, SLOT(Add_face(bool)));
	connect(ui.actionWyszukajTwarz, SIGNAL(triggered(bool)), this, SLOT(Search_face(bool)));
	connect(ui.actionW_a_ciel, SIGNAL(triggered(bool)), this, SLOT(Add_person(bool)));

	connect(ui.actionO_Qt, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(aboutQt  ()));
	connect(ui.actionO, SIGNAL(triggered(bool)), this, SLOT(About_dlg(bool)));
	
	
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

	ui.tableView->setModel(srchmodel.get());
	
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

	Show_segments();

}

void FarecMainWin::Show_segments( shared_ptr<Classifier> cls )
{
	if(not cls)
	{
		cls = Make_classifier();
	}

	if(cls->Get_segs().size() < 1)
	{
		cls->Classify();
	}

	outimg.reset(new QImage(*inimg));

	QPainter qpt(outimg.get());
	srand(time(0));

	for(auto it = cls->Get_segs().constBegin(); it != cls->Get_segs().constEnd(); ++it)
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
			db_sb_lbl->setToolTip(QString::fromUtf8("Połączono z: %1://%2@%3:%4/%5").arg(
					fdb.Get_dbconn().driverName()).arg(fdb.Get_dbconn().userName()).arg(
					fdb.Get_dbconn().hostName()).arg(fdb.Get_dbconn().port()).arg(
					fdb.Get_dbconn().databaseName()));

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

void FarecMainWin::Add_person( bool )
{
	Persons(fdb, this).exec();
}

void FarecMainWin::Add_face( bool )
{
	Person pn;
	PersonChooser pc(pn, fdb, this);
	pc.exec();

	if(pc.result() < 1)
	{
		return;
	}

	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}

	shared_ptr<Classifier> cls = Make_classifier();

	fdb.Insert_facedata(cls->Get_segs(), pn.getId());

	Show_segments(cls);

}

void FarecMainWin::Search_face( bool )
{
	if(!static_cast<bool> (inimg) or inimg->isNull())
	{
		return;
	}
	
	if(not fdb.Get_dbconn().isOpen())
	{
		QMessageBox::critical(this, "SQL", QString::fromUtf8("Brak połączenia z bazą SQL"));
		return;
	}

	bool ok = false;
	QHash<QString, FarecDb::Srch_algo> amap;
	QStringList sl;
	sl << QString::fromUtf8("Średnia różnic") << QString::fromUtf8("Punkty"); 
	
	amap[sl[0]] = FarecDb::Srch_algo::Avg_diff;
	amap[sl[1]] = FarecDb::Srch_algo::Score;
	
	//algo.setLabelText(QString::fromUtf8("Algorytm poszukiwania:"));
	QString wat = QInputDialog::getItem(this, QString::fromUtf8("Algorytm poszukiwania:"), QString::fromUtf8("Algorytm poszukiwania:"), sl, 0, 0, &ok);

	if(!ok)
	{
		return;
	}
	
	double tol = QInputDialog::getDouble(this, "Tolerancja", QString::fromUtf8(
			"Procent w jakim wartości mogą odbiegać od wzorca:"), 10.0, 0, 100, 3, &ok);

	if(ok)
	{
		shared_ptr<Classifier> cls = Make_classifier();
		

		FarecDb::searchres_t srch = fdb.Find_faces(cls->Get_segs(), tol, amap[wat]);

		if(srch)
		{
			srchmodel.reset(new PplSrchModel(srch, fdb));
			ui.tableView->setModel(srchmodel.get());
		} else {
			srchmodel.reset();
			ui.tableView->setModel(srchmodel.get());
		}

		Show_segments(cls);
	}
}

void FarecMainWin::About_dlg(bool)
{
	AboutDialog::Show_me("FaRec", " ver. 1.0.", 
			QString::fromUtf8("© <a href='mailto:krzysztofkundzicz\u0040gmail.com'>"
					"Krzysztof Kundzicz</a>; 2009; "
					"Licensed under <a href='http://www.gnu.org/licenses/gpl-3.0.txt'>GPLv3</a>."), this);
}

void FarecMainWin::Save_file_in( bool )
{
	Save_file(false);
}

void FarecMainWin::Save_file_out( bool )
{
	Save_file(true);
}

void FarecMainWin::Save_file( bool output )
{
	QString fn = QFileDialog::getSaveFileName(this, QString::fromUtf8("Wybór pliku"), QDir::home().absoluteFilePath("farec-out.png"), 
			"Obrazy PNG (*.png);;Wszystko (*) ");
	
	if(not fn.isNull())
	{
		bool ret = true;
		
		QString suff = fn.toLower().endsWith(".png") ? "" : ".png";

		if(output)
		{
			if(outimg and not outimg->isNull())
			{
				ret = outimg->save(fn + suff, "png", 0);
			}
		}
		else
		{
			if(inimg and not inimg->isNull())
			{
				ret = inimg->save(fn + suff, "png", 0);
			}
		}
		
		if(not ret)
		{
			QMessageBox::critical(this, QString::fromUtf8("Bład"), QString::fromUtf8("Błąd zapisu pliku do:\n%1").arg(fn + suff));
		}
	}

}

QString FarecMainWin::Make_img_sha1(bool outi) const
{
	QImage *im = outi ? outimg.get() : inimg.get();
	
	if(im== NULL or im->isNull())
	{
		return QString();
	}
	
	QByteArray ba;
	QBuffer bf(&ba);
	
	bf.open(QIODevice::ReadWrite);
	
	im->save(&bf, "BMP", 0);
	
	bf.close();
	
	return QString(QCryptographicHash::hash(ba, QCryptographicHash::Sha1).toHex());
}

shared_ptr<Classifier> FarecMainWin::Make_classifier(bool outi)
{
	QImage *im = outi ? outimg.get() : inimg.get();
	shared_ptr<Classifier> cls;
	
	if(im == NULL or im->isNull())
	{
		return cls;
	}
	
	QString sha1=Make_img_sha1(false);
	auto chfnd = cache.find(sha1);
	
	if( chfnd != cache.end() and 
			QMessageBox::question(this,"Cache", 
			QString::fromUtf8("Użyć wartości zapamiętanych?"), 
			QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes ) 
	{
		cls = chfnd.value(); 
	} 
	else 
	{
		cls.reset(new Classifier(this, *inimg));
		cls->Classify();
		cache[sha1] = cls;
	}
	
	return cls;
}
