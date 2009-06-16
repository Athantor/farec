#ifndef PERSONADDER_H
#define PERSONADDER_H

#include <QtGui/QDialog>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_PersonAdder.h"

#include "libfarec/db/Person.hh"

class PersonAdder : public QDialog
{
	Q_OBJECT

	public:

		enum Mode
		{
			New, Edit
		};

		PersonAdder( Person&, Mode, QWidget *parent = 0 );
		~PersonAdder();
		
	private slots:
		void Fill_data();
		void Load_image();
		void Save_image();
		void Clear_image();

	private:	
		Ui::PersonAdderClass ui;
		Mode mode;
		Person &psn;
		QString lastpath;
		
		void Fill_fields();

};

#endif // PERSONADDER_H
