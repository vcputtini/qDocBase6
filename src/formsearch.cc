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
#include "formsearch.h"
#include "ui_formsearch.h"

#include "customuppervalidator.h"
#include "exportfiles.h"
#include "multifiles.h"
#include "printimage.h"
#include "printmultiplesimages.h"
#include "progid.h"
#include "settings.h"
#include "tableview_results_itemdelegate.h"
#include "typeimage.h"

// STDLib C++
#include <memory>

/*!
 * \brief FormSearch::FormSearch
 * \param parent
 */
FormSearch::FormSearch(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormSearch)
  , Ident_t_{}
  , qryModel_(new QSqlQueryModel)
  , tempFiles_(new TemporaryFiles)
{
  ui->setupUi(this);

  Messages_ = &Messages_->instance();

  ui->lineEdit_UserName->setValidator(new CustomUpperValidator);
  ui->toolBox_MainSearch->setCurrentIndex(0);

  connect(ui->treeView_BaseEntities,
          SIGNAL(clicked(QModelIndex)),
          this,
          SLOT(treeView_BaseEntities_clicked(QModelIndex)));

  connect(ui->checkBox_AscOrDesc,
          SIGNAL(clicked(bool)),
          this,
          SLOT(checkBox_AscOrDesc_clicked(bool)));

  connect(ui->checkBox_DateType,
          SIGNAL(clicked(bool)),
          this,
          SLOT(checkBox_DateType_clicked(bool)));

  connect(ui->tableView_Results,
          SIGNAL(clicked(QModelIndex)),
          this,
          SLOT(tableView_Results_clicked(QModelIndex)));

  TableView_Results_ItemDelegate* delegateResults =
    new TableView_Results_ItemDelegate(ui->tableView_Results);
  ui->tableView_Results->setItemDelegate(delegateResults);

  fillSortingFields();
  fillComboBoxDeparts();
  fillCustomFields();
}

/*!
 * \brief FormSearch::~FormSearch
 */
FormSearch::~FormSearch()
{
  tempFiles_->remove();
  delete tempFiles_;
  delete qryModel_;
  delete ui;
}

/*!
 * \protected
 * \brief FormDocuments::closeEvent
 * \param event_
 */
void
FormSearch::closeEvent(QCloseEvent* event_)
{
  /*!
   * \note Informs that the window has been closed. Emits the closed(false)
   * signal so that the toolbar is placed in invisible mode
   */
  emit closed(false);
  emit deleteMenu(Globals::ModulesOfApp::ModSearch);
  event_->accept();
}

/*!
 * \public SLOT
 * \brief FormSearch::urlError
 * \param e_
 */
void
FormSearch::urlError(const QString e_)
{
  QMessageBox::warning(this, ProgId::Name, e_, QMessageBox::Cancel);
}

/*!
 * \public SLOT
 * \brief FormSearch::exportDocuments_clicked
 */
void
FormSearch::exportDocuments_clicked()
{
  ExportFiles* expFiles_ = new ExportFiles(this, ui->tableView_Results);
  connect(
    expFiles_, SIGNAL(error(QString)), this, SLOT(errorExportFiles(QString)));
  expFiles_->exec();
}

/*!
 * \private
 * \brief FormSearch::fillComboBoxDeparts
 */
void
FormSearch::fillComboBoxDeparts()
{
  ui->comboBox_Departaments->clear();

  QSqlQuery qry_;
  qry_.exec("SELECT DP_NAME FROM DOCDEPARTMENTS ORDER BY DP_NAME");
  if (qry_.lastError().isValid()) {
    return;
  }

  int i{ 0 };
  while (qry_.next()) {
    ui->comboBox_Departaments->insertItem(std::move(i),
                                          std::move(qry_.value(0).toString()));
    ++i;
  }
  qry_.clear();
}

/*!
 * \private
 * \brief FormSearch::fillSortingFields
 */
void
FormSearch::fillSortingFields()
{
  ui->comboBox_SortingFields->clear();

  const QStringList list_ = { SORT_01, SORT_02, SORT_03,
                              SORT_04, SORT_05, SORT_06 };

  ui->comboBox_SortingFields->insertItems(0, std::move(list_));
}

/*!
 * \private
 * \brief FormSearch::fillCustomFields
 */
void
FormSearch::fillCustomFields()
{
  ui->comboBox_FieldName->clear();
  Settings settings_;
  QStringList list_ = { std::move(settings_.CFldName(1)),
                        std::move(settings_.CFldName(2)),
                        std::move(settings_.CFldName(3)),
                        std::move(settings_.CFldName(4)),
                        std::move(settings_.CFldName(5)) };
  ui->comboBox_FieldName->insertItems(0, std::move(list_));
}

/*!
 * \private
 * \brief showTitle
 * \param mainEntity
 */
void
FormSearch::showTitle(const QString& mainEntity_)
{
  ui->toolBox_MainSearch->setItemText(
    1, tr("%0 - Base:[%1]").arg(tr("Filtros")).arg(mainEntity_));
}

/*!
 * \public SLOT
 * \brief FormSearch::treeView_BaseEntities_clicked
 * \param index_
 */
void
FormSearch::treeView_BaseEntities_clicked(QModelIndex index_)
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

  /*!
   * \note Returns only the value of the column with the customer identification
   * (customer code), no matter which column the user clicks on.
   */
  auto column_id_ = index_.model()->sibling(index_.row(), 1, index_);
  Ident_t_.BaseId = column_id_.data().toULongLong();

  // Determines which category is in use and returns the corresponding index.
  // auto categories_id_ = static_cast<Globals::Categories>(
  //  index_.model()->data(index_.parent(), Qt::UserRole).toInt());
  Ident_t_.Categorie =
    std::move(index_.model()->data(index_.parent(), Qt::UserRole).toInt());

  column_id_ = index_.model()->sibling(index_.row(), 2, index_);
  Ident_t_.mainEntity_ = column_id_.data().toString();

  showTitle(Ident_t_.mainEntity_);

  ui->toolBox_MainSearch->setCurrentIndex(1);
}

void
FormSearch::clearFields_clicked()
{
  ui->checkBox_AscOrDesc->setCheckState(Qt::Checked);
  ui->comboBox_SortingFields->setCurrentIndex(0);

  ui->groupBox_DateRange->setChecked(false);

  ui->groupBox_Secutiry->setChecked(false);
  ui->lineEdit_UserName->clear();
  ui->lineEdit_UserPasswd->clear();

  ui->checkBox_DateType->setCheckState(Qt::Checked);

  ui->lineEdit_DataField->clear();
  ui->lineEdit_Historical->clear();

  ui->groupBox_Historical->setChecked(false);
  ui->groupBox_BaseEntities->setChecked(false);
  ui->checkBox_HistoricalExactlySame->setChecked(false);

  ui->groupBox_CustomFields->setChecked(false);
  ui->checkBox_CustomExactlySame->setChecked(false);

  if (qryModel_) {
    qryModel_->clear();
    ui->tableView_Results->setModel(qryModel_);
  }

  if (ui->graphicsView_Docs->scene() != nullptr) {
    scene_->clear();
    ui->graphicsView_Docs->setScene(scene_);
  }
}

/*!
 * \public SLOT
 * \brief FormSearch::toolButtonExport_clicked
 */
void
FormSearch::toolButtonExport_clicked()
{
  ExportFiles* expFiles_ = new ExportFiles(this, ui->tableView_Results);
  connect(
    expFiles_, SIGNAL(error(QString)), this, SLOT(errorExportFiles(QString)));
  expFiles_->setBaseId(Ident_t_.BaseId);
  expFiles_->exec();
}

/*!
 * \public SLOT
 * \brief FormSearch::checkBox_AscOrDesc_clicked
 */
void
FormSearch::checkBox_AscOrDesc_clicked(bool status_)
{
  (status_ ? ui->checkBox_AscOrDesc->setIcon(QIcon::fromTheme("go-up"))
           : ui->checkBox_AscOrDesc->setIcon(QIcon::fromTheme("go-down")));
}

/*!
 * \public SLOT
 * \brief FormSearch::checkBox_DateType_clicked
 * \param status_
 */
void
FormSearch::checkBox_DateType_clicked(bool status_)
{
  (status_ ? ui->checkBox_DateType->setText(tr("Data do Registro"))
           : ui->checkBox_DateType->setText(tr("Data do Documento")));
}

/*!
 * \public SLOT
 * \brief FormSearch::treeView_Results_clicked
 * \param index_
 */
void
FormSearch::tableView_Results_clicked(QModelIndex index_)
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

  const int row_ = qryModel_->data(qryModel_->index(index_.row(), 0)).toInt();
  const QString fname_ =
    qryModel_->data(qryModel_->index(index_.row(), 8)).toString();
  if (fname_.isEmpty()) {
    return;
  }

  // Used in the destructor function so we can remove the created temporary
  // files.
  tempFiles_->add(QString("%0/%1").arg(QDir::tempPath()).arg(fname_));

  ui->graphicsView_Docs->resetTransform();
  scene_ = new QGraphicsScene(ui->graphicsView_Docs);

  TypeImage typeImage_;
  if (typeImage_.isImageViewable(fname_)) {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QSqlQuery qry_(QString("SELECT IM_IMAGE FROM DOCIMAGES WHERE ST_IDENT=%0")
                     .arg(std::move(row_)));

    qry_.next();
    if (qry_.lastError().isValid()) {
      QMessageBox::warning(
        this, ProgId::Name, qry_.lastError().text(), QMessageBox::Close);
      QApplication::restoreOverrideCursor();
      return;
    }

    const QByteArray ba_ =
      Globals::unCompressImage(qry_.value(0).toByteArray());

    QApplication::restoreOverrideCursor();

    if (QPixmap pix_(ba_); pix_.loadFromData(ba_)) {
      ItemInterceptDoubleClick* gItem_ = new ItemInterceptDoubleClick;
      connect(gItem_,
              SIGNAL(openUrlError(const QString)),
              this,
              SLOT(urlError(const QString)));
      connect(gItem_,
              SIGNAL(queryError(const QString)),
              this,
              SLOT(urlError(const QString)));
      gItem_->setProperty(row_, fname_);
      gItem_->setPixmap(pix_);
      scene_->addItem(gItem_);
      ui->graphicsView_Docs->scale(0.2, 0.2);
      ui->graphicsView_Docs->setScene(scene_);
    }
    return;
  }

  ItemInterceptDoubleClick* gItem_ = new ItemInterceptDoubleClick;
  connect(gItem_,
          SIGNAL(openUrlError(const QString)),
          this,
          SLOT(urlError(const QString)));
  connect(gItem_,
          SIGNAL(queryError(const QString)),
          this,
          SLOT(urlError(const QString)));
  gItem_->setProperty(row_, fname_);
  gItem_->setPixmap(typeImage_.type(fname_));
  scene_->addItem(gItem_);
  ui->graphicsView_Docs->setScene(scene_);

  QApplication::restoreOverrideCursor();

  // If there is an icon for the file type it is shown, otherwise it shows a
  // generic icon
  scene_->addPixmap(typeImage_.type(fname_));
  ui->graphicsView_Docs->scale(0.5, 0.5);
  ui->graphicsView_Docs->setScene(scene_);
}

/*!
 * \public SLOT
 * \brief FormSearch::execSearch
 */
void
FormSearch::execSearch()
{
  auto sb_ = std::make_unique<SqlBuilder>(Ident_t_.BaseId);
  sb_->setSorting(
    ui->groupBox_Ordering, ui->comboBox_SortingFields, ui->checkBox_AscOrDesc);
  sb_->setDepartments(ui->groupBox_Departs, ui->comboBox_Departaments);
  sb_->setDateRange(ui->groupBox_DateRange,
                    ui->dateEdit_From,
                    ui->dateEdit_To,
                    ui->checkBox_DateType);
  sb_->setHistorical(ui->groupBox_Historical,
                     ui->lineEdit_Historical,
                     ui->checkBox_HistoricalExactlySame);

  if (ui->groupBox_Secutiry->isChecked()) {
    if (!Globals::isOperatorValid(
          ui->lineEdit_UserName->text().simplified(),
          ui->lineEdit_UserPasswd->text().simplified())) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        Messages_->set(MessagesNS::Tokens::GEN_WARN_00015).text(),
        QMessageBox::Close);
      ui->lineEdit_UserName->setFocus();
      return;
    }
    sb_->setUnLock(ui->lineEdit_UserName, ui->lineEdit_UserPasswd);
  } else {
    sb_->setUnLock(ui->lineEdit_UserName, ui->lineEdit_UserPasswd);
  }

  sb_->setCustomField(ui->groupBox_CustomFields,
                      ui->comboBox_FieldName,
                      ui->lineEdit_DataField,
                      ui->checkBox_CustomExactlySame);

  qryModel_->setQuery(std::move(sb_->result()));
  qryModel_->setHeaderData(0, Qt::Horizontal, tr("#ID"));
  qryModel_->setHeaderData(1, Qt::Horizontal, tr("Data\nCadastro"));
  qryModel_->setHeaderData(2, Qt::Horizontal, tr("Operador"));
  qryModel_->setHeaderData(3, Qt::Horizontal, tr("Departamento"));
  qryModel_->setHeaderData(4, Qt::Horizontal, tr("Vínculo"));
  qryModel_->setHeaderData(5, Qt::Horizontal, tr("Data\nDocumento"));
  qryModel_->setHeaderData(6, Qt::Horizontal, tr("Número"));
  qryModel_->setHeaderData(7, Qt::Horizontal, tr("Valor R$"));
  qryModel_->setHeaderData(8, Qt::Horizontal, tr("Arquivo"));
  qryModel_->setProperty("colFileName", 8);

  if (ui->groupBox_CustomFields->isChecked()) {
    qryModel_->setHeaderData(
      9, Qt::Horizontal, ui->comboBox_FieldName->currentText());
  }

  ui->tableView_Results->setModel(qryModel_);

  for (int i = 0; i < qryModel_->columnCount(); ++i) {
    ui->tableView_Results->resizeColumnToContents(i);
  }
}

/*!
 * \public SLOT
 * \brief FormSearch::zoomIn_clicked
 */
void
FormSearch::zoomIn_clicked()
{
  Globals::zoomImage(Globals::ZoomImage::In, ui->graphicsView_Docs);
}

/*!
 * \public SLOT
 * \brief FormSearch::zoomOut_clicked
 */
void
FormSearch::zoomOut_clicked()
{
  Globals::zoomImage(Globals::ZoomImage::Out, ui->graphicsView_Docs);
}

/*!
 * \public SLOT
 * \brief FormSearch::printCurrentDoc_clicked
 */
void
FormSearch::printCurrentDoc_triggered()
{
  QItemSelectionModel* selectionModel_ =
    ui->tableView_Results->selectionModel();
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

  QModelIndex index_ = selectionModel_->currentIndex();
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
  auto column_file_ = index_.model()->sibling(index_.row(), 8, index_);

  TypeImage timage_;
  if (timage_.isImageViewable(column_file_.data().toString())) {
    PrintImage print_;
    print_.setStringList(
      QStringList() << QString::number(column_id_.data().toULongLong()),
      "SELECT HIGH_PRIORITY IM_IMAGE FROM DOCIMAGES WHERE ST_IDENT=");
    print_.setOrientation(PrintImage::PageOrientation::Portrait);
    print_.preview();
  } else {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00017).text())
        .arg(column_file_.data().toString().isEmpty()
               ? QString("%0").arg(tr("Não Selecionado"))
               : column_file_.data().toString()),
      QMessageBox::Ok);
  }
}

/*!
 * \public SLOT
 * \brief FormSearch::printDocsList_clicked
 */
void
FormSearch::printDocsList_triggered()
{
  QItemSelectionModel* selectionModel_ =
    ui->tableView_Results->selectionModel();
  if (!selectionModel_) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(
        Messages_->set(MessagesNS::Tokens::INTERNAL_ERROR)
          .text()
          .arg(__FUNCTION__)
          .arg(__LINE__)
          .arg(QString(Messages_->set(MessagesNS::Tokens::INTERR_0002).text())
                 .arg("QItemSelectionModel"))),
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
      ui->tableView_Results->clearSelection();
      return;
    }
  }

  QStringList strList_SelectedRows_{};

  for (auto idx_ = indexList_.cbegin(); idx_ != indexList_.cend(); ++idx_) {
    if (TypeImage typeImg_; typeImg_.isImageViewable(
          idx_->sibling(idx_->row(), 8).data().toString())) {
      strList_SelectedRows_ << idx_->sibling(idx_->row(), 0).data().toString();
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
  disconnect(&print_, SIGNAL(sqlLastError(const QString&)), nullptr, nullptr);

  ui->tableView_Results->clearSelection();
}

/* ======================================================================= */

/*!
 * \brief SqlBuilder::SqlBuilder
 */
SqlBuilder::SqlBuilder(const quint64 base_id_)
  : baseId_{ base_id_ }
  , stmnt_{ std::move(QString(select_base_).arg(baseId_)) }
  , order_by_{}
  , sortFields_m_{ { 0, "ST_IDENT" },    { 1, "ET_NOME" },
                   { 2, "ST_OPERATOR" }, { 3, "ST_RECDATE" },
                   { 4, "ST_DOCDATE" },  { 5, "ST_DEPART" } }
  , unlocked_{ false }
{
}

/*!
 * \brief SqlBuilder::~SqlBuilder
 */
SqlBuilder::~SqlBuilder() {}

/*!
 * \public
 * \brief SqlBuilder::setSorting
 * \param gb_
 * \param cb_
 * \param asc_or_desc_
 * \return
 */
SqlBuilder&
SqlBuilder::setSorting(QGroupBox* gb_, QComboBox* cb_, QCheckBox* asc_or_desc_)
{

  if (gb_->objectName() == "groupBox_Ordering") {
    order_by_ = QString(" ORDER BY A.%0 %1")
                  .arg(std::move(sortFields_m_.value(cb_->currentIndex())))
                  .arg((asc_or_desc_->isChecked() ? "ASC" : "DESC"));
  }
  return *this;
}

/*!
 * \brief SqlBuilder::setDepartments
 * \param gb_
 * \param cb_
 * \return
 */
SqlBuilder&
SqlBuilder::setDepartments(QGroupBox* gb_, QComboBox* cb_)
{
  if (gb_->objectName() == "groupBox_Departs" && gb_->isChecked()) {
    stmnt_ +=
      QString(" AND A.ST_DEPART='%0' ").arg(std::move(cb_->currentText()));
  }
  return *this;
}

/*!
 * \brief SqlBuilder::setDateRange
 * \param gb_
 * \param deFrom_
 * \param deTo_
 * \param datetype_
 * \return
 */
SqlBuilder&
SqlBuilder::setDateRange(QGroupBox* gb_,
                         QDateEdit* deFrom_,
                         QDateEdit* deTo_,
                         QCheckBox* datetype_)
{
  if (gb_->objectName() == "groupBox_DateRange" && gb_->isChecked()) {
    stmnt_ +=
      QString(" AND %0 BETWEEN '%1' AND '%2' ")
        .arg((datetype_->isChecked() ? " A.ST_RECDATE " : " A.ST_DOCDATE "))
        .arg(std::move(Globals::toISODate(deFrom_->text())))
        .arg(std::move(Globals::toISODate(deTo_->text())));
  }
  return *this;
}

/*!
 * \brief SqlBuilder::setUnLock
 * \param gb_
 * \param user_
 * \return
 */
SqlBuilder&
SqlBuilder::setUnLock(QLineEdit* user_, QLineEdit* passwd_)
{
  if (!user_->text().isEmpty() && !passwd_->text().isEmpty()) {
    stmnt_ += QString(" AND A.ST_OPERATOR='%0' AND A.ST_TOPSEC=1")
                .arg(std::move(user_->text()));
  } else if (!user_->text().isEmpty() && passwd_->text().isEmpty()) {
    stmnt_ += QString(" AND A.ST_OPERATOR='%0' AND A.ST_TOPSEC=0")
                .arg(std::move(user_->text()));
  } else {
    stmnt_ += QString(" AND A.ST_TOPSEC=0");
  }
  return *this;
}

/*!
 * \brief SqlBuilder::setHistorical
 * \param gb_
 * \param hist_
 * \param equal_
 * \return
 */
SqlBuilder&
SqlBuilder::setHistorical(QGroupBox* gb_, QLineEdit* hist_, QCheckBox* equal_)
{
  if (gb_->objectName() == "groupBox_Historical" && gb_->isChecked()) {
    if (equal_->isChecked()) {
      stmnt_ += QString(" AND A.ST_REMARK='%0' ")
                  .arg(std::move(hist_->text().simplified()));
    } else {
      stmnt_ += QString(" AND A.ST_REMARK LIKE '%%0%' ")
                  .arg(std::move(hist_->text().simplified()));
    }
  }
  return *this;
}

/*!
 * \brief SqlBuilder::setCustomField
 * \param gb_
 * \param fldname_
 * \param flddata
 * \param equal_
 * \return
 */
SqlBuilder&
SqlBuilder::setCustomField(QGroupBox* gb_,
                           QComboBox* fldname_,
                           QLineEdit* flddata_,
                           QCheckBox* equal_)
{
  if (gb_->objectName() == "groupBox_CustomFields" && gb_->isChecked()) {
    if (qsizetype pos_ = stmnt_.indexOf("#"); pos_ > -1) {
      stmnt_.remove(pos_, 1);
      stmnt_.insert(pos_,
                    QString(",A.ST_FIELD0%0DATA ")
                      .arg(std::move(fldname_->currentIndex() + 1)));
    }
    if (equal_->isChecked()) {
      stmnt_ += QString(" AND A.ST_FIELD0%0DATA='%1'")
                  .arg(std::move(fldname_->currentIndex() + 1))
                  .arg(std::move(flddata_->text().simplified()));
    } else {
      stmnt_ += QString(" AND A.ST_FIELD0%0DATA LIKE '%%1%'")
                  .arg(std::move(fldname_->currentIndex() + 1))
                  .arg(std::move(flddata_->text().simplified()));
    }
  }
  return *this;
}

bool
SqlBuilder::isUnlocked() const
{
  return unlocked_;
}

/*!
 * \brief SqlBuilder::result
 * \return
 */
const QString
SqlBuilder::result()
{
  if (qsizetype pos_ = stmnt_.indexOf("#"); pos_ > -1) {
    stmnt_.remove(pos_, 1);
  }
  return stmnt_ + order_by_;
};

/* ======================================================================= */
/*!
 * \private
 * \brief ItemInterceptDoubleClick::ItemInterceptDoubleClick
 */
ItemInterceptDoubleClick::ItemInterceptDoubleClick()
{
  Messages_ = &Messages_->instance();
}

/*!
 * \protected
 * \brief ItemInterceptDoubleClick::mouseDoubleClickEvent
 * \param event
 */
void
ItemInterceptDoubleClick::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
  if (Globals::isExecutable(fname_)) {
    emit openUrlError(
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00012).text());
    event->ignore();
    return;
  }

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  QSqlQuery qry_(
    QString("SELECT IM_IMAGE FROM DOCIMAGES WHERE ST_IDENT=%0").arg(id_));
  qry_.next();
  if (qry_.lastError().isValid()) {
    QApplication::restoreOverrideCursor();
    emit queryError(qry_.lastError().text());
    event->ignore();
    return;
  }

  const QByteArray ba_ = Globals::unCompressImage(qry_.value(0).toByteArray());

  MultiFiles* mf_ = new MultiFiles;
  connect(mf_, SIGNAL(error(QString)), this, SLOT(internalError(QString)));
  QString locTemp_ = QDir::tempPath();

  QString outFile_;
#ifdef Q_OS_WIN32
  outFile_ = QString("%0/%1").arg(locTemp_).arg(fname_);
#else
  outFile_ = QString("/%0/%1").arg(locTemp_).arg(fname_);
#endif

  mf_->setOutputFile(outFile_);
  mf_->setData(ba_);
  mf_->execute();
  QApplication::restoreOverrideCursor();

  if (!QDesktopServices::openUrl(
        QUrl("file:///" + outFile_, QUrl::TolerantMode))) {
    emit openUrlError(
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00013).text());
    event->ignore();
    return;
  }

  QApplication::restoreOverrideCursor();
  event->accept();
}

/*!
 * \public
 * \brief ItemInterceptDoubleClick::setProperty
 * \param ident_
 * \param fileName_
 */
void
ItemInterceptDoubleClick::setProperty(const qint64 ident_,
                                      const QString fileName_)
{
  id_ = std::move(ident_);
  fname_ = std::move(fileName_);
}

/*!
 * \public SLOT
 * \brief ItemInterceptDoubleClick::internalError
 * \param err_
 */
void
ItemInterceptDoubleClick::internalError(const QString err_)
{
  emit fileError(err_);
}
