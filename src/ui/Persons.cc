#include "Persons.hh"

Persons::Persons( QSqlDatabase& d, QWidget *parent ) :
	QDialog(parent), mydb(d), qstm(new QSqlTableModel(this, d))
{
	ui.setupUi(this);

	qstm->setTable("People");
	qstm->setEditStrategy(QSqlTableModel::OnManualSubmit);

	if(not qstm->select())
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"), QString::fromUtf8(
				"Błąd przy zaczytywaniu DB:\n%1").arg(mydb.lastError().text()));

		ui.buttonBox->buttons()[0]->setEnabled(false);
		ui.buttonBox->buttons()[0]->setToolTip(QString::fromUtf8("Błąd komunikacji z DB"));

		ui.AddTbn->setEnabled(false);
	}

	ui.tableTvw->setModel(qstm);

	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(Save_action() ));
	connect(ui.buttonBox->buttons()[1], SIGNAL(pressed()), this, SLOT(Reject_action () ));

	connect(ui.AddTbn, SIGNAL(pressed()), this, SLOT(Add_user () ));
	connect(ui.EditTbn, SIGNAL(pressed()), this, SLOT(Edit_user () ));
	connect(ui.DelTbn, SIGNAL(pressed()), this, SLOT(Del_user() ));

	connect(ui.tableTvw, SIGNAL(clicked ( const QModelIndex & )), this,
			SLOT(Enable_btns(const QModelIndex &) ));
	
	connect(ui.tableTvw, SIGNAL(doubleClicked ( const QModelIndex & )), this,
				SLOT(Row_clicked(const QModelIndex &) ));
}

Persons::~Persons()
{
	delete qstm;
}

void Persons::Add_user()
{

	QSqlRecord qsr = qstm->database().record("People");

	if(qsr.isEmpty())
	{
		return;
	}

	Person pn;
	PersonAdder pa(pn, PersonAdder::New, this);
	pa.exec();
	if(pa.result() < 1)
	{
		return;
	}

	Person2Qsr(qsr, pn);

	if(not qstm->insertRecord(-1, qsr))
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"),
				QString::fromUtf8("Błąd dodawaniu wpisu:\n%1").arg(mydb.lastError().text()));
	}

	is_modified = true;

	Put_pikczur_into_db(QString::number(pn.getId()), pn.getImg());
}

void Persons::Person2Qsr( QSqlRecord& qsr, Person &pn ) const
{
	//qsr.setValue("ID", static_cast<qulonglong> (pn.getId()));
	qsr.setValue("Name", pn.getName());
	qsr.setValue("Surname", pn.getSurname());
	qsr.setValue("Address", pn.getAddr());
	qsr.setValue("Comment", pn.getComments());
	qsr.setValue("Name", pn.getName());
	qsr.setValue("uuid", static_cast<QString> (pn.getUuid()));
	qsr.setValue("time", "NOW()");
}

void Persons::closeEvent( QCloseEvent * evt )
{
	if(is_modified and QMessageBox::warning(this, QString::fromUtf8("Uwaga"), QString::fromUtf8(
			"W tabeli są niezapisane dane!\nWyjść?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No)
			== QMessageBox::Yes)
	{
		qstm->revertAll();
		evt->setAccepted(true);
	}
	else if(not is_modified)
	{
		evt->setAccepted(true);
	}
	else
	{
		evt->setAccepted(false);
	}
}

void Persons::Save_action()
{
	if(not qstm->submitAll())
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"),
				QString::fromUtf8("Błąd dodawania wpisu:\n%1").arg(mydb.lastError().text()));
	}
	else
	{
		is_modified = false;

	}

	emit
	accepted();
	done(1);
}

void Persons::Reject_action()
{
	if(is_modified and QMessageBox::warning(this, QString::fromUtf8("Uwaga"), QString::fromUtf8(
			"W tabeli są niezapisane dane!\nNa pewno cofnąć?"), QMessageBox::Yes | QMessageBox::No,
			QMessageBox::No) == QMessageBox::Yes)
	{
		qstm->revertAll();
		is_modified = false;
	}
	else if(not is_modified)
	{
		emit rejected();
		done(-1);
	}
}

void Persons::Enable_btns( const QModelIndex & )
{
	ui.AddTbn->setEnabled(true);
	ui.EditTbn->setEnabled(true);
	ui.DelTbn->setEnabled(true);
}

void Persons::Row_clicked(const QModelIndex&)
{
	Edit_user();
}

void Persons::Edit_user()
{
	QSqlRecord qsr = qstm->record(ui.tableTvw->currentIndex().row());

	if(qsr.isEmpty())
	{
		return;
	}
	Person pn(qsr.value("ID").toULongLong(0), QUuid(qsr.value("uuid").toString()), QDateTime(), 0, qsr.value(
			"Name").toString(), qsr.value("Surname").toString(), qsr.value("Address").toString(), qsr.value(
			"Comment").toString());

	QImage tmp;
	Get_image_from_db(tmp, QString::number(pn.getId())); //doesn't work
	pn.setImg(tmp);

	PersonAdder pa(pn, PersonAdder::Edit, this);
	pa.exec();
	
	if(pa.result() < 1)
	{
		return;
	}

	Person2Qsr(qsr, pn);

	if(not (is_modified = qstm->setRecord(ui.tableTvw->currentIndex().row(), qsr)))
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"),
				QString::fromUtf8("Błąd dodawaniu wpisu:\n%1").arg(mydb.lastError().text()));
	}

	Put_pikczur_into_db(QString::number(pn.getId()), pn.getImg());
}

void Persons::Del_user()
{
	if(QMessageBox::warning(this, QString::fromUtf8("Uwaga"), QString::fromUtf8("Na pewno usunąć wpis?"),
			QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
	{
		is_modified = qstm->removeRow(ui.tableTvw->currentIndex().row());
	}
}

void Persons::Put_pikczur_into_db( const QString& id, const QImage& img, bool )
{

	QSqlQuery query(mydb);
	
	if(img.isNull())
	{
		query.prepare("DELETE FROM \"Ppl_miniphotos\" WHERE \"Person\" = ?");
	} else {
		query.prepare("SELECT COUNT(\"Person\") FROM \"Ppl_miniphotos\" WHERE \"Person\" = ?");
		
	}
	query.bindValue(0, id);
	
		
	if(not query.exec() or (not img.isNull() and not query.first()) )
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"), QString::fromUtf8(
				"Błąd podczas dodawania zdjęcia do DB:\n%1").arg(query.lastError().text()));

		return;
	}
	
	if(img.isNull())
	{
		return;
	}

	QByteArray qba;
	QBuffer qb(&qba);
	
	qb.open(QIODevice::WriteOnly);
	img.save(&qb, "PNG", 0);
	qb.close();
	
	
	if(query.value(0) == 0)
	{
		QSqlQuery insq(mydb);
		insq.prepare(
				"INSERT INTO \"Ppl_miniphotos\" (\"Person\", \"time\", \"photo\") VALUES (?, NOW(), ? );");
		insq.bindValue(0, id);
		insq.bindValue(1, QString(qba.toBase64()));

		if(not insq.exec())
		{
			QMessageBox::critical(this, QString::fromUtf8("Błąd"), QString::fromUtf8(
					"Błąd podczas dodawania zdjęcia do DB:\n%1").arg(insq.lastError().text()));

			return;
		}
	}
	else
	{
		QSqlQuery insq(mydb);
		insq.prepare("UPDATE \"Ppl_miniphotos\" SET \"Person\" = ?, \"photo\" = '?', \"time\" = NOW();");
		insq.bindValue(0, id);
		insq.bindValue(1, QString(qba.toBase64()));
		
		if(not insq.exec())
		{
			QMessageBox::critical(this, QString::fromUtf8("Błąd"), QString::fromUtf8(
					"Błąd podczas dodawania zdjęcia do DB:\n%1").arg(insq.lastError().text()));

			return;
		}	
	}

}

void Persons::Get_image_from_db( QImage& im, const QString& id )
{
	QSqlQuery query(mydb);
	query.prepare("SELECT \"photo\" FROM \"Ppl_miniphotos\" WHERE \"Person\" = ?;");
	query.bindValue(0, id);

	if(not query.exec())
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"), QString::fromUtf8(
				"Błąd podczas odczytu zdjęcia z DB:\n%1").arg(query.lastError().text()));
	}
	else
	{
		if(query.first())
			im = QImage::fromData(QByteArray::fromBase64( query.value(0).toByteArray() ));
	}
}
