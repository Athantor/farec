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
 * ImgOp.cc
 *
 *  Created on: 2009-05-24
 *      Author: athantor
 */

#include "ImgOp.hh"

ImgOp::ImgOp( QWidget * p, const QImage & qi ) :
	pnt(p), cops(new CommonOps(p)), myimg(new QImage(qi)), dopreps(true)
{
}

ImgOp::ImgOp( const ImgOp & oth )
{
	pnt = oth.pnt;
	cops.reset(new CommonOps(pnt));
	myimg = decltype(myimg)(new QImage(*oth.myimg));
	dopreps = oth.dopreps;
}

ImgOp& ImgOp::operator=( const ImgOp& oth )
{
	if(this == &oth)
		return *this;

	pnt = oth.pnt;
	cops.reset(new CommonOps(pnt));
	myimg = decltype(myimg)(new QImage(*oth.myimg));
	dopreps = oth.dopreps;

	return *this;
}

ImgOp::~ImgOp()
{
}

void ImgOp::Change_img( const QImage& qi )
{
	myimg.reset(new QImage(qi));
}

bool ImgOp::Get_do_preps() const
{
	return dopreps;
}

void ImgOp::Set_do_preps( bool dp )
{
	dopreps = dp;
}
