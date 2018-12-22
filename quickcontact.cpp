#include "quickcontact.h"
#include "ui_quickcontact.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QTextStream>
#include "about.h"

//----------------------------------------------------------
QuickContact::QuickContact( QWidget *parent) : QDialog( parent), ui( new Ui::QuickContact)
{
    ui->setupUi( this) ;
    m_nCurr = 0 ;
    m_bMod  = false ;
    m_szKey = "contacts" ;
    m_szLast = "last" ;
    m_szNumSep  = ":" ;
    m_szContSep = "," ;
    m_szTitle   = "Contacts" ;
    setChildrenFont() ;
    loadMap() ;
    showMap( true) ;
    setTitle() ;
    ui->btnSave->setEnabled( false) ;
    ui->btnNext->setEnabled( false) ;
    ui->btnReset->setEnabled( false) ;
    ui->filter->setFocus() ;
}

//----------------------------------------------------------
QuickContact::~QuickContact()
{
    delete ui ;
}

//----------------------------------------------------------
void
QuickContact::on_contacts_clicked( const QModelIndex &index)
{
    QListWidgetItem* pItem = ui->contacts->item( index.row()) ;

    ui->number->display( m_map.value( pItem->text())) ;
}

//--------------------------------d--------------------------
void
QuickContact::showAll()
{
    QListWidgetItem* pITem ;

    for ( int i = 0 ;  i < ui->contacts->count() ;  i ++) {
        pITem  = ui->contacts->item( i) ;
        if ( pITem == nullptr) {
            continue ;
        }
        ui->contacts->setItemHidden( pITem, false) ;
    }
}

//----------------------------------------------------------
void
QuickContact::on_filter_textChanged( const QString &arg1)
{
    if ( arg1.isEmpty()) {
        ui->btnNext->setEnabled( false) ;
        ui->btnReset->setEnabled( false) ;
        showAll() ;
        return ;
    }
    else {
        ui->btnReset->setEnabled( true) ;
    }
    m_found.clear() ;
    bool bOk ;
    int num = arg1.toInt( &bOk) ;
    if ( bOk) {
        QString res = m_map.key( num) ;
        if ( ! res.isEmpty()) {
            m_found = ui->contacts->findItems( res, Qt::MatchExactly) ;
        }
    }
    else {
        m_found = ui->contacts->findItems( arg1, Qt::MatchContains) ;
    }

    if ( ! m_found.isEmpty()) {
        m_nCurr = 0 ;
        ui->contacts->setCurrentItem( m_found.first()) ;
        ui->btnNext->setEnabled( true) ;
    }
    else {
        ui->btnNext->setEnabled( false) ;
    }

    QListWidgetItem* pITem ;
    for ( int i = 0 ;  i < ui->contacts->count() ;  i ++) {
        pITem  = ui->contacts->item( i) ;
        if ( pITem == nullptr) {
            continue ;
        }
        ui->contacts->setItemHidden( pITem, ! m_found.contains( pITem)) ;
    }
}

//----------------------------------------------------------
void
QuickContact::on_btnAdd_clicked()
{
    QString szContact = QInputDialog::getText( this, m_szTitle, tr( "Insert contact")) ;
    if( szContact.isEmpty()) {
        return ;
    }

    int nNum = QInputDialog::getInt( this, m_szTitle, tr( "Insert number")) ;
    if ( nNum == 0) {
        return ;
    }

    m_map.insert( szContact, nNum) ;

    ui->contacts->addItem( szContact) ;
    ui->number->display( nNum) ;

    setTitle() ;
    setModified( true) ;
}

//----------------------------------------------------------
void
QuickContact::on_btnEdit_clicked()
{
    doEdit( ui->contacts->currentRow()) ;
}

//----------------------------------------------------------
void
QuickContact::doEdit( int nRow)
{
    QListWidgetItem* pCurr = ui->contacts->item( nRow) ;

    if ( pCurr == nullptr) {
        return ;
    }

    QString szOld = pCurr->text() ;
    int nOld = m_map.value( szOld) ;

    QString szNew = QInputDialog::getText( this, m_szTitle, tr( "Modify contact"),
                                           QLineEdit::Normal, szOld) ;
    if ( szNew.isEmpty()) {
        return ;
    }

    int nNew = QInputDialog::getInt( this, m_szTitle, tr( "Modify number"), nOld) ;
    if ( nNew == 0) {
        return ;
    }

    if ( szOld == szNew  &&  nOld == nNew) {
        return ;
    }

    pCurr->setText( szNew) ;
    m_map.remove( szOld) ;
    m_map.insert( szNew, nNew) ;

    ui->number->display( nNew) ;

    setModified( true) ;
}

//----------------------------------------------------------
void
QuickContact::on_btnDelete_clicked()
{
    int nRow = ui->contacts->currentRow() ;
    QListWidgetItem* pCurr = ui->contacts->item( nRow) ;

    if ( pCurr == nullptr) {
        return ;
    }

    QString szCurr = pCurr->text() ;

    m_map.remove( szCurr) ;
    ui->contacts->takeItem( nRow) ;

    setTitle() ;
    setModified( true) ;
}

//----------------------------------------------------------
void
QuickContact::on_btnSave_clicked()
{
    QString szVal ;

    for ( m_iter = m_map.begin() ;  m_iter != m_map.end() ;  ++ m_iter) {
        szVal += m_iter.key() + m_szNumSep + QString::number( m_iter.value()) + m_szContSep ;
    }

    m_set.setValue( m_szKey, szVal) ;

    setModified( false) ;
}

//----------------------------------------------------------
void
QuickContact::on_btnAbout_clicked()
{
    about a ;

    a.setWindowTitle( m_szTitle) ;

    a.exec() ;
}

//----------------------------------------------------------
void
QuickContact::on_contacts_doubleClicked( const QModelIndex &index)
{
    doEdit( index.row()) ;
}

//----------------------------------------------------------
void
QuickContact::on_btnImp_clicked()
{
    QString szFile = QFileDialog::getOpenFileName( this, tr( "Select contacts file"), "", "*.txt") ;

    if ( szFile.isEmpty()) {
        return ;
    }

    QFile file( szFile) ;
    if ( ! file.open( QIODevice::ReadOnly | QIODevice::Text)) {
        return ;
    }

    QStringList slTokens ;
    QTextStream stream( &file) ;
    int nOldSize = m_map.count() ;

    while( ! stream.atEnd()) {
        slTokens = stream.readLine().split( m_szNumSep) ;
        if ( slTokens.size() < 2) {
            continue ;
        }
        m_map.insert( slTokens.first(), slTokens.last().toInt()) ;
    }

    file.close() ;

    if ( m_map.count() != nOldSize) {
        showMap() ;
        setModified( true) ;
        setTitle() ;
    }
}

//----------------------------------------------------------
void QuickContact::on_btnReset_clicked()
{
    ui->filter->clear() ;
    ui->btnNext->setEnabled( false) ;
    ui->btnReset->setEnabled( false) ;
    showAll() ;
}

//----------------------------------------------------------
void
QuickContact::showMap( bool bInit /*= false*/)
{
    int         nIdx ;
    QString     szCurr ;
    QStringList lszKeys ;

    ui->contacts->clear() ;

    if ( bInit) {
        szCurr = m_set.value( m_szLast, "").toString() ;
    }

    lszKeys = m_map.keys() ;
    for ( nIdx = 0 ;  nIdx < lszKeys.count() ;  nIdx ++) {
        ui->contacts->addItem( lszKeys.at( nIdx)) ;
    }

    if ( m_map.count() > 0) {
        if ( szCurr.isEmpty()) {
            ui->contacts->setCurrentRow( 0) ;
        }
        else {
            m_found = ui->contacts->findItems( szCurr, Qt::MatchExactly) ;
            if ( m_found.count() == 1) {
                ui->contacts->setFocus() ;
                ui->contacts->setCurrentRow( ui->contacts->row( m_found.first())) ;
            }
        }
    }

    ui->contacts->sortItems() ;
}

//----------------------------------------------------------
void
QuickContact::loadMap()
{
    QStringList lszContacts = m_set.value( m_szKey, "").toString().split( m_szContSep) ;

    foreach( QString szContact, lszContacts) {
        if ( ! szContact.isEmpty()) {
            QStringList lszContact = szContact.split( m_szNumSep) ;
            m_map.insert( lszContact.first(), lszContact.last().toInt()) ;
        }
    }
}

//----------------------------------------------------------
void
QuickContact::setChildrenFont()
{
    QFont font ;

    font.setPointSize( font.pointSize() + 6) ;

    ui->filter->setFont( font) ;
    ui->contacts->setFont( font) ;
}

//----------------------------------------------------------
void
QuickContact::closeEvent( QCloseEvent* pEvent)
{
    if ( pEvent == nullptr) {
        return ;
    }

    if ( ! m_bMod) {
        on_QuickContact_accepted() ;
        pEvent->accept();
    }
    else {
        int nRet = QMessageBox::question( this, tr("Save"), tr("Do you want to save your changes?"),
                                          QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel) ;
        switch ( nRet) {
            case QMessageBox::Cancel :
                pEvent->ignore() ;
            break ;
            case QMessageBox::Yes :
                on_QuickContact_accepted() ;
                on_btnSave_clicked() ;
                pEvent->accept() ;
            break ;
            case QMessageBox::No :
                on_QuickContact_accepted() ;
                pEvent->accept() ;
            break ;
        }
    }
}

//----------------------------------------------------------
void
QuickContact::on_contacts_currentRowChanged( int currentRow)
{
    QListWidgetItem* pItem = ui->contacts->item( currentRow) ;

    if ( pItem != nullptr) {
        ui->number->display( m_map.value( pItem->text())) ;
    }
}

//----------------------------------------------------------
void
QuickContact::setModified( bool bModified)
{
    m_bMod = bModified ;
    ui->btnSave->setEnabled( bModified) ;
}

//----------------------------------------------------------
void
QuickContact::setTitle()
{
    setWindowTitle( m_szTitle + " : " + QString::number( m_map.count())) ;
}

//----------------------------------------------------------
void
QuickContact::on_btnNext_clicked()
{
    m_nCurr = ( m_nCurr == m_found.count() - 1) ? 0 : m_nCurr + 1 ;
    ui->contacts->setCurrentItem( m_found.at( m_nCurr)) ;
}

//----------------------------------------------------------
void
QuickContact::keyPressEvent( QKeyEvent* pEvent)
{
    if ( pEvent == nullptr) {
        return ;
    }

    if ( pEvent->key() == Qt::Key_Escape  &&
         QMessageBox::question( this, tr( "Exit"), tr( "Do you really want to exit?"),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        accept() ;
    }
}

//----------------------------------------------------------
void
QuickContact::on_QuickContact_accepted()
{
    QListWidgetItem* pItem = ui->contacts->currentItem() ;
    if ( pItem != nullptr) {
        m_set.setValue( m_szLast, pItem->text()) ;
    }
}

//----------------------------------------------------------
void
QuickContact::mouseDoubleClickEvent( QMouseEvent* pEvent)
{
    if ( pEvent == nullptr) {
        return ;
    }

    QRect rect = ui->number->rect() ;
    rect.moveTo( ui->number->pos()) ;

    QListWidgetItem* pItem = ui->contacts->currentItem() ;

    if ( pItem != nullptr  &&  rect.contains( pEvent->pos())) {
        ui->contacts->scrollToItem( pItem) ;
    }
}
