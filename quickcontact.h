#pragma once

#include <QDialog>
#include <QSettings>

namespace Ui {
class QuickContact;
}

class QuickContact : public QDialog
{
    Q_OBJECT

    public:
        explicit QuickContact(QWidget *parent = nullptr);
        ~QuickContact();

    private slots:
        void on_contacts_clicked(const QModelIndex &index);
        void on_filter_textChanged(const QString &arg1);
        void on_btnAdd_clicked();
        void on_btnEdit_clicked();
        void on_btnDelete_clicked();
        void on_btnSave_clicked();
        void on_btnAbout_clicked();
        void on_contacts_doubleClicked(const QModelIndex &index);
        void on_pushButton_clicked();
        void on_btnImp_clicked();

    private:
        Ui::QuickContact *ui;
        QSettings        m_Set ;

};
