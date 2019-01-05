#include "input.h"
#include "ui_input.h"

//----------------------------------------------------------
Input::Input( QWidget *parent) : QDialog( parent), ui(new Ui::Input)
{
    ui->setupUi(this);
}

//----------------------------------------------------------
Input::Input( const QString& szContact, int nNumber,
              QWidget *parent, int nShort) : QDialog( parent), ui(new Ui::Input)
{
    m_nShort = nShort ;
    m_nNumber = nNumber ;
    m_szContact = szContact ;
    ui->setupUi(this) ;
    ui->contact->setText( m_szContact) ;
    ui->number->setText( QString::number( m_nNumber)) ;
    ui->shortNumber->setText( QString::number( m_nShort)) ;
}

//----------------------------------------------------------
Input::~Input()
{
    delete ui;
}

//----------------------------------------------------------
bool
Input::GetData( QString* pszContact, int* pnNumber, int* pnShort)
{
    if ( pszContact == nullptr  ||  pnNumber == nullptr  ||  pnShort == nullptr) {
        return  false ;
    }

    *pnShort = m_nShort ;
    *pnNumber = m_nNumber ;
    *pszContact = m_szContact ;

    return true ;
}

//----------------------------------------------------------
void
Input::on_contact_textChanged(const QString &arg1)
{
    m_szContact = arg1 ;
}

//----------------------------------------------------------
void
Input::on_number_textChanged(const QString &arg1)
{
    m_nNumber = arg1.toInt() ;
}

//----------------------------------------------------------
void Input::on_shortNumber_textChanged(const QString &arg1)
{
    m_nShort = arg1.toInt() ;
}
