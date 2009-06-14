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
#ifdef DEBUG_KRZYS
#include <QMainWindow>
#include <QStatusBar>
#endif

#include <stack>

#ifdef DEBUG_KRZYS
#include <sys/time.h>
#include <boost/cast.hpp>
#endif

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;
#ifdef DEBUG_KRZYS
using boost::polymorphic_cast;
#endif

class CommonOps
{
	public:
		CommonOps( QWidget * );
		virtual ~CommonOps();

		shared_ptr<QProgressDialog> Get_pdialog() const;
		void Start_processing( const QString &, size_t );
		void Stop_processing();

	private:
		QWidget * pnt;
		std::stack<shared_ptr<QProgressDialog> > qpds;
		size_t cursctr;

#ifdef DEBUG_KRZYS
		std::stack<shared_ptr<timeval> > times;

		inline static double tv2sec( const timeval& tv )
		{
			return tv.tv_sec + (tv.tv_usec * 1e-6);
		}
#endif

};

#endif /* COMMONOPS_HH_ */
