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
 *  Created on: 2009-05-31
 *      Author: athantor
 */

#ifndef GRADIENTDISPLAY_H
#define GRADIENTDISPLAY_H

#include <QtGui/QDialog>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QResizeEvent>

#include <cstdint>
#include <numeric>

#include <boost/scoped_ptr.hpp>

#include "ui_GradientDisplay.h"

#include "libfarec/ImgData.hh"
#include "libfarec/ImgOp.hh"

#include "Graph.hh"

using boost::scoped_ptr;

class GradientDisplay : public QDialog, private ImgOp
{
	Q_OBJECT

	public:
		GradientDisplay( QImage *, QWidget *parent = 0 );
		~GradientDisplay();

	private:
		Ui::GradientDisplayClass ui;
		scoped_ptr<QImage> theimg;

		void Setup_lbl(int32_t);
		void Setup_gph();
		void Disp_grads();
	protected:
		virtual void resizeEvent ( QResizeEvent * );
};

#endif // GRADIENTDISPLAY_H
