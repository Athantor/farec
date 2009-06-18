#include "PersonChooser.hh"

PersonChooser::PersonChooser( Person & p, FarecDb &d, QWidget *parent ) :
	QDialog(parent), pers(p), fdb(d)
{
	ui.setupUi(this);

	Fill_cbx();
	
	connect(ui.DetailsTbn, SIGNAL(pressed()), this, SLOT(Show_details()));
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(Fill_data()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
	setResult(-1);
}

PersonChooser::~PersonChooser()
{

}

void PersonChooser::Fill_cbx()
{
	QSqlQuery que(fdb);

	if(not que.exec(
			"SELECT \"ID\", COALESCE(\"Surname\", '[Brak]') || ', ' || COALESCE(\"Name\", '[Brak]') || ': ' "
				"|| COALESCE(\"Address\", '[Brak]') FROM \"People\" ORDER BY \"Surname\" ASC, \"Name\" ASC, \"Address\" ASC;"))
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"),
				QString::fromUtf8("Błąd odczytu z DB:\n%1").arg(que.lastError().text()));

		ui.buttonBox->buttons()[0]->setEnabled(false);
		ui.DetailsTbn->setEnabled(false);

		return;
	}

	while(que.next())
	{
		if(que.isValid())
		{
			ui.PersonsCbx -> addItem(que.value(1).toString(), QVariant(que.value(0)));
		}
	}

	ui.buttonBox->buttons()[0]->setEnabled(true);
	ui.DetailsTbn->setEnabled(true);
}

void PersonChooser::Show_details()
{
	shared_ptr<Person> tmp = Do_person();
	
	if(tmp)
	{
		PersonAdder(*tmp, PersonAdder::Mode::View, this).exec();
	}

}

shared_ptr<Person> PersonChooser::Do_person(  )
{
	shared_ptr<Person> pn ;
	QSqlQuery que(fdb);
	que.prepare("SELECT * FROM \"People\" WHERE \"ID\" = ? ");
	que.bindValue(0, ui.PersonsCbx->itemData(ui.PersonsCbx->currentIndex()));

	if(not que.exec())
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"),
				QString::fromUtf8("Błąd odczytu z DB:\n%1").arg(que.lastError().text()));

		return pn;
	}

	if(que.first() and que.isValid())
	{
		QSqlRecord qsr = que.record();
		
		pn = shared_ptr<Person>(new Person(qsr.value("ID").toULongLong(0), QUuid(qsr.value("uuid").toString()), QDateTime(), 0, qsr.value(
					"Name").toString(), qsr.value("Surname").toString(), qsr.value("Address").toString(), qsr.value(
					"Comment").toString()));
	}
	else
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"),
						QString::fromUtf8("Błąd odczytu z DB:\n%1").arg(que.lastError().text()));
	}
	
	return pn;
}

void PersonChooser::Fill_data()
{
	pers = *Do_person();
	
	emit accept();
	done(1);
}
