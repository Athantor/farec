#include "AboutDialog.hh"

AboutDialog::AboutDialog(QWidget *parent, const QString &n, const QString&v, const QString& nt, bool nofmt)
    : QDialog(parent), progname(n), progver(v), note(nt)
{
	ui.setupUi(this);
	
	if(nofmt)
	{
		ui.ProgName->setText(n);
		ui.ProgVer->setText(v);
		ui.Note->setText(v);
	}
	else
	{
		ui.ProgName->setText(QString::fromUtf8("<span style='font-weight:bold; font-size: 32pt;'>%1</span>").arg(n));
		ui.ProgVer->setText(QString::fromUtf8("<span style='font-weight:bold; font-size: 18pt; "
				"font-style:italic;'>— %1</span>").arg(v));
		ui.Note->setText(QString::fromUtf8("<span style='font-weight:normal; font-style:italic; "
				"font-size: 8pt; text-align:center;'>%1</span>").arg(nt));
		ui.Note->setAlignment(Qt::AlignHCenter);
	}
	
	connect(ui.buttonBox, SIGNAL(accepted  ()), this, SLOT(accept()));
	connect(ui.buttonBox, SIGNAL(rejected  ()), this, SLOT(accept()));
}

AboutDialog::~AboutDialog()
{

}

void AboutDialog::Show_me(const QString& nm, const QString& vr, const QString& nt, QWidget *pnt)
{
	AboutDialog(pnt, nm, vr, nt).exec();
}
