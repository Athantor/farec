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
 * CommonOps.hh
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#ifndef COMMONOPS_HH_
#define COMMONOPS_HH_

#include <QWidget>
#include <QProgressDialog>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

class CommonOps
{
	public:
		CommonOps(QWidget *);
		virtual ~CommonOps();

		shared_ptr<QProgressDialog> Get_pdialog() const;
		void Start_processing(const QString &, size_t);
		void Stop_processing();

	private:
		QWidget * pnt;
		shared_ptr<QProgressDialog> qpd;

};

#endif /* COMMONOPS_HH_ */
