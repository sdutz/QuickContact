#include "about.h"
#include "ui_about.h"

//----------------------------------------------------------
about::about( QWidget *parent) : QDialog( parent), ui( new Ui::about)
{
    ui->setupUi( this) ;

    m_szText = "Made by sdutz" ;
    ui->info->setAlignment( Qt::AlignCenter) ;
    ui->info->setText( m_szText) ;

    m_nAlign.append( Qt::AlignCenter) ;
    m_nAlign.append( Qt::AlignLeft) ;
    m_nAlign.append( Qt::AlignRight) ;
    m_nAlign.append( Qt::AlignTop) ;
    m_nAlign.append( Qt::AlignBottom) ;
}

//----------------------------------------------------------
about::~about()
{
    delete ui ;
}

//----------------------------------------------------------
void
about::mousePressEvent( QMouseEvent* pEvent)
{
    if ( pEvent == nullptr) {
        return ;
    }

    ui->info->setAlignment( m_nAlign.at( qrand() % m_nAlign.count())) ;
    ui->info->setText( m_szText) ;
}
