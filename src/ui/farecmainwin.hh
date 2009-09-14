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

#ifndef FARECMAINWIN_H
#define FARECMAINWIN_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QSqlError>
#include <QCryptographicHash>
#include <QBuffer>

#ifdef DEBUG_KRZYS
#include <QInputDialog>
#endif

#include <boost/scoped_ptr.hpp>
#include <boost/cast.hpp>
#include <boost/shared_ptr.hpp>

#include <time.h>

#include "ui_farecmainwin.h"

#include "libfarec/ImgPrep.hh"
#include "libfarec/FeatExtract.hh"
#include "libfarec/Classifier.hh"

#include "libfarec/db/FarecDb.hh"
#include "libfarec/db/Person.hh"

#include "GradientDisplay.hh"
#include "DbConnect.hh"
#include "Persons.hh"
#include "PersonChooser.hh"

using boost::scoped_ptr;
using boost::polymorphic_downcast;

inline uint64_t frand( uint64_t max = RAND_MAX )
{
  return static_cast<double> (rand()) / ((static_cast<double> (RAND_MAX) + 1.0) / (static_cast<double> (max)));
}

class FarecMainWin : public QMainWindow
{
	Q_OBJECT

	public:
		FarecMainWin( QWidget *parent = 0 );
		~FarecMainWin();

	public slots:
		void Load_file( bool );
		void Connect_to_db( bool );
		void Disconnect_from_db(bool);
		void Add_person(bool);
		void Save_file_in(bool);
		void Save_file_out(bool);
		void Add_face(bool);
		void Search_face(bool);

	private slots:
		void Resize_labels_imgs( int, int );
		void Make_preview_the_source( bool );
		void Show_grads_src( bool );
		void Show_grads_pview( bool );
		void Show_segments(bool);

#ifdef DEBUG_KRZYS
		void Test_autoprep( bool );
		void To_gray( bool );
		void Contrast( bool );
		void Test_sobel( bool );
		void Test_otsu( bool );
		void Test_gauss( bool );
		void Test_median( bool );
		void Test_hough( bool );
		void Test_avg( bool );
		void Test_face_reg( bool );
		void Test_eyes_cht( bool );
		void Test_eyes_vpf( bool );
		void Test_eyes_apx( bool );
		void Test_Vpf_hori( bool );
		void Test_Vpf_vert( bool );
		void Test_nos( bool );
		void Test_usta( bool );
#endif

	private:
		Ui::FarecMainWinClass ui;
		FarecDb fdb;
		QLabel *db_sb_lbl;
		//     sha1         data
		QHash<QString, shared_ptr<Classifier> > cache;

		void Connect_slots();
		void Populate_toolbar();

		void Set_label_img( QLabel *, QImage &, bool = false );

		void Show_grads( QImage * );
		void Show_segments(shared_ptr<Classifier> = shared_ptr<Classifier>() );
		
		QString Make_img_sha1(bool = false);

		virtual void resizeEvent( QResizeEvent * );

		void Save_file(bool);
		
		scoped_ptr<QImage> inimg, outimg;

#ifdef DEBUG_KRZYS
		void Test_vpf( ImgData::Vpf_dir );
		void Test_vpf_eye( ImgData::Vpf_dir, const QRect &, ImgData& );

		void Draw_eye_shape( shared_ptr<FeatExtract::eyeloc_t>*, QPainter &, FeatExtract::cht_eyeloc_t );
#endif
};

#endif // FARECMAINWIN_H
