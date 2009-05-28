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

CommonOps::CommonOps( QWidget *p ) :
	pnt(p), cursctr(0)
{
}

CommonOps::~CommonOps()
{
}

shared_ptr<QProgressDialog> CommonOps::Get_pdialog() const
{
	return qpds.top();
}

void CommonOps::Start_processing( const QString & desc, size_t max )
{
	if(++cursctr >= 1)
		pnt->setCursor(QCursor(Qt::WaitCursor));

	shared_ptr<QProgressDialog> qpd(new QProgressDialog(desc.size() ? desc : QString::fromUtf8("Przetwarzanie…"), "",
			0, max, pnt));
	qpds.push(qpd);

	qpd->setCancelButton(0);
	qpd->setValue(0);
	qpd->setAutoClose(true);

	qpd->show();
}

void CommonOps::Stop_processing()
{

	shared_ptr<QProgressDialog> qpd = qpds.top();

	qpd->close();
	qpd.reset();

	if(--cursctr <= 1)
		pnt->setCursor(QCursor(Qt::ArrowCursor));
}
