#ifndef PERSONS_H
#define PERSONS_H

#include <QtGui/QDialog>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QCloseEvent>
#include <QByteArray>
#include <QBuffer>

#include "ui_Persons.h"

#include "libfarec/db/Person.hh"
#include "PersonAdder.hh"

class Persons : public QDialog
{
	Q_OBJECT

	public:
		Persons( QSqlDatabase &, QWidget *parent = 0 );
		~Persons();
		virtual void closeEvent ( QCloseEvent * );
		
	public slots:
		void Add_user();
		void Edit_user();
		void Del_user();
	private slots:
		void Save_action();
		void Reject_action();
		
		void Enable_btns(const QModelIndex & );
		void Row_clicked(const QModelIndex &);

	private:
		Ui::PersonsClass ui;
		QSqlDatabase &mydb;
		QSqlTableModel *qstm;
		bool is_modified;
		
		void Person2Qsr(QSqlRecord&, Person &) const;
		void Put_pikczur_into_db(const QString&, const QImage&, bool=false);
		void Get_image_from_db(QImage&, const QString&);
		
};

#endif // PERSONS_H
