#include "farecmainwin.hh"

FarecMainWin::FarecMainWin( QWidget *parent ) :
	QMainWindow(parent), inimg(new QImage), outimg(new QImage)
{
	ui.setupUi(this);

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

#ifdef DEBUG_KRZYS

	ui.menuOpsy->setEnabled(true);

	connect(ui.actionNa_szaro, SIGNAL(triggered(bool)), this, SLOT(To_gray(bool)));
	connect(ui.actionSobel, SIGNAL(triggered(bool)), this, SLOT(Test_sobel(bool)));
#else
	ui.menuOpsy->setEnabled(false);
#endif


}

void FarecMainWin::Load_file( bool )
{
	QString fn = QFileDialog::getOpenFileName(this, tr("Wybór pliku"), QDir::homePath(), tr(
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
						+ QString::number(inimg->width()) + QString::fromUtf8("×") + QString::number(inimg->height())
						+ "}");
			}

			ui.PviewImgLbl->setPixmap(QPixmap());
			outimg.reset();
		}
		else
		{
			QMessageBox::critical(this, QString::fromUtf8("Błąd"), QString::fromUtf8("Błąd otwierania pliku:\n") + fn);
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

		lbl->setPixmap(QPixmap::fromImage(img).scaled(lbl->width() - (pad ? 5 : 0), lbl->height() - (pad ? 5 : 0),
				Qt::KeepAspectRatio, tm));
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

void FarecMainWin::resizeEvent( QResizeEvent * )
{
	Resize_labels_imgs(0, 0);
}
