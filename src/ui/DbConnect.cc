#include "DbConnect.hh"

DbConnect::DbConnect( FarecDb & conn, QWidget *parent ) :
	QDialog(parent), fdb(conn)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(accepted ()), this, SLOT(Open_conn()));
	connect(ui.buttonBox, SIGNAL(rejected ()), this, SLOT(Reject()));

	ui.statusLbl->setVisible(false);

#ifdef DEBUG_KRZYS
	ui.host->setCurrentIndex(0);
	ui.host->addItem("athantor.mooo.com");

	ui.login->setCurrentIndex(0);
	ui.login->addItem("farec");
	ui.login->addItem("athantor");
#endif

}

DbConnect::~DbConnect()
{
}

void DbConnect::Open_conn()
{
	if(ui.host->currentText().size() < 1)
	{
		QMessageBox::critical(this, QString::fromUtf8("Błąd"), QString::fromUtf8(
				"Pole hosta nie może być puste!"));
		return;
	}

	parentWidget()->setCursor(Qt::WaitCursor);
	setCursor(Qt::WaitCursor);

	ui.statusLbl->setVisible(true);
	update();
	repaint();

	fdb.open(ui.login->currentText(), ui.pass->text(), ui.host->currentText(), ui.port->value());

	emit accepted();

	setCursor(Qt::ArrowCursor);
	parentWidget()->setCursor(Qt::ArrowCursor);

	ui.statusLbl->setText(QString::fromUtf8("<b style='font-size: 12pt'>%1</b>").arg(
			fdb.Get_dbconn().isValid() ? "OK" : "FAIL"));

	done(1);
	//close();
}

void DbConnect::Reject()
{
	emit rejected();
	done(-1);
}
