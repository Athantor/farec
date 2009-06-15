#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QtGui/QDialog>
#include <QMessageBox>
#include "ui_DbConnect.h"

#include <boost/shared_ptr.hpp>

#include "libfarec/db/FarecDb.hh"

using boost::shared_ptr;

class DbConnect : public QDialog
{
	Q_OBJECT

	public:	
		DbConnect( FarecDb & conn, QWidget *parent = 0 );
		~DbConnect();

	private:
		Ui::DbConnectClass ui;
		FarecDb & fdb;
	
	private slots:
		void Open_conn();
		void Reject();
};

#endif // DBCONNECT_H
