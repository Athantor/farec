#ifndef PERSONCHOOSER_H
#define PERSONCHOOSER_H

#include <QtGui/QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include "ui_PersonChooser.h"

#include <boost/shared_ptr.hpp>

#include "libfarec/db/Person.hh"
#include "libfarec/db/FarecDb.hh"
#include "PersonAdder.hh"

using boost::shared_ptr;

class PersonChooser : public QDialog
{
	Q_OBJECT

	public:
		PersonChooser( Person &, FarecDb &, QWidget *parent = 0 );
		~PersonChooser();

	private slots:
		void Show_details();
		void Fill_data();

	private:
		Ui::PersonChooserClass ui;
		Person& pers;
		FarecDb & fdb;

		void Fill_cbx();
		shared_ptr<Person> Do_person( );
};

#endif // PERSONCHOOSER_H
