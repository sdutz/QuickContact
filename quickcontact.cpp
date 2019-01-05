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
    m_szWidth = "width" ;
    m_szHeight = "height" ;
    m_szNumSep  = ":" ;
    m_szContSep = "," ;
    m_szTitle   = "Contacts" ;
    setChildrenFont() ;
    loadMap() ;
    showMap( true) ;
    setTitle() ;
    ui->btnSave->setEnabled( false) ;
    ui->btnReset->setEnabled( false) ;
    ui->filter->setFocus() ;
    enablePrevNext() ;
    resize( m_set.value( m_szWidth, minimumWidth()).toInt(),
            m_set.value( m_szHeight, minimumHeight()).toInt()) ;
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

    ui->number->display( m_mapN.value( pItem->text())) ;
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
        on_btnReset_clicked() ;
        return ;
    }
    else {
        ui->btnReset->setEnabled( true) ;
    }
    m_found.clear() ;
    bool bOk ;
    int num = arg1.toInt( &bOk) ;
    if ( bOk) {
        QString res = m_mapN.key( num) ;
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
    }

    enablePrevNext() ;

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

    m_mapN.insert( szContact, nNum) ;

    ui->contacts->addItem( szContact) ;
    ui->contacts->sortItems() ;
    ui->number->display( nNum) ;

    setTitle() ;
    setModified( true) ;
    enablePrevNext() ;
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
    int nOld = m_mapN.value( szOld) ;

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
    m_mapN.remove( szOld) ;
    m_mapN.insert( szNew, nNew) ;

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

    m_mapN.remove( szCurr) ;
    if ( m_mapS.find( szCurr) != m_mapS.end()) {
        m_mapS.remove( szCurr) ;
    }
    ui->contacts->takeItem( nRow) ;

    setTitle() ;
    setModified( true) ;
    enablePrevNext() ;
}

//----------------------------------------------------------
void
QuickContact::on_btnSave_clicked()
{
    QString szVal ;

    for ( m_iter = m_mapN.begin() ;  m_iter != m_mapN.end() ;  ++ m_iter) {
        szVal += m_iter.key() + m_szNumSep + QString::number( m_iter.value()) ;
        if ( m_mapS.find( m_iter.key()) != m_mapS.end()) {
            szVal += m_szNumSep + QString::number( m_mapS.value( m_iter.key())) ;
        }
        szVal += m_szContSep ;
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
    int nOldSize = m_mapN.count() ;

    while( ! stream.atEnd()) {
        slTokens = stream.readLine().split( m_szNumSep) ;
        if ( slTokens.count() < 2) {
            continue ;
        }
        m_mapN.insert( slTokens.at(0), slTokens.at(1).toInt()) ;
        if ( slTokens.count() == 3) {
            m_mapS.insert( slTokens.at(0), slTokens.at(2).toInt()) ;
        }
    }

    file.close() ;

    if ( m_mapN.count() != nOldSize) {
        showMap() ;
        setModified( true) ;
        setTitle() ;
    }
}

//----------------------------------------------------------
void QuickContact::on_btnReset_clicked()
{
    ui->filter->clear() ;
    ui->btnReset->setEnabled( false) ;
    m_found.clear() ;
    showAll() ;
    m_found = ui->contacts->findItems( m_mapN.key( ui->number->intValue()), Qt::MatchFlag::MatchExactly) ;
    if ( m_found.count() == 1) {
        ui->contacts->setFocus() ;
        ui->contacts->setCurrentRow( ui->contacts->row( m_found.first())) ;
        ui->contacts->scrollToItem( m_found.first()) ;
    }
    m_found.clear() ;
    enablePrevNext() ;
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

    lszKeys = m_mapN.keys() ;
    for ( nIdx = 0 ;  nIdx < lszKeys.count() ;  nIdx ++) {
        ui->contacts->addItem( lszKeys.at( nIdx)) ;
    }

    if ( m_mapN.count() > 0) {
        if ( szCurr.isEmpty()) {
            ui->contacts->setCurrentRow( 0) ;
        }
        else {
            m_found = ui->contacts->findItems( szCurr, Qt::MatchExactly) ;
            if ( m_found.count() == 1) {
                ui->contacts->setFocus() ;
                ui->contacts->setCurrentRow( ui->contacts->row( m_found.first())) ;
                m_found.clear() ;
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
            m_mapN.insert( lszContact.at(0), lszContact.at(1).toInt()) ;
            if ( lszContact.count() == 3) {
                m_mapS.insert( lszContact.at(0), lszContact.at(2).toInt()) ;
            }
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
        ui->number->display( m_mapN.value( pItem->text())) ;
        QString szTip ;
        if ( m_mapS.find( pItem->text()) != m_mapS.end()) {
            szTip = QString::number( m_mapS.value( pItem->text())) ;
        }
        ui->number->setToolTip( szTip) ;
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
    setWindowTitle( m_szTitle + " : " + QString::number( m_mapN.count())) ;
}

//----------------------------------------------------------
void
QuickContact::on_btnNext_clicked()
{
    if ( m_found.isEmpty()) {
        int nCurr = ui->contacts->currentRow() ;
        ui->contacts->setCurrentRow( nCurr == ui->contacts->count() - 1 ? 0 : nCurr + 1) ;
    }
    else {
        m_nCurr = ( m_nCurr == m_found.count() - 1) ? 0 : m_nCurr + 1 ;
        ui->contacts->setCurrentItem( m_found.at( m_nCurr)) ;
    }
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

    m_set.setValue( m_szWidth, width()) ;
    m_set.setValue( m_szHeight, height()) ;
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

//----------------------------------------------------------
void
QuickContact::on_btnPrev_clicked()
{
    if ( m_found.isEmpty()) {
        int nCurr = ui->contacts->currentRow() ;
        ui->contacts->setCurrentRow( nCurr == 0 ? ui->contacts->count() - 1 : nCurr - 1) ;
    }
    else {
        m_nCurr = ( m_nCurr == 0) ? m_found.count() - 1 : m_nCurr - 1 ;
        ui->contacts->setCurrentItem( m_found.at( m_nCurr)) ;
    }
}

//----------------------------------------------------------
void
QuickContact::enablePrevNext( void)
{
    bool bDisable = ui->contacts->count() == 0  ||
                   ( ! ui->filter->text().isEmpty()  &&  m_found.isEmpty()) ;

    ui->btnNext->setEnabled( ! bDisable) ;
    ui->btnPrev->setEnabled( ! bDisable) ;
}
