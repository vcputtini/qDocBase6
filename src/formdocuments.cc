/***************************************************************************
 * Copyright (c) 2007-2024                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com
 *                                                                         *
 * Redistribution and use in source and binary forms, with or without      *
 * modification, are permitted provided that the following conditions      *
 * are met:                                                                *
 * 1. Redistributions of source code must retain the above copyright       *
 *    notice, this list of conditions and the following disclaimer.        *
 * 2. Redistributions in binary form must reproduce the above copyright    *
 *    notice, this list of conditions and the following disclaimer in the  *
 *    documentation and/or other materials provided with the distribution. *
 * 4. Neither the name of the Author     nor the names of its contributors *
 *    may be used to endorse or promote products derived from this software*
 *    without specific prior written permission.                           *
 *                                                                         *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS  *
 * BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR   *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS*
 * INTERRUPTION)                                                           *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,     *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING   *
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
 * POSSIBILITY OFSUCH DAMAGE.                                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "formdocuments.h"
#include "progid.h"
#include "qevent.h"
#include "ui_formdocuments.h"

#include "sysactivity.h"
#include "typeimage.h"

#include "tableview_documents_itemdelegate.h"
#include "tableview_documents_p.h"

#include "exportfiles.h"
#include "formimportbatch.h"
#include "importbatch.h"
#include "multifiles.h"
#include "printdocanalitic.h"
#include "printdocgeneral.h"
#include "printimage.h"
#include "printmultiplesimages.h"

/*!
 * \brief FormDocuments::FormDocuments
 * \param parent
 */
FormDocuments::FormDocuments(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormDocuments)
  , modelThumbs_(new QStandardItemModel)
  , scene_(new QGraphicsScene)
  , dateBegin_(QDate(2001, 1, 1))
  , dateEnd_(QDate::currentDate())
  , datePurpose_{ true }
  , Ident_t_{}
  , newFileName_{}
{
  ui->setupUi(this);

  Messages_ = &Messages_->instance();

  progress_.close();

  connect(ui->toolButton_FindFiles,
          SIGNAL(clicked()),
          this,
          SLOT(toolButton_FindFiles_clicked()));

  connect(ui->toolButton_FilesReload,
          SIGNAL(clicked()),
          this,
          SLOT(toolButton_FilesReload_clicked()));

  connect(ui->comboBox_Files,
          SIGNAL(currentTextChanged(QString)),
          this,
          SLOT(comboBox_Files_currentTextChanged(QString)));

  connect(ui->listView_Files,
          SIGNAL(clicked(QModelIndex)),
          this,
          SLOT(listView_Files_clicked(QModelIndex)));

  connect(ui->listView_Files,
          SIGNAL(doubleClicked(QModelIndex)),
          this,
          SLOT(listView_Files_doubleClicked(QModelIndex)));

  connect(ui->listView_Files,
          SIGNAL(entered(QModelIndex)),
          this,
          SLOT(listView_Files_entered(QModelIndex)));

  connect(ui->comboBox_DocType,
          SIGNAL(currentIndexChanged(int)),
          this,
          SLOT(comboBox_DocType_currentIndexChanged(int)));

  connect(ui->toolButton_Departments_Save,
          SIGNAL(clicked()),
          this,
          SLOT(saveDepartments_slot()));

  connect(ui->toolButton_Departments_Reload,
          SIGNAL(clicked(bool)),
          this,
          SLOT(reloadDepartments_slot(bool)));

  connect(ui->toolButton_Departments_Delete,
          SIGNAL(clicked()),
          this,
          SLOT(deleteDepartments_slot()));

  connect(ui->treeView_BaseEntities,
          SIGNAL(clicked(QModelIndex)),
          this,
          SLOT(treeView_BaseEntities_clicked(QModelIndex)));

  connect(ui->treeView_StorageEntities,
          SIGNAL(clicked(QModelIndex)),
          this,
          SLOT(treeView_StorageEntities_clicked(QModelIndex)));

  connect(ui->TableView_Documents,
          SIGNAL(doubleClicked(const QModelIndex&)),
          this,
          SLOT(tableView_Documents_doubleClicked(const QModelIndex&)));

  connect(ui->lineEdit_Operator,
          &QLineEdit::textChanged,
          this,
          [this](const QString& str_) {
            ui->lineEdit_Operator->setText(str_.toUpper());
          });

  connect(ui->checkBox_Batch, &QCheckBox::clicked, this, [this](bool state_) {
    (state_ ? ui->listView_Files->setSelectionMode(
                QAbstractItemView::ExtendedSelection)
            : ui->listView_Files->setSelectionMode(
                QAbstractItemView::SingleSelection));
  });

  clearRecord(true);

  ui->toolBox_Docs->setItemText(0, tr("Pesquisa"));
  ui->toolBox_Docs->setItemText(1, tr("Registro"));
  ui->toolBox_Docs->setCurrentIndex(1);
  ui->toolBox_Docs->setItemEnabled(1, false);

  ui->spinBox_BeginYear->setValue(QDate::currentDate().year());
  ui->spinBox_EndYear->setValue(QDate::currentDate().year());

  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
  if (settings.value("defaultdocid").toBool()) {
    ui->groupBox_DocId->setChecked(true);
  } else {
    ui->groupBox_DocId->setChecked(false);
  }

  ui->comboBox_DocType->setCurrentIndex(0);

  TableView_Documents_ItemDelegate* delegateResults =
    new TableView_Documents_ItemDelegate(ui->TableView_Documents);
  ui->TableView_Documents->setItemDelegate(delegateResults);

  ui->label_Ident->setText(QString("%0").arg(Globals::lastDocumentId()));

  /*!
   * \note Fields disabled by default in UI definition
   */
  if (settings.value("field01Act").toBool()) {
    ui->lineEdit_CustomFiled_1->setEnabled(true);
    ui->lineEdit_CustomFiled_1->setPlaceholderText(
      settings.value("field01Name").toString());
    ui->lineEdit_CustomFiled_1->setMaxLength(
      settings.value("field01Len").toInt());
  }

  if (settings.value("field02Act").toBool()) {
    ui->lineEdit_CustomFiled_2->setEnabled(true);
    ui->lineEdit_CustomFiled_2->setPlaceholderText(
      settings.value("field02Name").toString());
    ui->lineEdit_CustomFiled_2->setMaxLength(
      settings.value("field02Len").toInt());
  }

  if (settings.value("field03Act").toBool()) {
    ui->lineEdit_CustomFiled_3->setEnabled(true);
    ui->lineEdit_CustomFiled_3->setPlaceholderText(
      settings.value("field03Name").toString());
    ui->lineEdit_CustomFiled_3->setMaxLength(
      settings.value("field03Len").toInt());
  }

  if (settings.value("field04Act").toBool()) {
    ui->lineEdit_CustomFiled_4->setEnabled(true);
    ui->lineEdit_CustomFiled_4->setPlaceholderText(
      settings.value("field04Name").toString());
    ui->lineEdit_CustomFiled_4->setMaxLength(
      settings.value("field04Len").toInt());
  }

  if (settings.value("field05Act").toBool()) {
    ui->lineEdit_CustomFiled_5->setEnabled(true);
    ui->lineEdit_CustomFiled_5->setPlaceholderText(
      settings.value("field05Name").toString());
    ui->lineEdit_CustomFiled_5->setMaxLength(
      settings.value("field05Len").toInt());
  }

  loadListThumbs(ui->comboBox_Files->currentText());
}

/*!
 * \brief FormDocuments::~FormDocuments
 */
FormDocuments::~FormDocuments()
{
  delete ui;
}

/*!
 * \protected
 * \brief FormDocuments::closeEvent
 * \param event_
 */
void
FormDocuments::closeEvent(QCloseEvent* event_)
{
  /*!
   * \note Informs that the window has been closed. Emits the closed(false)
   * signal so that the toolbar is placed in invisible mode
   */
  emit closed(false);
  emit deleteMenu(Globals::ModulesOfApp::ModDocuments);
  event_->accept();
}

/*!
 * \public SLOT
 * \brief FormDocuments::comboBox_DocType_currentTextChanged
 * \param text_
 */
void
FormDocuments::comboBox_DocType_currentIndexChanged(int index_)
{
  ui->lineEdit_DocIdent->clear();
  ui->lineEdit_DocIdent->setInputMask("");
  switch (index_) {
    case 0:
    case 1: {
      ui->lineEdit_DocIdent->setInputMask("99.999.999/9999-99");
      break;
    }
    case 2: {
      ui->lineEdit_DocIdent->setInputMask("999.999.999.999");
      break;
    }
    case 3: {
      ui->lineEdit_DocIdent->setInputMask(">xxxxxxxxxxxxxxxxxxxx");
      break;
    }
    case 4: {
      ui->lineEdit_DocIdent->setInputMask("999.999.999-99");
      break;
    }
    case 5: {
      ui->lineEdit_DocIdent->setInputMask("99.999.999-X");
      break;
    }
    case 6: {
      ui->lineEdit_DocIdent->setInputMask(">xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
    }
  }
}

/*!
 * \public SLOT
 * \brief FormDocuments::comboBox_Files_currentTextChanged
 * \param text_
 */
void
FormDocuments::comboBox_Files_currentTextChanged(QString text_)
{
  if (text_.length() < 3) {
    return;
  } else if (text_.contains("\\")) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00011).text(),
      QMessageBox::Close);
    return;
  }
  loadListThumbs(text_);
}

/*!
 * \public SLOT
 * \brief FormDocuments::toolButton_FindFiles_clicked
 */
void
FormDocuments::toolButton_FindFiles_clicked()
{
  QString pathImages_ = QFileDialog::getExistingDirectory(
    this, tr("Imagens"), settings_.getRootDir());
  QString aux_ = pathImages_;
  ui->comboBox_Files->setCurrentText(aux_ + "/*");
  loadListThumbs(ui->comboBox_Files->currentText());
}

/*!
 * \public SLOT
 * \brief FormDocuments::toolButton_FilesReload_clicked
 */
void
FormDocuments::toolButton_FilesReload_clicked()
{
  loadListThumbs(ui->comboBox_Files->currentText());
}

/*!
 * \public SLOT
 * \brief listView_Files_clicked
 * \param index_
 */
void
FormDocuments::listView_Files_clicked(QModelIndex index_)
{
  QString textIEC_{};
  QString sizeFormatted_{};

  Globals::toIEC(
    index_.data(Qt::UserRole + 1).toReal(), textIEC_, sizeFormatted_);
  if (index_.data().toString().size() > 100) {
    ui->label_FilesInfo->setText(index_.data().toString());
  } else {
    ui->label_FilesInfo->setText(QString(tr("%0\t [%1 %2]"))
                                   .arg(index_.data().toString())
                                   .arg(textIEC_)
                                   .arg(sizeFormatted_.simplified()));
  }

  QDir dir_;
  fullFileName_ = dir_.filePath(index_.data(Qt::UserRole).toString());

  QImage img_;

  scene_ = new QGraphicsScene(ui->graphicsView_Images);
  ui->graphicsView_Images->resetTransform();

  TypeImage ti_;
  if (ti_.isImageViewable(fullFileName_)) {
    QImageReader reader_(fullFileName_);
    reader_.read(&img_);
    QPixmap pixTmp_(QPixmap::fromImage(img_));
    scene_->addPixmap(pixTmp_);
    ui->graphicsView_Images->scale(0.2, 0.2);
    flagControls.IsViewable = true;
  } else {
    QPixmap pixTmp_(ti_.type(fullFileName_));
    pixTmp_.scaled(QSize(64, 64));
    scene_->addPixmap(pixTmp_);
    flagControls.IsViewable = false;
  }

  ui->graphicsView_Images->setScene(scene_);
  flagControls.ImageSelected = true;
}

/*!
 * \public SLOT
 * \brief FormDocuments::listView_Files_doubleClicked
 * \param index_
 */
void
FormDocuments::listView_Files_doubleClicked(QModelIndex index_)
{
  QDir dir_;

  if (Globals::isExecutable(index_.data(Qt::UserRole).toString())) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00012).text(),
      QMessageBox::Ok);
    return;
  }

  QString outFile_ = dir_.filePath(index_.data(Qt::UserRole).toString());
  if (outFile_.isNull() || outFile_.isEmpty()) {
    return;
  }

#ifdef Q_OS_LINUX
  if (!QDesktopServices::openUrl(
        QUrl("file://" + index_.data(Qt::UserRole).toString(),
             QUrl::TolerantMode))) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00013).text(),
      QMessageBox::Ok);
  }
#else
  if (!QDesktopServices::openUrl(
        QUrl("file:///" + index_.data(Qt::UserRole).toString(),
             QUrl::TolerantMode))) {
    QMessageBox::warning(
      this,
      ProId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00013).text(),
      QMessageBox::Ok);
  }
#endif
}

/*!
 * \public SLOT
 * \brief FormDocuments::listView_Files_entered
 * \param index_
 */
void
FormDocuments::listView_Files_entered(QModelIndex index_)
{
  ui->listView_Files->setCurrentIndex(index_);
  ui->label_FilesInfo->setText(index_.data().toString());
}

/*!
 * \public SLOT
 * \brief treeView_BaseEntities_clicked
 * \param index_
 */
void
FormDocuments::treeView_BaseEntities_clicked(QModelIndex index_)
{
  if (!index_.isValid()) {
    return;
  }

  /*!
   * \note Returns only the value of the column with the customer identification
   * (customer code), no matter which column the user clicks on.
   */
  auto column_id_ = index_.model()->sibling(index_.row(), 1, index_);

  // Determines which category is in use and returns the corresponding index.
  auto categories_id_ = static_cast<Globals::Categories>(
    index_.model()->data(index_.parent(), Qt::UserRole).toInt());
  Ident_t_.Categorie =
    std::move(index_.model()->data(index_.parent(), Qt::UserRole).toInt());

  Ident_t_.BaseId = column_id_.data().toULongLong();
  ui->treeView_StorageEntities->setEntityId(Ident_t_.BaseId);
  ui->treeView_StorageEntities->statusRecord(Qt::Checked);
  column_id_ = index_.model()->sibling(index_.row(), 2, index_);
  Ident_t_.mainEntity_ = column_id_.data().toString();

  ui->treeView_StorageEntities->setCategorie(categories_id_);
  ui->treeView_StorageEntities->fillTree();
  ui->treeView_StorageEntities->expand(index_);

  // -----------------------------------------------------------------------
  ui->TableView_Documents->setCategorie(
    static_cast<Globals::Categories>(Ident_t_.Categorie));
  ui->TableView_Documents->setEntityId(Ident_t_.BaseId);
  ui->TableView_Documents->setEntityIdStorage(Ident_t_.StorageId);
  ui->TableView_Documents->setDateRange(ui->spinBox_BeginYear->value(),
                                        ui->spinBox_EndYear->value());
  ui->TableView_Documents->fillView();
}

/*!
 * \public SLOT
 * \brief FormDocuments::treeView_StorageEntities_clicked
 * \param index_
 */
void
FormDocuments::treeView_StorageEntities_clicked(QModelIndex index_)
{
  if (!index_.isValid()) {
    return;
  }

  if (ui->spinBox_BeginYear->value() > ui->spinBox_EndYear->value()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00014).text(),
      QMessageBox::Close);
    return;
  }

  auto isChecked_ = index_.model()->sibling(index_.row(), 0, index_);

  if (isChecked_.data(Qt::CheckStateRole).toBool()) {
    auto column_id_ = index_.model()->sibling(index_.row(), 1, index_);
    Ident_t_.StorageId = column_id_.data().toULongLong();

    column_id_ = index_.model()->sibling(index_.row(), 2, index_);
    Ident_t_.linkedEntity_ = column_id_.data().toString();

    showTitle(Ident_t_.mainEntity_, Ident_t_.linkedEntity_);

    ui->toolBox_Docs->setCurrentIndex(1);
    ui->toolBox_Docs->setItemEnabled(1, true);
    clearRecord();
  } else {
    auto column_id_ = index_.model()->sibling(index_.row(), 1, index_);
    Ident_t_.StorageId = column_id_.data().toULongLong();
  }
}

/*!
 * \public SLOT
 * \brief FormDocuments::toolBarDateEditBegin_dateChanged
 * \param date_
 */
void
FormDocuments::toolBarDateEditBegin_dateChanged(QDate date_)
{
  (date_.isValid() ? dateBegin_ = std::move(date_)
                   : dateBegin_ = std::move(QDate::currentDate()));
}

/*!
 * \public SLOT
 * \brief FormDocuments::toolBarDateEditEnd_dateChanged
 * \param date_
 */
void
FormDocuments::toolBarDateEditEnd_dateChanged(QDate date_)
{
  (date_.isValid() ? dateEnd_ = std::move(date_)
                   : dateEnd_ = std::move(QDate::currentDate()));
}

/*!
 * \public SLOT
 * \brief checkBoxDatePorpuse_toggled
 * \param b_
 */
void
FormDocuments::checkBoxDatePurpose_triggered(bool state_)
{
  datePurpose_ = std::move(state_);
}

/*!
 * \public SLOT
 * \brief FormDocuments::iDialog_finished
 * \param result_
 */
void
FormDocuments::iDialog_finished(int result_)
{
}

/*!
 * \public SLOT
 * \brief FormDocuments::iDialog_textValueSelected
 * \param passwd_
 */
void
FormDocuments::iDialog_textValueSelected(const QString& passwd_)
{
  if (!passwd_.isEmpty()) {
    if (Globals::isOperatorValid(recDoc.st_operator.simplified(),
                                 passwd_.simplified())) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        Messages_->set(MessagesNS::Tokens::GEN_WARN_00015).text(),
        QMessageBox::Close);
      return;
    }

    showDataFieldsOnForm();
  }
}

/*!
 * \public SLOT
 * \brief FormDocuments::tableView_Documents_doubleClicked
 * \param index_
 */
void
FormDocuments::tableView_Documents_doubleClicked(const QModelIndex& index_)
{
  if (!index_.isValid()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::INTERNAL_ERROR)
                .text()
                .arg(__FUNCTION__)
                .arg(__LINE__)
                .arg(Messages_->set(MessagesNS::Tokens::INTERR_0001).text())),
      QMessageBox::Close);
    return;
  }

  auto column_id_ = index_.model()->sibling(index_.row(), 0, index_);

  const QString stmnt_{
    QString("SELECT ST_IDENT, DATE_FORMAT(ST_RECDATE,'%d/%m/%Y','%0') AS "
            "ST_RECDATE, ")
      .arg(Globals::whatLocale().first) +
    QString("ST_OPERATOR, ET_IDENT, ST_ETRELA,") +
    QString("ST_DOCDATE, ST_NUMBER, ST_VALUE, ST_DOCTYPE, ST_DOCIDENT,"
            "ST_DEPART, ST_FILE, ST_FILETYPE,"
            "ST_REMARK, ST_TOPSEC,"
            "ST_FIELD01DATA, ST_FIELD02DATA,"
            "ST_FIELD03DATA, ST_FIELD04DATA, ST_FIELD04DATA, "
            "(SELECT C.ET_NOME FROM DOCENTITY C WHERE "
            "C.ET_IDENT=A.ST_ETRELA) AS LINK_NAME,"
            "(SELECT B.IM_IMAGE FROM DOCIMAGES B WHERE B.ST_IDENT=A.ST_IDENT) "
            "AS IM_IMAGE "
            "FROM DOCSTORAGE A WHERE ST_IDENT = %0")
      .arg(std::move(column_id_.data().toInt()))
  };

  QSqlQueryModel qmodel_;
  qmodel_.setQuery(std::move(stmnt_));
  if (qmodel_.lastError().isValid()) {
    QMessageBox::critical(
      this, ProgId::Name, qmodel_.lastError().text(), QMessageBox::Close);
    return;
  }

  Ident_t_.linkedEntity_ =
    std::move(qmodel_.record(0).value("LINK_NAME").toString());

  recDoc.st_ident =
    std::move(qmodel_.record(0).value("ST_IDENT").toULongLong());
  recDoc.st_regdate =
    std::move(qmodel_.record(0).value("ST_RECDATE").toString());
  recDoc.st_operator =
    std::move(qmodel_.record(0).value("ST_OPERATOR").toString());

  recDoc.et_ident =
    std::move(qmodel_.record(0).value("ET_IDENT").toULongLong());
  Ident_t_.StorageId = recDoc.et_ident;

  recDoc.st_etrela =
    std::move(qmodel_.record(0).value("ST_ETRELA").toULongLong());

  recDoc.st_docdate =
    std::move(qmodel_.record(0).value("ST_DOCDATE").toString());
  recDoc.st_number = std::move(qmodel_.record(0).value("ST_NUMBER").toInt());
  recDoc.st_value = std::move(qmodel_.record(0).value("ST_VALUE").toDouble());
  recDoc.st_doctype = std::move(qmodel_.record(0).value("ST_DOCTYPE").toInt());
  recDoc.st_docident =
    std::move(qmodel_.record(0).value("ST_DOCIDENT").toString());

  recDoc.st_depart = std::move(qmodel_.record(0).value("ST_DEPART").toString());
  recDoc.st_file = std::move(qmodel_.record(0).value("ST_FILE").toString());
  recDoc.st_filetype =
    std::move(qmodel_.record(0).value("ST_FILETYPE").toBool());
  recDoc.st_remark = std::move(qmodel_.record(0).value("ST_REMARK").toString());
  recDoc.st_topsec = std::move(qmodel_.record(0).value("ST_TOPSEC").toBool());

  recDoc.st_field01data =
    std::move(qmodel_.record(0).value("ST_FIELD01DATA").toString());
  recDoc.st_field02data =
    std::move(qmodel_.record(0).value("ST_FIELD02DATA").toString());
  recDoc.st_field03data =
    std::move(qmodel_.record(0).value("ST_FIELD03DATA").toString());
  recDoc.st_field04data =
    std::move(qmodel_.record(0).value("ST_FIELD04DATA").toString());
  recDoc.st_field05data =
    std::move(qmodel_.record(0).value("ST_FIELD05DATA").toString());

  recDoc.im_image =
    std::move(qUncompress(qmodel_.record(0).value("IM_IMAGE").toByteArray()));

  if (recDoc.st_topsec) {
    QInputDialog* iDialog = new QInputDialog(this, Qt::Dialog);
    iDialog->setWindowTitle(tr("Desbloqueio de Registro"));
    iDialog->setCancelButtonText(tr("Cancelar"));
    iDialog->setOkButtonText(tr("Confirmar"));
    iDialog->setInputMode(QInputDialog::TextInput);
    iDialog->setLabelText(tr("Senha:"));
    iDialog->setTextEchoMode(QLineEdit::Password);
    iDialog->open(this, SLOT(iDialog_textValueSelected(QString)));
    return;
  }

  showDataFieldsOnForm();
}

/*!
 * \public SLOT
 * \brief FormDocuments::saveRecord
 */
void
FormDocuments::saveRecord()
{
  auto whatExtension_ = [](const QString& fullFileName_) {
    if (fullFileName_.count(".") > 1) {
      return QString();
    }
    QFileInfo fInfo_(fullFileName_);
    return fInfo_.suffix();
  };

  QFileInfo fInfo_(fullFileName_);
  QString internalFileNamePattern_ = fInfo_.fileName().replace(" ", "_");

  QSqlDatabase::database().transaction();
  QSqlQuery qry_doc_;
  qry_doc_.prepare(insert_doc_stmnt_);

  switch (settings_.getDbNativeDateFormat()) {
    case Globals::SupportedDateFormats::ISO: {
      if (Globals::is_ptBR()) {
        qry_doc_.bindValue(":ST_RECDATE",
                           Globals::toISODate(ui->label_CurrDate->text()));
      } else {
        qry_doc_.bindValue(":ST_RECDATE", ui->label_CurrDate->text());
      }
      break;
    }
    case Globals::SupportedDateFormats::EUA: {
      if (Globals::is_ptBR()) {
        qry_doc_.bindValue(":ST_RECDATE",
                           Globals::toEUADate(ui->label_CurrDate->text()));
      } else {
        qry_doc_.bindValue(":ST_RECDATE", ui->label_CurrDate->text());
      }
      break;
    }
    case Globals::SupportedDateFormats::UK: {
      qry_doc_.bindValue(":ST_RECDATE", ui->label_CurrDate->text());
    }
  }

  qry_doc_.bindValue(":ST_OPERATOR",
                     ui->lineEdit_Operator->text().simplified());
  qry_doc_.bindValue(":ET_IDENT", Ident_t_.BaseId);
  qry_doc_.bindValue(":ST_ETRELA", Ident_t_.StorageId);
  qry_doc_.bindValue(":ST_DOCDATE", ui->dateEdit_DocId->date());
  qry_doc_.bindValue(":ST_NUMBER", ui->lineEdit_DocNumber->text().toInt());
  qry_doc_.bindValue(":ST_VALUE",
                     ui->lineEdit_DocValue->text().simplified().toFloat());
  qry_doc_.bindValue(":ST_DOCTYPE", ui->comboBox_DocType->currentIndex());
  qry_doc_.bindValue(":ST_DOCIDENT",
                     ui->lineEdit_DocIdent->text().simplified());
  qry_doc_.bindValue(":ST_DEPART",
                     ui->comboBox_Departments->currentText().simplified());

  qry_doc_.bindValue(":ST_FILE",
                     Globals::normalizeFileName(internalFileNamePattern_));
  qry_doc_.bindValue(":ST_FILETYPE", ui->groupBox_DocId->isChecked());

  qry_doc_.bindValue(":ST_REMARK", ui->textEdit_Comments->toPlainText());
  qry_doc_.bindValue(
    ":ST_TOPSEC",
    (ui->lineEdit_OperPass->text().simplified().isEmpty() ? false : true));
  qry_doc_.bindValue(":ST_FIELD01DATA",
                     ui->lineEdit_CustomFiled_1->text().simplified());
  qry_doc_.bindValue(":ST_FIELD02DATA",
                     ui->lineEdit_CustomFiled_2->text().simplified());
  qry_doc_.bindValue(":ST_FIELD03DATA",
                     ui->lineEdit_CustomFiled_3->text().simplified());
  qry_doc_.bindValue(":ST_FIELD04DATA",
                     ui->lineEdit_CustomFiled_4->text().simplified());
  qry_doc_.bindValue(":ST_FIELD05DATA",
                     ui->lineEdit_CustomFiled_5->text().simplified());
  qry_doc_.exec();
  if (qry_doc_.lastError().isValid()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      qry_doc_.lastError().text() +
        "  ErrCode: " + qry_doc_.lastError().nativeErrorCode() + "\n\n" +
        Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text(),
      QMessageBox::Close);
    return;
  }
  quint64 last_insert_id_ = qry_doc_.lastInsertId().toULongLong();
  QSqlDatabase::database().commit();

  if (last_insert_id_ == 0) {
    QMessageBox::critical(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text() +
        "lastInsert_ID",
      QMessageBox::Close);
    return;
  }

  /* ---------------------------------------------------------------------- */
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  MultiFiles* multiFilesInput_ = new MultiFiles;
  connect(multiFilesInput_,
          SIGNAL(errorText(const QString&)),
          this,
          SLOT(multiFilesErrorText(const QString&)));
  multiFilesInput_->setFileName(fullFileName_);
  multiFilesInput_->execute();
  QByteArray baInputFile_(multiFilesInput_->data());

  const QString insert_img_stmnt_{ "INSERT INTO DOCIMAGES (ST_IDENT, IM_IMAGE) "
                                   "VALUES (:ST_IDENT, :IM_IMAGE)" };

  QSqlDatabase::database().transaction();
  QSqlQuery qry_img_;
  qry_img_.prepare(insert_img_stmnt_);
  qry_img_.bindValue(":ST_IDENT", last_insert_id_);
  qry_img_.bindValue(
    ":IM_IMAGE", qCompress(baInputFile_), QSql::In | QSql::Binary);
  qry_img_.exec();
  if (qry_img_.lastError().isValid()) {
    QMessageBox::critical(
      this,
      ProgId::Name,
      qry_img_.lastError().text() + "\n\n" +
        Messages_->set(MessagesNS::Tokens::DB_WARN_IMAGENOTSAVED).text(),
      QMessageBox::Close);
    QSqlDatabase::database().transaction();
    QSqlQuery delInvalidReg_;
    delInvalidReg_.prepare(
      QString("DELETE FROM DOCSTORAGE WHERE ST_IDENT=%0").arg(last_insert_id_));
    delInvalidReg_.exec();
    QSqlDatabase::database().commit();
    QApplication::restoreOverrideCursor();
    return;
  }
  QSqlDatabase::database().commit();

  // Deletes the selected image from the list
  QModelIndex cur = ui->listView_Files->currentIndex();
  if (cur.isValid() && !modelThumbs_->removeRows(cur.row(), 1, cur.parent())) {
    QMessageBox::critical(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00016).text(),
      QMessageBox::Close);
  }

  // Erases the file from disk, if configured to do so
  if (settings_.removeImageFromDir()) {
    if (QDir d; !d.remove(fullFileName_)) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        QString(Messages_->set(MessagesNS::Tokens::FILE_NOTDELETED).text())
          .arg(fullFileName_),
        QMessageBox::Close);
    }
  }

  fullFileName_.clear();
  filePicName_.clear();

  clearRecord(true);
  QApplication::restoreOverrideCursor();
}

/*!
 * \public SLOT
 * \brief FormDocuments::updateRecord
 */
void
FormDocuments::updateRecord()
{
  SysActivity* sysAct_ = new SysActivity(this);
  if (!sysAct_->logger(Globals::ModulesOfApp::ModDocuments,
                       SysActivity::Operations::Update,
                       ui->label_Ident->text().toLongLong())) {
    delete sysAct_;
    return;
  }

  //-------------------------------------------------------------------------
  QSqlDatabase::database().transaction();
  QSqlQuery qry_doc_;
  qry_doc_.prepare(std::move(stmnt_update_));

  qry_doc_.bindValue(":ST_RECDATE",
                     std::move(Globals::toISODate(ui->label_CurrDate->text())));
  qry_doc_.bindValue(":ST_OPERATOR",
                     std::move(ui->lineEdit_Operator->text().simplified()));
  qry_doc_.bindValue(":ET_IDENT", std::move(Ident_t_.BaseId));
  qry_doc_.bindValue(":ST_ETRELA", std::move(Ident_t_.StorageId));
  qry_doc_.bindValue(":ST_DOCDATE", std::move(ui->dateEdit_DocId->date()));
  qry_doc_.bindValue(":ST_NUMBER",
                     std::move(ui->lineEdit_DocNumber->text().toInt()));
  qry_doc_.bindValue(
    ":ST_VALUE",
    std::move(ui->lineEdit_DocValue->text().simplified().toFloat()));
  qry_doc_.bindValue(":ST_DOCTYPE",
                     std::move(ui->comboBox_DocType->currentIndex()));
  qry_doc_.bindValue(":ST_DOCIDENT",
                     std::move(ui->lineEdit_DocIdent->text().simplified()));
  qry_doc_.bindValue(
    ":ST_DEPART",
    std::move(ui->comboBox_Departments->currentText().simplified()));

  if (fullFileName_.isEmpty()) {
    qry_doc_.bindValue(":ST_FILE", std::move(recDoc.st_file));
  } else {
    QFileInfo fInfo_(std::move(fullFileName_));
    qry_doc_.bindValue(
      ":ST_FILE", std::move(Globals::normalizeFileName(fInfo_.fileName())));
  }

  qry_doc_.bindValue(":ST_FILETYPE",
                     std::move(ui->groupBox_DocId->isChecked()));

  qry_doc_.bindValue(":ST_REMARK",
                     std::move(ui->textEdit_Comments->toPlainText()));
  qry_doc_.bindValue(
    ":ST_TOPSEC",
    (ui->lineEdit_OperPass->text().simplified().isEmpty() ? false : true));
  qry_doc_.bindValue(
    ":ST_FIELD01DATA",
    std::move(ui->lineEdit_CustomFiled_1->text().simplified()));
  qry_doc_.bindValue(
    ":ST_FIELD02DATA",
    std::move(ui->lineEdit_CustomFiled_2->text().simplified()));
  qry_doc_.bindValue(
    ":ST_FIELD03DATA",
    std::move(ui->lineEdit_CustomFiled_3->text().simplified()));
  qry_doc_.bindValue(
    ":ST_FIELD04DATA",
    std::move(ui->lineEdit_CustomFiled_4->text().simplified()));
  qry_doc_.bindValue(
    ":ST_FIELD05DATA",
    std::move(ui->lineEdit_CustomFiled_5->text().simplified()));

  qry_doc_.bindValue(":ST_IDENT",
                     std::move(ui->label_Ident->text().toULongLong()));

  qry_doc_.exec();
  if (qry_doc_.lastError().isValid()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      qry_doc_.lastError().text() + "\n\n" +
        Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text(),
      QMessageBox::Close);
    return;
  }

  QSqlDatabase::database().commit();

  QList<QGraphicsItem*> item = ui->graphicsView_Images->items();
  if (item.count() > 0 && flagControls.ImageSelected) {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    MultiFiles* multiFilesInput_ = new MultiFiles;
    connect(multiFilesInput_,
            SIGNAL(errorText(const QString&)),
            this,
            SLOT(multiFilesErrorText(const QString&)));
    multiFilesInput_->setFileName(fullFileName_);
    multiFilesInput_->execute();
    QByteArray baInputFile_(multiFilesInput_->data());

    QSqlDatabase::database().transaction();
    QSqlQuery qry_img_;
    qry_img_.prepare(std::move(insert_img_stmnt_));
    qry_img_.bindValue(":ST_IDENT",
                       std::move(ui->label_Ident->text().toULongLong()));
    qry_img_.bindValue(
      ":IM_IMAGE", qCompress(baInputFile_), QSql::In | QSql::Binary);
    qry_img_.exec();
    if (qry_img_.lastError().isValid()) {
      QMessageBox::critical(
        this,
        ProgId::Name,
        qry_img_.lastError().text() +
          Messages_->set(MessagesNS::Tokens::DB_WARN_IMAGENOTSAVED).text(),
        QMessageBox::Close);
      QSqlDatabase::database().transaction();
      QSqlQuery delInvalidReg_;
      delInvalidReg_.prepare(
        std::move(QString("DELETE FROM DOCSTORAGE WHERE ST_IDENT=%0")
                    .arg(ui->label_Ident->text().toULongLong())));
      delInvalidReg_.exec();
      QSqlDatabase::database().commit();
      QApplication::restoreOverrideCursor();
      return;
    }
    QSqlDatabase::database().commit();

    // Deletes the selected image from the list
    QModelIndex cur = ui->listView_Files->currentIndex();
    if (cur.isValid() &&
        !modelThumbs_->removeRows(cur.row(), 1, cur.parent())) {
      QMessageBox::critical(
        this,
        ProgId::Name,
        Messages_->set(MessagesNS::Tokens::GEN_WARN_00016).text(),
        QMessageBox::Close);
    }

    // Erases the file from disk, if configured to do so
    if (settings_.removeImageFromDir()) {
      if (QDir d; !d.remove(fullFileName_)) {
        QMessageBox::warning(
          this,
          ProgId::Name,
          QString(Messages_->set(MessagesNS::Tokens::FILE_NOTDELETED).text())
            .arg(fullFileName_),
          QMessageBox::Close);
      }
    }

    flagControls.ImageSelected = false;
  }
  fullFileName_.clear();
  filePicName_.clear();
  QApplication::restoreOverrideCursor();
}

/*!
 * \public SLOT
 * \brief FormDocuments::deleteRecord
 */
void
FormDocuments::deleteRecord()
{
  if (!scene_) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(
        Messages_->set(MessagesNS::Tokens::INTERNAL_ERROR)
          .text()
          .arg(__FUNCTION__)
          .arg(__LINE__)
          .arg(QString(Messages_->set(MessagesNS::Tokens::INTERR_0002).text())
                 .arg("QGraphicsScene"))),
      QMessageBox::Close);
    return;
  }

  QList<QGraphicsItem*> item = ui->graphicsView_Images->items();
  if (item.count() == 0) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Nenhuma imagem foi selecionada."),
                         QMessageBox::Close);
    return;
  }

  QMessageBox messageBox(this);
  messageBox.setWindowTitle(ProgId::Name);
  messageBox.setText(
    tr("Este procedimento é irreversível!\nVocê confirma a operação?"));
  QAbstractButton* cancelButton =
    messageBox.addButton(tr("Cancelar"), QMessageBox::ActionRole);
  QAbstractButton* continueButton =
    messageBox.addButton(tr("Prossegir"), QMessageBox::ActionRole);
  messageBox.setEscapeButton(cancelButton);

  messageBox.exec();
  if (messageBox.clickedButton() == continueButton) {
    SysActivity* sact = new SysActivity(this);
    if (!sact->logger(Globals::ModulesOfApp::ModDocuments,
                      SysActivity::Operations::Delete,
                      ui->label_Ident->text().toULongLong())) {
      delete sact;
      return;
    }

    const QString stmnt_ = QString("DELETE FROM DOCSTORAGE WHERE ST_IDENT=%0")
                             .arg(ui->label_Ident->text().toULongLong());

    QSqlQuery qry_;
    qry_.prepare(stmnt_);
    qry_.exec();
    if (qry_.numRowsAffected() == 0) {
      QMessageBox::critical(
        this,
        ProgId::Name,
        Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text(),
        QMessageBox::Close);
      return;
    }
    if (qry_.lastError().isValid()) {
      QMessageBox::critical(
        this,
        ProgId::Name,
        qry_.lastError().text() + "\n\n" +
          Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text(),
        QMessageBox::Close);
      return;
    }
    QMessageBox::information(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::DB_WARN_RECSAVED).text(),
      QMessageBox::Close);
  }
}

/*!
 * \private
 * \brief FormDocuments::clearRecord
 */
void
FormDocuments::clearRecord(bool clearTitle_)
{
  ui->dateEdit_DocId->setDate(QDate::currentDate());
  ui->label_CurrDate->setText(QDate::currentDate().toString(
    (Globals::is_ptBR()) ? "dd/MM/yyyy" : "MM-dd-yyyy"));

  QList<QLineEdit*> allLineEdits = this->findChildren<QLineEdit*>();
  for (QLineEdit* le : allLineEdits) {
    le->clear();
  }
  ui->label_FilesInfo->clear();
  ui->textEdit_Comments->clear();

  QList<QGraphicsItem*> items_ = ui->graphicsView_Images->items();
  qDeleteAll(items_);

  if (clearTitle_) {
    showTitle();
  }
}

/*!
 * \public SLOT
 * \brief FormDocuments::saveDepartments_slot
 */
void
FormDocuments::saveDepartments_slot()
{
  ui->comboBox_Departments->save();
}

/*!
 * \public SLOT
 * \brief FormDocuments::reloadDepartments_slot
 * \param b_
 */
void
FormDocuments::reloadDepartments_slot(bool b_)
{
  ui->comboBox_Departments->update();
}

/*!
 * \public SLOT
 * \brief FormDocuments::deleteDepartments_slot
 */
void
FormDocuments::deleteDepartments_slot()
{
  ui->comboBox_Departments->erase();
}

/*!
 * \brief FormDocuments::errorExportFiles
 * \param errText_
 */
void
FormDocuments::errorExportFiles(const QString& errText_)
{
  QMessageBox::warning(this, ProgId::Name, errText_, QMessageBox::Ok);
}

/*!
 * \brief FormDocuments::sqlErrorImportBatch
 * \param errText_
 */
void
FormDocuments::sqlErrorImportBatch(const QString& errText_)
{
  QMessageBox::warning(this, ProgId::Name, errText_, QMessageBox::Ok);
}

/*!
 * \public SLOT
 * \brief FormDocuments::toolButtonExport_clicked
 */
void
FormDocuments::toolButtonExport_clicked()
{
  ExportFiles* expFiles_ = new ExportFiles(this, ui->TableView_Documents);
  connect(
    expFiles_, SIGNAL(error(QString)), this, SLOT(errorExportFiles(QString)));
  expFiles_->exec();
}

/*!
 * \brief toolButtonImportBatch_clicked
 */
void
FormDocuments::toolButtonImportBatch_clicked()
{
  if (ui->lineEdit_Operator->text().isEmpty()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00006).text())
        .arg(tr("Operador")),
      QMessageBox::Close);
    ui->listView_Files->setSelectionMode(QAbstractItemView::SingleSelection);
    return;
  }

  if (ui->comboBox_Departments->currentText().isEmpty()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00006).text())
        .arg(tr("Departamentos")),
      QMessageBox::Close);
    ui->listView_Files->setSelectionMode(QAbstractItemView::SingleSelection);
    return;
  }

  QItemSelectionModel* selectionModel_ = ui->listView_Files->selectionModel();
  if (!selectionModel_) {
    ui->listView_Files->setSelectionMode(QAbstractItemView::SingleSelection);
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::INTERNAL_ERROR)
                .text()
                .arg(__FUNCTION__)
                .arg(__LINE__)
                .arg(Messages_->set(MessagesNS::Tokens::INTERR_0002).text())),
      QMessageBox::Close);
    delete selectionModel_;
    return;
  }

  QModelIndexList indexList_ = selectionModel_->selectedRows();
  if (indexList_.count() == 0) {
    ui->listView_Files->setSelectionMode(QAbstractItemView::SingleSelection);
    QMessageBox::information(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00018).text(),
      QMessageBox::Close);
    return;
  }

  if (ui->groupBox_DocId->isChecked()) {
    FormImportBatch* form_ =
      new FormImportBatch(Ident_t_.BaseId,
                          Ident_t_.StorageId,
                          ui->lineEdit_Operator->text().simplified(),
                          ui->comboBox_Departments->currentText());
    form_->addItems(indexList_);
    form_->show();
    return;
  }

  progress_.setLabelText(tr("Importando ..."));
  progress_.setCancelButtonText(tr("Cancelar"));
  progress_.setMinimum(0);
  progress_.setMaximum(indexList_.count() - 1);
  progress_.setWindowModality(Qt::ApplicationModal);
  progress_.open();

  ImportBatch importBatch_(indexList_);
  importBatch_.setID(Ident_t_.BaseId,
                     Ident_t_.StorageId,
                     ui->lineEdit_Operator->text().simplified(),
                     ui->comboBox_Departments->currentText());
  connect(&importBatch_,
          SIGNAL(sqlError(const QString&)),
          this,
          SLOT(sqlErrorImportBatch(const QString&)));
  connect(
    &importBatch_, SIGNAL(progressBar(int)), this, SLOT(progressBar(int)));
  importBatch_.exec();
}

void
FormDocuments::progressBar(const int& value_)
{
  progress_.setValue(std::move(value_));
}

/*!
 * \public SLOT
 * \brief FormDocuments::printOneImage_triggered
 */
void
FormDocuments::printOneImage_triggered()
{
  TypeImage timage_;
  if (timage_.isImageViewable(recDoc.st_file)) {
    PrintImage print_;
    print_.setStringList(
      QStringList() << QString::number(ui->label_Ident->text().toInt()),
      "SELECT HIGH_PRIORITY IM_IMAGE FROM DOCIMAGES WHERE ST_IDENT=");
    print_.setOrientation(PrintImage::PageOrientation::Portrait);
    print_.preview();
  } else {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00017).text())
        .arg(recDoc.st_file.isEmpty() ? QString("%0").arg(tr("Não Selecionado"))
                                      : recDoc.st_file),
      QMessageBox::Ok);
  }
}

/*!
 * \public SLOT
 * \brief FormDocuments::printImages_triggered
 */
void
FormDocuments::printImages_triggered()
{
  QItemSelectionModel* selectionModel_ =
    ui->TableView_Documents->selectionModel();
  if (!selectionModel_) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::INTERNAL_ERROR)
                .text()
                .arg(__FUNCTION__)
                .arg(__LINE__)
                .arg(Messages_->set(MessagesNS::Tokens::INTERR_0002).text())),
      QMessageBox::Close);
    delete selectionModel_;
    return;
  }

  QModelIndexList indexList_ = selectionModel_->selectedRows();
  if (indexList_.count() == 0) {
    QMessageBox::information(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00018).text(),
      QMessageBox::Close);
    return;
  }
  if (indexList_.count() > 10) {
    QMessageBox m;
    m.setText(QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00019).text())
                .arg(indexList_.count()));
    m.setInformativeText(
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00020).text());
    m.setWindowTitle(tr("Impressão de Relatórios"));
    QPushButton* canceltButton =
      m.addButton(tr("Cancelar"), QMessageBox::ActionRole);
    QPushButton* okButton =
      m.addButton(tr("Continuar"), QMessageBox::ActionRole);
    Q_UNUSED(okButton);
    m.setDefaultButton(canceltButton);
    m.exec();
    if (m.clickedButton() == canceltButton) {
      ui->TableView_Documents->clearSelection();
      return;
    }
  }

  QStringList strList_SelectedRows_{};

  for (auto idx_ = indexList_.cbegin(); idx_ != indexList_.cend(); ++idx_) {
    if (TypeImage typeImg_; typeImg_.isImageViewable(
          idx_->sibling(idx_->row(), 6).data().toString())) {
      strList_SelectedRows_ << idx_->data().toStringList();
    }
  }

  PrintMultiplesImages print_;
  connect(
    &print_, &PrintMultiplesImages::message, this, [this](const QString& err_) {
      QMessageBox::warning(this, ProgId::Name, err_, QMessageBox::Close);
    });
  connect(&print_,
          &PrintMultiplesImages::sqlLastError,
          this,
          [this](const QString& err_) {
            QMessageBox::warning(this, ProgId::Name, err_, QMessageBox::Close);
          });

  print_.setTitle(ProgId::strName());
  print_.setSubTitle(
    tr("Listagem de Documentos: %0").arg(Ident_t_.mainEntity_));

  print_.setStringList(strList_SelectedRows_,
                       "SELECT IM_IMAGE FROM DOCIMAGES WHERE ST_IDENT=");
  print_.preview();
  disconnect(&print_, SIGNAL(message(const QString&)), nullptr, nullptr);
  disconnect(&print_, SIGNAL(lastError(const QString&)), nullptr, nullptr);

  ui->TableView_Documents->clearSelection();
}

/*!
 * \public SLOT
 * \brief FormDocuments::printListDocs_triggered
 */
void
FormDocuments::printListDocs_triggered()
{
  const QString stmnt_{
    QString(
      "SELECT HIGH_PRIORITY "
      "ST_IDENT, DATE_FORMAT(ST_RECDATE,'%d/%m/%Y') AS ST_RECDATE, "
      "ST_OPERATOR, ST_DEPART, DATE_FORMAT(ST_DOCDATE,'%d/%m/%Y') AS "
      "ST_DOCDATE, "
      "ST_NUMBER, ST_VALUE, ST_FILE, "
      "(SELECT A.ET_NOME FROM DOCENTITY A WHERE A.ET_IDENT=B.ET_IDENT) AS "
      "ENTITY, "
      "(SELECT A.ET_NOME FROM DOCENTITY A WHERE A.ET_IDENT=B.ST_ETRELA) AS "
      "LINK_NAME FROM DOCSTORAGE B WHERE B.ET_IDENT=%0")
      .arg(Ident_t_.BaseId)
  };

  PrintDocGeneral print_;
  print_.setQueryModel(std::move(stmnt_));
  print_.setTitle(ProgId::Name);
  print_.setSubTitle(tr("Listagem Geral do Cadastro de Documentos"));
  print_.setOrientation(PrintDocGeneral::PageOrientation::Portrait);
  print_.preview();
}

/*!
 * \public SLOT
 * \brief FormDocuments::printListValDocs_triggered
 */
void
FormDocuments::printListValDocs_triggered()
{
  /*!
   *  \note we are assuming the database internal date is in yyyy-mm-dd
   * format.
   */
  QString stmnt_where_{ QString("AND %0 BETWEEN '%1' AND '%2'")
                          .arg((datePurpose_ ? "ST_RECDATE" : "ST_DOCDATE"))
                          .arg(dateBegin_.toString("yyyy-MM-dd"))
                          .arg(dateEnd_.toString("yyyy-MM-dd")) };
  const QString stmnt_{
    QString(
      "SELECT HIGH_PRIORITY ST_IDENT, "
      "(SELECT A.ET_NOME FROM  DOCENTITY A WHERE A.ET_IDENT=B.ET_IDENT) AS "
      "ENTITY, "
      "(SELECT A.ET_NOME FROM  DOCENTITY A WHERE A.ET_IDENT=B.ST_ETRELA) "
      "AS "
      "LINK_NAME, "
      "ST_ETRELA, ET_IDENT FROM DOCSTORAGE B WHERE ST_VALUE > 0.0 AND "
      "ET_IDENT=%0 ")
      .arg(Ident_t_.BaseId) +
    (Ident_t_.StorageId > 0
       ? QString(" AND ST_ETRELA = %0").arg(Ident_t_.StorageId)
       : "") +
    stmnt_where_ + " GROUP BY ST_ETRELA ORDER BY ST_IDENT"
  };

  PrintDocAnalitic print_;
  print_.setQueryModel(std::move(stmnt_));
  print_.setWhereClause(std::move(stmnt_where_));
  print_.setTitle(ProgId::Name);
  print_.setSubTitle(tr("Listagem Analítica de Documentos"));
  print_.setOrientation(PrintDocAnalitic::PageOrientation::Portrait);
  print_.preview();
}

/*!
 * \public SLOT
 * \brief FormDocuments::multiFilesErrorText
 */
void
FormDocuments::multiFilesErrorText(const QString& errText_)
{
  QMessageBox::critical(
    this,
    ProgId::Name,
    QString(Messages_->set(MessagesNS::Tokens::FILE_ERROR_READING).text()) +
      fullFileName_ + "<br>" + errText_,
    QMessageBox::Close);
}

void
FormDocuments::inputDialog_operatorSelected(const QString& text_)
{
  newFileName_ = std::move(text_);
}

/*!
 * \private
 * \brief FormDocuments::loadListThumbs
 * \param text_
 */
void
FormDocuments::loadListThumbs(const QString text_)
{
  modelThumbs_->clear();

  const QStringList banished_ = { "scr", "reg", "pif", "crt", "cpl", "bas",
                                  "chm", "cmd", "com", "prf", "scr" };

  QDir dir_;

  QString temp_ = std::move(text_.simplified());
  if (text_.endsWith("*")) {
    temp_.removeLast();
  }

  dir_.setCurrent(temp_);
  dir_.setFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  dir_.setSorting(QDir::Name);

  int x = temp_.lastIndexOf("/", text_.size()) + 1;

  dir_.setNameFilters(QStringList() << text_.last(text_.size() - x));

  QFileInfoList list_ = dir_.entryInfoList();

  if (list_.size() <= 0) {
    return;
  }

  QStringList files_{};
  for (int i = 0; i < list_.size(); ++i) {
    QFileInfo fileInfo_ = list_.at(i);
    if (fileInfo_.isDir()) {
      continue;
    }

    // Ignore file with zero size
    if (fileInfo_.size() == 0) {
      continue;
    }

    // If the file has more than a single dot in the name, it must be
    // ignored
    if (fileInfo_.fileName().count(".") > 1) {
      continue;
    }

    // max length of file name including extension
    if (fileInfo_.fileName().size() > 254) {
      continue;
    }

    // Ignore possible temporary files
    if (fileInfo_.fileName().startsWith("~") ||
        fileInfo_.fileName().endsWith("$$$") ||
        fileInfo_.fileName().startsWith(".") ||
        fileInfo_.fileName().toLower().endsWith("tmp") ||
        fileInfo_.suffix().contains("~")) {
      continue;
    }

    if (Globals::isExecutable(fileInfo_.fileName().toLower())) {
      continue;
    }

    // possible viruses
    if (banished_.contains(fileInfo_.suffix().toLower())) {
      continue;
    }

    files_ << fileInfo_.absolutePath() + "/" + fileInfo_.fileName();
  }

  imagesWatcher_* images_ = new imagesWatcher_(this);
  // Cancel and wait if we are already loading images.
  if (images_->isRunning()) {
    images_->cancel();
    images_->waitForFinished();
  }

  if (files_.count() == 0) {
    return;
  }

  // ==========================================================
  QProgressDialog progress(tr("Carregando Imagens ... "),
                           tr("Cancelar"),
                           0,
                           images_->progressMaximum());
  connect(images_, SIGNAL(finished()), &progress, SLOT(reset()));
  connect(images_, SIGNAL(canceled()), &progress, SLOT(cancel()));
  connect(images_,
          SIGNAL(progressRangeChanged(int, int)),
          &progress,
          SLOT(setRange(int, int)));
  connect(
    images_, SIGNAL(progressValueChanged(int)), &progress, SLOT(setValue(int)));

  QFuture<QPair<QImage, QString>> result_ =
    QtConcurrent::mapped(files_, Globals::loadImages);
  images_->setFuture(result_);

  //==========================================================================
  progress.exec();

  int row_ = 0;
  for (int i = 0; i <= files_.size() - 1; ++i) {
    if (files_.at(i).isEmpty()) {
      break;
    }
    QFileInfo fi(files_.at(i));
    QStandardItem* item = new QStandardItem;
    item->setIcon(QPixmap::fromImage(images_->resultAt(i).first));
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignBottom);
    item->setText(fi.fileName());

    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setData(files_.at(i),
                  Qt::UserRole); // sets full file path for use in showPixmap()
    item->setData(images_->resultAt(i).second,
                  Qt::UserRole + 1);            // set file type
    item->setData(fi.size(), Qt::UserRole + 1); // set the file size
    modelThumbs_->setItem(row_, 0, item);
    ++row_;
  }
  ui->listView_Files->setModel(modelThumbs_);
  ui->listView_Files->update();
}

/*!
 * \private
 * \brief FormDocuments::showDataFieldsOnForm
 */
void
FormDocuments::showDataFieldsOnForm()
{
  ui->toolBox_Docs->setItemEnabled(1, true);

  showTitle(Ident_t_.mainEntity_, Ident_t_.linkedEntity_);

  ui->label_Ident->setText(QString::number(recDoc.st_ident));
  ui->label_CurrDate->setText(recDoc.st_regdate);
  ui->lineEdit_Operator->setText(recDoc.st_operator);
  ui->comboBox_Departments->setCurrentText(recDoc.st_depart);

  ui->groupBox_DocId->setChecked(recDoc.st_doctype);
  ui->lineEdit_DocNumber->setText(QString::number(recDoc.st_number));

  Settings settings_;
  if (Globals::is_ptBR()) {
    ui->dateEdit_DocId->setDate(QDate::fromString(
      Globals::to_ptBR_Date(recDoc.st_docdate), "dd/MM/yyyy"));
  } else {
    ui->dateEdit_DocId->setDate(
      QDate::fromString(recDoc.st_docdate, "yyyy-mm-dd"));
  }

  ui->groupBox_DocId->setChecked(recDoc.st_filetype);
  ui->lineEdit_DocValue->setText(
    QString::number(recDoc.st_value, 'f', 2).simplified());
  ui->comboBox_DocType->setCurrentIndex(recDoc.st_doctype);
  ui->lineEdit_DocIdent->setText(recDoc.st_docident);

  ui->lineEdit_CustomFiled_1->setText(recDoc.st_field01data);
  ui->lineEdit_CustomFiled_2->setText(recDoc.st_field02data);
  ui->lineEdit_CustomFiled_3->setText(recDoc.st_field03data);
  ui->lineEdit_CustomFiled_4->setText(recDoc.st_field04data);
  ui->lineEdit_CustomFiled_5->setText(recDoc.st_field05data);

  ui->textEdit_Comments->setText(recDoc.st_remark);

  /* -------------------------------------------------------------------- */
  ui->graphicsView_Images->resetTransform();

  scene_ = new QGraphicsScene(ui->graphicsView_Images);

  if (TypeImage timg_; timg_.isImageViewable(recDoc.st_file)) {
    if (QPixmap pix_(recDoc.im_image); pix_.loadFromData(recDoc.im_image)) {
      scene_->addPixmap(pix_);
      ui->graphicsView_Images->scale(0.2, 0.2);
    }
  } else {
    scene_->addPixmap(timg_.type(recDoc.st_file));
  }
  ui->graphicsView_Images->setScene(scene_);
  QApplication::restoreOverrideCursor();
  ui->toolBox_Docs->setCurrentIndex(1);
}

/*!
 * \private
 * \brief FormDocuments::showTitle
 * \param mainEntity_
 * \param linkedEntity_
 */
void
FormDocuments::showTitle(const QString mainEntity_, const QString linkedEntity_)
{
  ui->toolBox_Docs->setItemText(1,
                                tr("Registro - Base:[%0] Vinculada:[%1]")
                                  .arg(mainEntity_)
                                  .arg(linkedEntity_));
}
