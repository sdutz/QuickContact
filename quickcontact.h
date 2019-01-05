#pragma once

//----------------------------------------------------------
#include <QDialog>
#include <QSettings>
#include <QCloseEvent>

//----------------------------------------------------------
class QListWidgetItem ;

//----------------------------------------------------------
namespace Ui {
    class QuickContact;
}

//----------------------------------------------------------
class QuickContact : public QDialog
{
    Q_OBJECT

    public:
        explicit QuickContact( QWidget *parent = nullptr) ;
        ~QuickContact() ;

    protected:
        void setChildrenFont() ;
        void loadMap() ;
        void showMap( bool bInit = false) ;
        void setModified( bool bModified) ;
        void setTitle() ;
        void doEdit( int nRow) ;
        void showAll() ;
        void enablePrevNext( void) ;
        void goTo( const QString& szContact) ;

    private slots:
        void keyPressEvent( QKeyEvent* pEvent) ;
        void on_contacts_clicked( const QModelIndex &index) ;
        void on_filter_textChanged( const QString &arg1) ;
        void on_btnAdd_clicked() ;
        void on_btnEdit_clicked() ;
        void on_btnDelete_clicked() ;
        void on_btnSave_clicked() ;
        void on_btnAbout_clicked() ;
        void on_contacts_doubleClicked( const QModelIndex &index) ;
        void on_btnImp_clicked() ;
        void on_btnReset_clicked() ;
        void closeEvent( QCloseEvent* pEvent) ;
        void on_contacts_currentRowChanged( int currentRow);
        void on_btnNext_clicked();
        void on_QuickContact_accepted();
        void mouseDoubleClickEvent( QMouseEvent* pEvent) ;
        void on_btnPrev_clicked();

    private:
        bool              m_bMod ;
        int               m_nCurr ;
        QString           m_szKey ;
        QString           m_szTitle ;
        QString           m_szNumSep ;
        QString           m_szContSep ;
        QString           m_szLast ;
        QString           m_szWidth ;
        QString           m_szHeight ;
        QSettings         m_set ;
        QMap<QString,int> m_mapN ;
        QMap<QString,int> m_mapS ;
        QMap<QString, int>::iterator m_iter ;
        QList<QListWidgetItem*> m_found ;
        Ui::QuickContact  *ui ;
} ;
