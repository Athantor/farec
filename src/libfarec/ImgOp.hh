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
 * ImgOp.hh
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#ifndef IMGOP_HH_
#define IMGOP_HH_

#include <QImage>
#include <QWidget>


#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "CommonOps.hh"

using boost::scoped_ptr;
using boost::shared_ptr;

class ImgOp
{
	public:
		const static size_t CHT_CIRCNUM = 50; 
		typedef shared_ptr<QImage> ret_t;

		virtual ~ImgOp();

		virtual void Change_img(const QImage&);
		
		void Set_do_preps(bool dp);
		bool Get_do_preps() const;

	protected:
		ImgOp() = delete;
		ImgOp(QWidget *, const QImage&);
		ImgOp(const ImgOp &);
		ImgOp& operator=( const ImgOp& );
		QWidget *pnt;
		scoped_ptr<CommonOps> cops;
		ret_t myimg;
		bool dopreps;

};

#endif /* IMGOP_HH_ */
