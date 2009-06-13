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

#include "GradientDisplay.hh"

GradientDisplay::GradientDisplay( QImage *im, QWidget *parent ) :
	QDialog(parent), ImgOp(this, *im), theimg(im)
{
	ui.setupUi(this);
	Setup_lbl(0);
	Setup_gph();
	Disp_grads();
}

GradientDisplay::~GradientDisplay()
{
}

void GradientDisplay::Disp_grads()
{

	if(not static_cast<bool> (theimg))
		return;

	
	cops->Start_processing("Przetwarzanie gradientów", theimg -> height() * theimg -> width());
	
	ImgData::dirgrads_t dg = ImgData(this, *theimg).Make_directional_gradients();

	ui.vgrad -> Set_data(dg->get<1>());
	ui.hgrad -> Set_data(dg->get<0>());

	cops->Stop_processing();
}

void GradientDisplay::Setup_lbl( int32_t pad )
{
	if(theimg->isNull())
		return;

	Qt::TransformationMode tm = (theimg->width() > 800 or theimg->height() > 600) ? Qt::FastTransformation
			: Qt::SmoothTransformation;

	ui.ImgPrv->setPixmap(QPixmap::fromImage(*theimg).scaled(ui.ImgPrv->width() + (pad), ui.ImgPrv->height()
			+ pad, Qt::IgnoreAspectRatio, tm));
}

void GradientDisplay::Setup_gph()
{
	QPen qp(QColor("black"));
	qp.setCapStyle(Qt::FlatCap);
	qp.setCosmetic(true);
	qp.setWidth(1);
	qp.setStyle(Qt::SolidLine);

	QBrush qb(QColor("white"), Qt::SolidPattern);

	ui.hgrad->Set_brush(qb);
	ui.hgrad->Set_pen(qp);
	ui.hgrad->Set_gdir(Graph::LTR);
	ui.hgrad->Set_buddy(ui.ImgPrv);
	ui.hgrad->setStyleSheet(ui.hgrad->styleSheet() + " background-color: #fff; ");

	ui.vgrad->Set_brush(qb);
	ui.vgrad->Set_pen(qp);
	ui.vgrad->Set_gdir(Graph::UTD);
	ui.vgrad->Set_buddy(ui.ImgPrv);
	ui.vgrad->setStyleSheet(ui.vgrad->styleSheet() + " background-color: #fff; ");

}

void GradientDisplay::resizeEvent( QResizeEvent * evt )
{
	int32_t wdh = (evt->size() - evt->oldSize()).width();
	int32_t hgt = (evt->size() - evt->oldSize()).height();
	int32_t &wat = wdh != 0 ? wdh : hgt;

	Setup_lbl(wat < 0 ? wat < -10 ? -10 : wat : 0);
}

