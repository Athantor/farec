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
 * CommonOps.cc
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#include "CommonOps.hh"

CommonOps::CommonOps(QWidget *p) : pnt(p)
{
	// TODO Auto-generated constructor stub

}

CommonOps::~CommonOps()
{
	// TODO Auto-generated destructor stub
}


shared_ptr<QProgressDialog> CommonOps::Get_pdialog() const
{
	return qpd;
}

void CommonOps::Start_processing( const QString & desc, size_t max )
{
	pnt->setCursor(QCursor(Qt::WaitCursor));

	qpd.reset(new QProgressDialog(desc, "", 0, max, pnt));
	qpd->setCancelButton(0);
	qpd->setValue(0);
	qpd->setAutoClose(true);

	qpd->show();
}

void CommonOps::Stop_processing()
{
	qpd->close();
	qpd.reset();

	pnt->setCursor(QCursor(Qt::ArrowCursor));
}
