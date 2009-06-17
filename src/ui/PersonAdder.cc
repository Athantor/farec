#include "PersonAdder.hh"

PersonAdder::PersonAdder( Person& p, Mode m, QWidget *parent ) :
	QDialog(parent), mode(m), psn(p), lastpath(QDir::homePath())
{
	ui.setupUi(this);

	connect(ui.LoadImgTbn, SIGNAL(pressed ()), this, SLOT(Load_image()));
	connect(ui.SaveImgTbn, SIGNAL(pressed ()), this, SLOT(Save_image()));
	connect(ui.DelImgTbn, SIGNAL(pressed ()), this, SLOT(Clear_image()));

	connect(ui.buttonBox, SIGNAL(accepted ()), this, SLOT(Fill_data()));
	connect(ui.buttonBox, SIGNAL(rejected ()), this, SLOT(reject()));

	Fill_fields();
	
	setResult(-1);

}

PersonAdder::~PersonAdder()
{
}

void PersonAdder::Fill_data()
{
	psn.setUuid(QUuid(ui.UuidLe->text()));
	psn.setId(ui.IdLe->text().toLongLong());
	psn.setName(ui.NameLe->text());
	psn.setSurname(ui.snameLe->text());
	psn.setAddr(ui.AddrLe->text());
	psn.setComments(ui.CmtPte->document()->toPlainText());

	if(ui.PicLbl->pixmap() and not ui.PicLbl->pixmap()->isNull())
	{
		psn.setImg(ui.PicLbl->pixmap()->toImage());
	}

	emit
	accepted();
	done(1);
}

void PersonAdder::Fill_fields()
{
	if(mode == Mode::New)
	{
		this->setWindowTitle(QString::fromUtf8("Nowy wpis"));
		
		ui.UuidLe->setText(QUuid::createUuid());
		ui.IdLe->setText("0");
	}
	else if(mode == Mode::Edit or mode == Mode::View)
	{
		this->setWindowTitle(QString::fromUtf8("Edycja"));
		
		ui.UuidLe->setText(psn.getUuid());
		ui.IdLe->setText(QString::number(psn.getId()));
		ui.NameLe->setText(psn.getName());
		ui.snameLe->setText(psn.getSurname());
		ui.AddrLe->setText(psn.getAddr());
		ui.CmtPte->document()->setPlainText(psn.getComments());
		ui.PicLbl->setPixmap(QPixmap::fromImage(QImage(psn.getImg()).scaled(ui.PicLbl->width(),
				ui.PicLbl->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));

		if(mode == Mode::Edit and not psn.getImg().isNull())
		{
			ui.SaveImgTbn->setEnabled(true);
			ui.DelImgTbn->setEnabled(true);
		}
		
		if(mode == Mode::View)
		{
			this->setWindowTitle(QString::fromUtf8("Podgląd"));
			
			ui.LoadImgTbn->setEnabled(false);
			ui.DelImgTbn->setEnabled(false);
			
			ui.NameLe->setReadOnly(true);
			ui.snameLe->setReadOnly(true);
			ui.AddrLe->setReadOnly(true);
			ui.CmtPte->setReadOnly(true);
			ui.UuidLe->setReadOnly(true);
			
			ui.buttonBox->buttons()[1]->setVisible(false);
			
		}
	}
}

void PersonAdder::Load_image()
{
	QString path = QFileDialog::getOpenFileName(this, QString::fromUtf8("Wybór zdjęcia"), lastpath,
			QString::fromUtf8(
					"Obrazy (*.jpg *.jpeg *.png *.gif *.bmp *.tif *.tiff *.xpm *.pbm);;Wszystkie (*)"));

	if(path.size() > 0)
	{
		QImage tmp(path);
		if(not tmp.isNull())
		{
			ui.PicLbl->setPixmap(QPixmap::fromImage(QImage(tmp).scaled(ui.PicLbl->width(),
					ui.PicLbl->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
			lastpath = QDir(path).absolutePath();

			ui.SaveImgTbn->setEnabled(true);
			ui.DelImgTbn->setEnabled(true);
		}
		else
		{
			ui.SaveImgTbn->setEnabled(false);
			ui.DelImgTbn->setEnabled(false);
		}
	}
}

void PersonAdder::Save_image()
{

	if(not ui.PicLbl->pixmap() or ui.PicLbl->pixmap()->isNull())
	{
		return;
	}

	QString tmppth = QDir(lastpath).absoluteFilePath("farec.png");

	QString path = QFileDialog::getSaveFileName(this, QString::fromUtf8("Wybór miejsca zapisu"), tmppth,
			QString::fromUtf8("Obrazy PNG (*.png);;Wszystkie (*)"));

	tmppth = tmppth.endsWith(".png") ? tmppth : tmppth + ".png";

	if(path.size() > 0)
	{
		if(ui.PicLbl->pixmap()->toImage().save(path, "png", 0))
		{
			lastpath = QDir(path).absolutePath();
		}
		else
		{
			QMessageBox::warning(this, QString::fromUtf8("Błąd"), QString::fromUtf8(
					"Zapis obrazu do\n„%1”\nnie powiódł się!").arg(path));
		}
	}
}

void PersonAdder::Clear_image()
{
	if(not ui.PicLbl->pixmap() or ui.PicLbl->pixmap()->isNull())
	{
		return;
	}

	if(QMessageBox::question(this, "Usuwanie", QString::fromUtf8("Czy na pewno usunąć?"), QMessageBox::Yes
			| QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
	{
		ui.PicLbl->setPixmap(QPixmap());
		ui.PicLbl->setText("<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px;"
			" margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:28pt; "
			" font-weight:600;\">-</span></p>");

		ui.SaveImgTbn->setEnabled(false);
		ui.DelImgTbn->setEnabled(false);
	}
}
