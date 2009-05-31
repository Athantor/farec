#ifndef FARECMAINWIN_H
#define FARECMAINWIN_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>

#ifdef DEBUG_KRZYS
#include <QInputDialog>
#endif

#include <boost/scoped_ptr.hpp>
#include <boost/cast.hpp>

#include "ui_farecmainwin.h"

#include "libfarec/ImgPrep.hh"

using boost::scoped_ptr;
using boost::polymorphic_downcast;

class FarecMainWin : public QMainWindow
{
	Q_OBJECT

	public:
		FarecMainWin( QWidget *parent = 0 );
		~FarecMainWin();

	public slots:
		void Load_file( bool );

	private slots:
		void Resize_labels_imgs(int, int);
		void Make_preview_the_source( bool );

#ifdef DEBUG_KRZYS
		void To_gray( bool );
		void Test_sobel( bool );
		void Test_otsu(bool);
		void Test_gauss(bool);
		void Test_median(bool);
#endif

	private:
		Ui::FarecMainWinClass ui;

		void Connect_slots();
		void Set_label_img( QLabel *, QImage & , bool = false);

		virtual void resizeEvent(QResizeEvent *);

		scoped_ptr<QImage> inimg, outimg;
};

#endif // FARECMAINWIN_H
