#pragma once

#include <QDialog>

//----------------------------------------------------------
namespace Ui {
    class Input ;
}

//----------------------------------------------------------
class Input : public QDialog
{
    Q_OBJECT

    public:
        explicit Input( QWidget *parent = nullptr) ;
        explicit Input( const QString& szContact, int nNumber,
                        QWidget *parent = nullptr, int nShort = 0) ;
        ~Input();
        bool GetData( QString* pszContact, int* pnNumber, int* pnShort) ;

    private slots:
        void on_contact_textChanged(const QString &arg1);
        void on_number_textChanged(const QString &arg1);
        void on_shortNumber_textChanged(const QString &arg1);

    private:
        Ui::Input *ui ;
        int       m_nShort ;
        int       m_nNumber ;
        QString   m_szContact ;
} ;
