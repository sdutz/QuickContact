#include "about.h"
#include "ui_about.h"

//----------------------------------------------------------
about::about( QWidget *parent) : QDialog( parent), ui( new Ui::about)
{
    ui->setupUi( this) ;

    ui->info->setAlignment( Qt::AlignCenter) ;
    ui->info->setText( "Made by sdutz") ;
}

//----------------------------------------------------------
about::~about()
{
    delete ui ;
}
