#include "quickcontact.h"
#include "ui_quickcontact.h"

//----------------------------------------------------------
QuickContact::QuickContact(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuickContact)
{
    ui->setupUi(this);
}

//----------------------------------------------------------
QuickContact::~QuickContact()
{
    delete ui;
}

//----------------------------------------------------------
void
QuickContact::on_contacts_clicked(const QModelIndex &index)
{

}

//----------------------------------------------------------
void
QuickContact::on_filter_textChanged(const QString &arg1)
{

}

//----------------------------------------------------------
void
QuickContact::on_btnAdd_clicked()
{

}

//----------------------------------------------------------
void
QuickContact::on_btnEdit_clicked()
{

}

//----------------------------------------------------------
void
QuickContact::on_btnDelete_clicked()
{

}

//----------------------------------------------------------
void
QuickContact::on_btnSave_clicked()
{

}

//----------------------------------------------------------
void
QuickContact::on_btnAbout_clicked()
{

}

//----------------------------------------------------------
void
QuickContact::on_contacts_doubleClicked(const QModelIndex &index)
{

}

//----------------------------------------------------------
void
QuickContact::on_pushButton_clicked()
{

}

//----------------------------------------------------------
void
QuickContact::on_btnImp_clicked()
{

}
