#pragma once

//----------------------------------------------------------
#include <QDialog>

//----------------------------------------------------------
namespace Ui {
    class about ;
}

//----------------------------------------------------------
class about : public QDialog
{
    Q_OBJECT

    public:
        explicit about( QWidget *parent = nullptr);
        ~about() ;

    private slots :
        void mousePressEvent( QMouseEvent* pEvent) ;

    private:
        Ui::about              *ui ;
        QString                m_szText ;
        QVector<Qt::Alignment> m_nAlign ;
} ;

