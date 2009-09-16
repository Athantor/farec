#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>
#include "ui_AboutDialog.h"

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    AboutDialog(QWidget *parent, const QString &, const QString&, const QString&, bool = false);
    ~AboutDialog();

    static void Show_me(const QString&, const QString&, const QString&, QWidget * = 0);
    
private:
    Ui::AboutDialog ui;
    
    const QString progname, progver, note;
};

#endif // ABOUTDIALOG_H
