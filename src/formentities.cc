#include "formentities.h"
#include "br_cnpj_alpha.h"
#include "br_cnpj_v1.h"
#include "br_cpf.h"
#include "customuppervalidator.h"
#include "lineedit_fieldchecking.h"
#include "messages.h"
#include "progid.h"
#include "qobjectdefs.h"
#include "sysactivity.h"
#include "ui_formentities.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>

// STDC++
#include <limits>

/*!
 * \brief FormEntities::FormEntities
 * \param parent
 */
FormEntities::FormEntities(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormEntities)
{
  ui->setupUi(this);

  Messages_ = &Messages_->instance();

  connect(ui->comboBox_Customer,
          SIGNAL(currentIndexChanged(int)),
          this,
          SLOT(comboBox_Customer_currentIndexChanged(int)));

  connect(ui->tableView_Entities,
          SIGNAL(doubleClicked(QModelIndex)),
          this,
          SLOT(tableView_Entities_doubleClicked(QModelIndex)));

  connect(ui->checkBox_RegActive, &QCheckBox::toggled, this, [this]() {
    ui->checkBox_RegActive->setText(
      (ui->checkBox_RegActive->isChecked() ? tr("Ativo") : tr("Inativo")));
    ui->checkBox_RegActive->setIcon(
      (ui->checkBox_RegActive->isChecked()
         ? QPixmap(":/images/png/images/greencircle.png")
         : QPixmap(":/images/png/images/redcircle.png")));
  });

  connect(ui->checkBox_ZipCode_Type,
          SIGNAL(stateChanged(int)),
          this,
          SLOT(checkBox_ZipCode_Type_stateChanged(int)));

  connect(ui->checkBox_showStates,
          &QCheckBox::checkStateChanged,
          this,
          [this](int state_) {
            switch (state_) {
              case Qt::Checked: {
                ui->checkBox_showStates->setText(tr("Ativos"));
                break;
              }
              case Qt::Unchecked: {
                ui->checkBox_showStates->setText(tr("Desativados"));
                break;
              }
              case Qt::PartiallyChecked: {
                ui->checkBox_showStates->setText(tr("Todos"));
              }
            }
            update_data(state_);
          });

  connect(
    ui->radioButton_Company, &QRadioButton::clicked, this, [this](bool b_) {
      enableFields();
      setTabCategories(Globals::Categories::Company);
      update_data(Qt::Checked);
    });

  connect(
    ui->radioButton_Customer, &QRadioButton::clicked, this, [this](bool b_) {
      enableFields();
      setTabCategories(Globals::Categories::Customer);
      update_data(Qt::Checked);
    });

  connect(
    ui->radioButton_Suppliers, &QRadioButton::clicked, this, [this](bool b_) {
      enableFields();
      setTabCategories(Globals::Categories::Supplier);
      update_data(Qt::Checked);
    });

  connect(
    ui->radioButton_Projects, &QRadioButton::clicked, this, [this](bool b_) {
      enableFields();
      setTabCategories(Globals::Categories::Project);
      update_data(Qt::Checked);
    });

  connect(
    ui->radioButton_Custom, &QRadioButton::clicked, this, [this](bool b_) {
      enableFields();
      setTabCategories(Globals::Categories::Custom);
      update_data(Qt::Checked);
    });

  connect(ui->radioButton_Basic, &QRadioButton::clicked, this, [this](bool b_) {
    enableFields();
    setTabCategories(Globals::Categories::Basic);
    update_data(Qt::Checked);
  });

  connect(ui->toolButton_NicknameSuggestion,
          SIGNAL(clicked()),
          this,
          SLOT(toolButton_NicknameSuggestion_clicked()));

  ui->checkBox_RegActive->setChecked(true);
  ui->lineEdit_AddrNumber->setText("0");
  ui->checkBox_ZipCode_Type->setChecked(true);
  ui->checkBox_ZipCode_Type->setText(tr("BR CEP"));
  ui->lineEdit_PostalCode->setInputMask("00.000-000");

  ui->comboBox_StatesAbbrev->addItems(globals.stateAbbrev);
  ui->comboBox_StatesAbbrev->setCurrentText("SP");

  ui->lineEdit_BR_CNPJ->clear();
  ui->lineEdit_BR_CPF->clear();
  ui->lineEdit_BR_IM->clear();
  ui->lineEdit_BR_RG->clear();

  /*!
   *  \brief Configures for the fields to only receive capital letters and valid
   * characters
   */
  const QList<QLineEdit*> allLineEdits = this->findChildren<QLineEdit*>();
  for (QLineEdit* le_ : allLineEdits) {
    if (le_->objectName() == "lineEditEmail1" ||
        le_->objectName() == "lineEditEmail2" ||
        le_->objectName() == "lineEditURL") {
      continue;
    }
    le_->setValidator(new CustomUpperValidator);
  }

  ui->toolBox_Entities->setCurrentIndex(1);
  ui->toolBox_Entities->setItemText(0, tr("Busca"));
  ui->toolBox_Entities->setItemText(1, tr("Dados Cadastrais"));
  ui->toolBox_Entities->setItemText(2, tr("Histórico"));

  ui->tabWidget_Entities->setCurrentIndex((int)TabPages::PageECS);
  ui->tabWidget_Entities->setTabText((int)TabPages::PageECS,
                                     tr("Empresas/Clientes/Fornecedores"));
  ui->tabWidget_Entities->setTabText((int)TabPages::PageProject,
                                     tr("Projetos"));
  ui->tabWidget_Entities->setTabText((int)TabPages::PageCustom,
                                     tr("Personalizados"));

  // Shows the client code for the first item in the comboxbox
  QModelIndex idx_ = ui->comboBox_Customer->model()->index(0, 0);
  QVariant data_ = ui->comboBox_Customer->model()->data(idx_);
  ui->label_ProjId->setText(data_.toString());

  ui->lcdNumber_counter->display(QString::number(Globals::lastEntityId()));
  ui->label_CurrDate->setText(QDate::currentDate().toString(
    (Globals::is_ptBR()) ? "dd/MM/yyyy" : "MM-dd-yyyy"));
  ui->lineEdit_ProjYear->setText(
    std::move(QDate::currentDate().toString("yyyy")));

  connect(
    ui->toolBox_Entities, &QToolBox::currentChanged, this, [this](int index) {
      if (index == 0) {
        update_data(Qt::Checked);
      }
    });
  update_data(Qt::Checked);

  ui->lineEdit_Name->setFocus();
}

/*!
 * \brief FormEntities::~FormEntities
 */
FormEntities::~FormEntities()
{
  delete ui;
}

/*!
 * \brief protected SLOT FormEntities::closeEvent
 * \param event_
 * \note  Disconnect the signal before closing the window. Otherwise Qt
 *  understands the end-of-edit signal first, which tells the program to display
 *  messages at the wrong time.
 *
 */
void
FormEntities::closeEvent(QCloseEvent* event_)
{

  QList<QLineEdit*> allLineEdits = this->findChildren<QLineEdit*>();
  foreach (QLineEdit* le, allLineEdits) {
    disconnect(le, SIGNAL(editingFinished()), this, nullptr);
  }

  /*!
   * \note Informs that the window has been closed. Emits the closed(false)
   * signal so that the toolbar is placed in invisible mode
   */
  emit closed(false);
  emit deleteMenu(Globals::ModulesOfApp::ModEntity);
  event_->accept();
}

/*!
 * \todo DEFINIR O QUE ESTA FUNCAO DEVE FAZER OU SIMPLESMENTE APAGAR
 * \brief FormEntities::isRegActive
 * \return true|false
 */
bool
FormEntities::isRegActive()
{
  return true;
}

/*!
 * \brief SLOT FormEntities::comboBox_Customer_currentIndexChanged
 * \param index_
 *
 * \note Selects the value contained in the first column (customer code) and
 * shows it in the label_ProjId object.
 */
void
FormEntities::comboBox_Customer_currentIndexChanged(int index_)
{
  QModelIndex idx_ = ui->comboBox_Customer->model()->index(index_, 0);
  QVariant data_ = ui->comboBox_Customer->model()->data(idx_);
  ui->label_ProjId->setText(data_.toString());
}

/*!
 * \brief SLOT FormEntities::tableView_Entities_doubleClicked
 * \param idx_
 */
void
FormEntities::tableView_Entities_doubleClicked(QModelIndex index_)
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
  auto column_id_ = index_.model()->sibling(index_.row(), 0, index_);

  const QString stmnt_{
    QString("SELECT ET_IDENT, DATE_FORMAT(ET_DATA,'%d/%m/%Y') AS ET_DATE , "
            "ET_ATIVO, ET_CATEG, "
            "ET_NOME, ET_ALIAS, ET_ENDER, ET_ADDRNUMBER, ET_BAIR, ET_CITY, "
            "ET_UF, ET_POSTALCODE, "
            "ET_CATPHONE1, ET_TELNUM1, ET_CATPHONE2, ET_TELNUM2, "
            "ET_CATPHONE3, ET_TELNUM3, ET_CATPHONE4, ET_TELNUM4, "
            "ET_CONTATO1, ET_EMAIL1, ET_CONTATO2, ET_EMAIL2, "
            "ET_BR_CPF, ET_BR_RG, ET_BR_CNPJ, ET_BR_IE, ET_BR_IM, "
            "ET_OTHERDOCTYPE, ET_URL, "
            "ET_OBCOD, ET_OBANO, "
            "(SELECT B.ET_NOME FROM DOCENTITY B WHERE "
            "B.ET_IDENT=A.ET_OBCLI) AS OBCLINAME_, ET_OBCLI, "
            "ET_OBIDPROJ, ET_OBCRONO, ET_OUTESPEC, ET_OUTNFLD1, "
            "ET_OUTDFLD1, ET_OUTNFLD2, ET_OUTDFLD2, ET_OUTOBS, ET_OBSGER "
            "FROM DOCENTITY A WHERE ET_IDENT=%0")
      .arg(std::move(column_id_.data().toString()))
  };

  QSqlQueryModel qmodel_;
  qmodel_.setQuery(stmnt_);
  if (qmodel_.lastError().isValid()) {
    QMessageBox::critical(
      this, ProgId::Name, qmodel_.lastError().text(), QMessageBox::Close);
  } else {
    auto showData_ = [&qmodel_](const QString fldName_) {
      return qmodel_.record(0).value(fldName_);
    };

    ui->lcdNumber_counter->display(showData_("ET_IDENT").toString());
    ui->label_CurrDate->setText(showData_("ET_DATE").toString());

    emit ui->checkBox_RegActive->toggled(showData_("ET_ATIVO").toBool());

    switch (static_cast<Globals::Categories>(showData_("ET_CATEG").toInt())) {
      case Globals::Categories::Company: {
        ui->radioButton_Company->setChecked(true);
        break;
      }
      case Globals::Categories::Customer: {
        ui->radioButton_Customer->setChecked(true);
        break;
      }
      case Globals::Categories::Supplier: {
        ui->radioButton_Suppliers->setChecked(true);
        break;
      }
      case Globals::Categories::Project: {
        ui->radioButton_Projects->setChecked(true);
        break;
      }
      case Globals::Categories::Custom: {
        ui->radioButton_Custom->setChecked(true);
        break;
      }
      case Globals::Categories::Basic: {
        ui->radioButton_Basic->setChecked(true);
        break;
      }
      case Globals::Categories::None: {
        ;
      }
    }

    ui->textEdit_Comments->setText(
      std::move(showData_("ET_OBSGER").toString()));

    ui->lineEdit_Name->setText(showData_("ET_NOME").toString());
    ui->lineEdit_Alias->setText(showData_("ET_ALIAS").toString());
    ui->lineEdit_Address->setText(showData_("ET_ENDER").toString());
    ui->lineEdit_AddrNumber->setText(showData_("ET_ADDRNUMBER").toString());
    ui->lineEdit_District->setText(showData_("ET_BAIR").toString());
    ui->lineEdit_City->setText(showData_("ET_CITY").toString());
    ui->comboBox_StatesAbbrev->setCurrentText(showData_("ET_UF").toString());
    ui->lineEdit_PostalCode->setText(showData_("ET_POSTALCODE").toString());

    ui->comboBox_Phone_1->setCurrentIndex(
      std::move(showData_("ET_CATPHONE1").toInt()));
    ui->lineEdit_Phone_1->setText(
      std::move(showData_("ET_TELNUM1").toString()));
    ui->comboBox_Phone_2->setCurrentIndex(
      std::move(showData_("ET_CATPHONE2").toInt()));
    ui->lineEdit_Phone_2->setText(
      std::move(showData_("ET_TELNUM2").toString()));
    ui->comboBox_Phone_3->setCurrentIndex(
      std::move(showData_("ET_CATPHONE3").toInt()));
    ui->lineEdit_Phone_3->setText(
      std::move(showData_("ET_TELNUM3").toString()));
    ui->comboBox_Phone_4->setCurrentIndex(
      std::move(showData_("ET_CATPHONE4").toInt()));
    ui->lineEdit_Phone_4->setText(
      std::move(showData_("ET_TELNUM4").toString()));

    ui->toolBox_Entities->setCurrentIndex(1);

    ui->lineEdit_Contact_1->setText(
      std::move(showData_("ET_CONTATO1").toString()));
    ui->lineEdit_Email_1->setText(std::move(showData_("ET_EMAIL1").toString()));
    ui->lineEdit_Contact_2->setText(
      std::move(showData_("ET_CONTATO2").toString()));
    ui->lineEdit_Email_2->setText(std::move(showData_("ET_EMAIL2").toString()));

    setTabCategories(
      static_cast<Globals::Categories>(showData_("ET_CATEG").toInt()));

    ui->lineEdit_BR_CNPJ->setText(
      std::move(showData_("ET_BR_CNPJ").toString()));
    ui->lineEdit_BR_IE->setText(std::move(showData_("ET_BR_IE").toString()));
    ui->lineEdit_BR_IM->setText(std::move(showData_("ET_BR_IM").toString()));
    ui->lineEdit_OtherId->setText(
      std::move(showData_("ET_OTHERDOCTYPE").toString()));
    ui->lineEdit_BR_CPF->setText(std::move(showData_("ET_BR_CPF").toString()));
    ui->lineEdit_BR_RG->setText(std::move(showData_("ET_BR_RG").toString()));

    ui->lineEdit_URL->setText(std::move(showData_("ET_URL").toString()));

    ui->lineEdit_ProjId->setText(std::move(showData_("ET_OBCOD").toString()));
    ui->lineEdit_ProjYear->setText(std::move(showData_("ET_OBANO").toString()));

    ui->comboBox_Customer->setCurrentText(
      std::move(showData_("OBCLINAME_").toString()));
    ui->label_ProjId->setText(std::move(showData_("ET_OBCLI").toString()));

    ui->lineEdit_DocIdProject->setText(
      std::move(showData_("ET_OBIDPROJ").toString()));

    ui->lineEdit_CustomSpecTitle->setText(
      std::move(showData_("ET_OUTSPEC").toString()));
    ui->lineEdit_CustomNameFld_1->setText(
      std::move(showData_("ET_OUTNFLD1").toString()));
    ui->lineEdit_CustomDataFld_1->setText(
      std::move(showData_("ET_OUTDFLD1").toString()));
    ui->lineEdit_CustomNameFld_2->setText(
      std::move(showData_("ET_OUTNFLD2").toString()));
    ui->lineEdit_CustomDataFld_2->setText(
      std::move(showData_("ET_OUTDFLD2").toString()));
  }
}

/*!
 * \brief FormEntities::update_data
 * \param state_
 */
void
FormEntities::update_data(int state_)
{

  auto whatCategory_ = [this]() {
    if (ui->radioButton_Company->isChecked()) {
      return QString("ET_CATEG = 0");
    } else if (ui->radioButton_Customer->isChecked()) {
      return QString("ET_CATEG = 1");
    } else if (ui->radioButton_Suppliers->isChecked()) {
      return QString("ET_CATEG = 2");
    } else if (ui->radioButton_Projects->isChecked()) {
      return QString("ET_CATEG = 3");
    } else if (ui->radioButton_Custom->isChecked()) {
      return QString("ET_CATEG = 4");
    } else if (ui->radioButton_Basic->isChecked()) {
      return QString("ET_CATEG = 5");
    } else {
      return QString();
    }
  };

  QString qry_{ "SELECT ET_IDENT, ET_NOME, IF( ET_BR_CNPJ = '', ET_BR_CPF, "
                "ET_BR_CNPJ) AS DOC, ET_DATA FROM DOCENTITY WHERE " };

  switch (state_) {
    case Qt::Checked: { // Active
      qry_ += "ET_ATIVO = 1 AND " + (whatCategory_()) + " ORDER BY ET_NOME";
      break;
    }
    case Qt::Unchecked: { // Deactivated
      qry_ += "ET_ATIVO = 0 AND " + (whatCategory_()) + " ORDER BY ET_NOME";
      break;
    }
    case Qt::PartiallyChecked: { // All
      qry_ += (whatCategory_()) + " ORDER BY ET_NOME";
    }
  }

  QSqlQueryModel* model_ = new QSqlQueryModel;
  model_->clear();
  model_->setQuery(qry_);
  if (!model_->lastError().isValid()) {
    if (model_->rowCount() > 0) {
      model_->setHeaderData(0, Qt::Horizontal, tr("ID"));
      model_->setHeaderData(1, Qt::Horizontal, tr("Nome/Razão Social"));
      model_->setHeaderData(2, Qt::Horizontal, tr("Documento"));
      model_->setHeaderData(3, Qt::Horizontal, tr("Data\nCadastro"));
      ui->tableView_Entities->setModel(model_);
      ui->tableView_Entities->resizeColumnToContents(0);
      ui->tableView_Entities->resizeColumnToContents(1);
      ui->tableView_Entities->resizeColumnToContents(2);
      ui->tableView_Entities->resizeColumnToContents(3);
      show();
    } else {
      model_->setHeaderData(0, Qt::Horizontal, tr("ID"));
      model_->setHeaderData(1, Qt::Horizontal, tr("Nome/Razão Social"));
      model_->setHeaderData(2, Qt::Horizontal, tr("Documento"));
      model_->setHeaderData(3, Qt::Horizontal, tr("Data\nCadastro"));
      ui->tableView_Entities->setModel(model_);
    }
  }
}

/*!
 * \brief SLOT FormEntities::checkBox_ZipCode_Type_stateChanged
 * \param state_
 */
void
FormEntities::checkBox_ZipCode_Type_stateChanged(int state_)
{
  ui->lineEdit_PostalCode->clear();
  switch (state_) {
    case Qt::Checked: {
      ui->checkBox_ZipCode_Type->setText(tr("BR CEP"));
      ui->lineEdit_PostalCode->setInputMask("00.000-000");
      ui->lineEdit_PostalCode->setToolTip(
        tr("Código de Endereçamento Postal Brasileiro"));
      break;
    }
    case Qt::Unchecked: {
      ui->checkBox_ZipCode_Type->setText(tr("Outros"));
      ui->lineEdit_PostalCode->setInputMask("");
      ui->lineEdit_PostalCode->setMaxLength(10);
      ui->lineEdit_PostalCode->setPlaceholderText(tr("Código"));
      ui->lineEdit_PostalCode->setToolTip(tr("Outros formatos de código"));
      break;
    }
    case Qt::PartiallyChecked: {
      ui->checkBox_ZipCode_Type->setText(tr("US ZIP"));
      ui->lineEdit_PostalCode->setInputMask("00000-0000");
      ui->lineEdit_PostalCode->setToolTip(
        tr("Código de Endereçamento Postal Norte Americano"));
    }
  }
}

/*!
 * \public SLOT
 * \brief qDocBase::actionPrintActiveEntities_triggered
 */
void
FormEntities::printActionEntities_triggered(bool b_)
{
  printRecords(true);
}

/*!
 * \public SLOT
 * \brief qDocBase::actionPrintInactiveEntities_triggered
 */
void
FormEntities::printInactiveEntities_triggered(bool b_)
{
  printRecords(false);
}

/*!
 * \brief FormEntities::toolButton_NicknameSuggestion_clicked
 */
void
FormEntities::toolButton_NicknameSuggestion_clicked()
{
  QStringList words_ = ui->lineEdit_Name->text().split(" ");
  ui->lineEdit_Alias->setText(words_.at(0));
}

/*!
 * \brief FormEntities::saveRecord
 */
void
FormEntities::saveRecord()
{
  if (fieldChecks()) {
    execQuery(ExecSaveOrUpdate::Save);
  }
}

/*!
 * \brief FormEntities::updateRecord
 */
void
FormEntities::updateRecord()
{
  if (fieldChecks(ExecSaveOrUpdate::Update)) {
    execQuery(ExecSaveOrUpdate::Update);
  }
}

/*!
 * \brief FormEntities::deleteRecord
 */
void
FormEntities::deleteRecord()
{
  QMessageBox messageBox(this);
  messageBox.setIcon(QMessageBox::Warning);
  messageBox.setWindowTitle(ProgId::Name);
  messageBox.setTextFormat(Qt::PlainText);
  messageBox.setText(tr("<b>** Este procedimento é irreversível! **</b>"));
  messageBox.setInformativeText(
    Messages_->set(MessagesNS::Tokens::GEN_WARN_00010).text());
  QAbstractButton* cancelButton =
    messageBox.addButton(tr("Cancelar"), QMessageBox::ActionRole);
  [[maybe_unused]] QAbstractButton* continueButton =
    messageBox.addButton(tr("Continuar"), QMessageBox::ActionRole);
  messageBox.setEscapeButton(cancelButton);

  messageBox.exec();
  if (messageBox.clickedButton() == continueButton) {
    SysActivity* sact = new SysActivity(this);
    if (!sact->logger(Globals::ModulesOfApp::ModEntity,
                      SysActivity::Operations::Delete,
                      ui->lcdNumber_counter->value())) {
      delete sact;
      return;
    }

    QSqlQuery qry_;
    qry_.prepare("DELETE FROM DOCENTITY WHERE ET_IDENT=:ET_IDENT");
    qry_.bindValue(":ET_IDENT", std::move(ui->lcdNumber_counter->value()));
    qry_.exec();
    if (qry_.lastError().isValid() || (qry_.numRowsAffected() == 0)) {
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
      Messages_->set(MessagesNS::Tokens::DB_WARN_RECDELETED).text(),
      QMessageBox::Ok);
    clearRecord();
    ui->lineEdit_Name->setFocus();
  }
}

/*!
 * \brief FormEntities::execQuery
 * \param control_
 */
void
FormEntities::execQuery(ExecSaveOrUpdate control_)
{
  QString stmnt_{};

  switch (control_) {
    case ExecSaveOrUpdate::Save: {
      stmnt_ = QString(std::move(insert_stmnt_));
      break;
    }
    case ExecSaveOrUpdate::Update: {
      stmnt_ = QString(std::move(update_stmnt_));
    }
  }

  QSqlDatabase::database().transaction();
  QSqlQuery qry_;
  qry_.prepare(stmnt_);

  if (control_ == ExecSaveOrUpdate::Update) {
    qry_.bindValue(":ET_IDENT", std::move(ui->lcdNumber_counter->value()));
  }

  qry_.bindValue(":ET_DATA",
                 std::move(QDate::currentDate())); // ISO yyyy-mm-dd

  qry_.bindValue(":ET_ATIVO", std::move(ui->checkBox_RegActive->isChecked()));
  qry_.bindValue(":ET_CATEG", std::move(Globals::whatCategory(this)));

  qry_.bindValue(":ET_NOME", std::move(ui->lineEdit_Name->text().simplified()));
  qry_.bindValue(":ET_ALIAS",
                 std::move(ui->lineEdit_Alias->text().simplified()));
  qry_.bindValue(":ET_ENDER",
                 std::move(ui->lineEdit_Address->text().simplified()));
  qry_.bindValue(":ET_ADDRNUMBER", std::move(ui->lineEdit_AddrNumber->text()));
  qry_.bindValue(":ET_BAIR",
                 std::move(ui->lineEdit_District->text().simplified()));
  qry_.bindValue(":ET_CITY", std::move(ui->lineEdit_City->text().simplified()));
  qry_.bindValue(":ET_UF",
                 (ui->comboBox_StatesAbbrev->currentText().isEmpty()
                    ? nullptr
                    : std::move(ui->comboBox_StatesAbbrev->currentText())));
  qry_.bindValue(":ET_POSTALCODE",
                 (ui->lineEdit_PostalCode->text().simplified() == "..-" ||
                      ui->lineEdit_PostalCode->text().simplified() == "-"
                    ? nullptr
                    : std::move(ui->lineEdit_PostalCode->text())));

  qry_.bindValue(":ET_CATPHONE1",
                 std::move(static_cast<int>(
                   Globals::indexPhoneCategory(ui->comboBox_Phone_1))));
  qry_.bindValue(":ET_TELNUM1",
                 std::move(ui->lineEdit_Phone_1->text().simplified()));
  qry_.bindValue(":ET_CATPHONE2",
                 std::move(static_cast<int>(
                   Globals::indexPhoneCategory(ui->comboBox_Phone_2))));
  qry_.bindValue(":ET_TELNUM2",
                 std::move(ui->lineEdit_Phone_2->text().simplified()));
  qry_.bindValue(":ET_CATPHONE3",
                 std::move(static_cast<int>(
                   Globals::indexPhoneCategory(ui->comboBox_Phone_3))));
  qry_.bindValue(":ET_TELNUM3",
                 std::move(ui->lineEdit_Phone_3->text().simplified()));
  qry_.bindValue(":ET_CATPHONE4",
                 std::move(static_cast<int>(
                   Globals::indexPhoneCategory(ui->comboBox_Phone_4))));
  qry_.bindValue(":ET_TELNUM4",
                 std::move(ui->lineEdit_Phone_4->text().simplified()));

  qry_.bindValue(":ET_CONTATO1",
                 std::move(ui->lineEdit_Contact_1->text().simplified()));
  qry_.bindValue(":ET_EMAIL1",
                 std::move(ui->lineEdit_Email_1->text().simplified()));
  qry_.bindValue(":ET_CONTATO2",
                 std::move(ui->lineEdit_Contact_2->text().simplified()));
  qry_.bindValue(":ET_EMAIL2",
                 std::move(ui->lineEdit_Email_2->text().simplified()));

  qry_.bindValue(":ET_BR_CPF",
                 (ui->lineEdit_BR_CPF->text() == "..-"
                    ? nullptr
                    : std::move(ui->lineEdit_BR_CPF->text())));

  qry_.bindValue(":ET_BR_RG",
                 (ui->lineEdit_BR_RG->text() == "..-"
                    ? nullptr
                    : std::move(ui->lineEdit_BR_RG->text())));
  qry_.bindValue(":ET_BR_CNPJ",
                 (ui->lineEdit_BR_CNPJ->text() == "../-"
                    ? nullptr
                    : std::move(ui->lineEdit_BR_CNPJ->text())));

  qry_.bindValue(":ET_BR_IE",
                 (ui->lineEdit_BR_IE->text() == "..."
                    ? nullptr
                    : std::move(ui->lineEdit_BR_IE->text())));

  qry_.bindValue(":ET_BR_IM",
                 (ui->lineEdit_BR_IM->text().simplified().isEmpty()
                    ? nullptr
                    : std::move(ui->lineEdit_BR_IM->text().simplified())));

  qry_.bindValue(":ET_OTHERDOCTYPE",
                 (ui->lineEdit_OtherId->text().simplified().isEmpty()
                    ? nullptr
                    : std::move(ui->lineEdit_OtherId->text().simplified())));

  qry_.bindValue(":ET_URL",
                 (ui->lineEdit_URL->text().simplified().isEmpty()
                    ? nullptr
                    : std::move(ui->lineEdit_URL->text().simplified())));

  if (!ui->comboBox_Customer->currentText().isEmpty()) {
    qry_.bindValue(":ET_OBCOD", std::move(ui->lineEdit_ProjId->text().toInt()));
    qry_.bindValue(":ET_OBANO",
                   std::move(ui->lineEdit_ProjYear->text().toInt()));
  } else {
    qry_.bindValue(":ET_OBCOD", 0);
    qry_.bindValue(":ET_OBANO", 0);
  }

  if (Globals::whatCategory(this) ==
      static_cast<int>(Globals::Categories::Project)) {
    qry_.bindValue(":ET_OBCLI",
                   (!ui->comboBox_Customer->currentText().isEmpty()
                      ? std::move(ui->label_ProjId->text())
                      : 0));
  } else {
    qry_.bindValue(":ET_OBCLI", 0);
  }

  qry_.bindValue(":ET_OBIDPROJ", std::move(ui->lineEdit_DocIdProject->text()));

  qry_.bindValue(":ET_OBCRONO",
                 std::move(ui->spinBox_ProjectTimeLine->value()));
  qry_.bindValue(":ET_OUTESPEC",
                 std::move(ui->lineEdit_CustomSpecTitle->text().simplified()));
  qry_.bindValue(":ET_OUTNFLD1",
                 std::move(ui->lineEdit_CustomNameFld_1->text().simplified()));
  qry_.bindValue(":ET_OUTDFLD1",
                 std::move(ui->lineEdit_CustomDataFld_1->text().simplified()));
  qry_.bindValue(":ET_OUTNFLD2",
                 std::move(ui->lineEdit_CustomNameFld_2->text().simplified()));
  qry_.bindValue(":ET_OUTDFLD2",
                 std::move(ui->lineEdit_CustomDataFld_2->text().simplified()));

  qry_.bindValue(":ET_OUTOBS",
                 std::move(ui->lineEdit_CustomComments->text().simplified()));
  qry_.bindValue(":ET_OBSGER", std::move(ui->textEdit_Comments->toPlainText()));

  qry_.exec();
  if (qry_.lastError().isValid()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      qry_.lastError().text() + "\n\n" +
        Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text(),
      QMessageBox::Close);
    return;
  }

  QSqlDatabase::database().commit();
  this->setStatusTip(
    Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text());

  if (control_ == ExecSaveOrUpdate::Save) {
    clearRecord();
    ui->lcdNumber_counter->display(QString::number(Globals::lastEntityId()));
  }
}

/*!
 * \brief FormEntities::fieldChecks
 */
bool
FormEntities::fieldChecks(ExecSaveOrUpdate control_)
{
  if (ui->radioButton_Basic->isChecked()) {
    ui->lineEdit_Name->setFieldDescription(
      tr("%0").arg(ui->lineEdit_Name->placeholderText()));
    if (ui->lineEdit_Name->emptyField()) {
      ui->lineEdit_Name->setFocus();
      return false;
    }
    if (ui->lineEdit_Name->invalidChars()) {
      ui->lineEdit_Name->setFocus();
      return false;
    }
    return true;
  }
  ui->lineEdit_Name->setFieldDescription(
    tr("%0").arg(ui->lineEdit_Name->placeholderText()));
  if (ui->lineEdit_Name->emptyField()) {
    ui->lineEdit_Name->setFocus();
    return false;
  }
  if (ui->lineEdit_Name->invalidChars()) {
    ui->lineEdit_Name->setFocus();
    return false;
  }
  // ------------------------------------------------------------------------
  ui->lineEdit_Alias->setFieldDescription(
    tr("%0").arg(ui->lineEdit_Alias->placeholderText()));
  if (ui->lineEdit_Alias->emptyField()) {
    ui->lineEdit_Alias->setFocus();
    return false;
  }
  if (ui->lineEdit_Alias->invalidChars()) {
    ui->lineEdit_Alias->setFocus();
    return false;
  }
  // ------------------------------------------------------------------------
  ui->lineEdit_Address->setFieldDescription(
    tr("%0").arg(ui->lineEdit_Address->placeholderText()));
  if (ui->lineEdit_Address->emptyField()) {
    ui->lineEdit_Address->setFocus();
    return false;
  }
  if (ui->lineEdit_Address->invalidChars()) {
    ui->lineEdit_Address->setFocus();
    return false;
  }
  if ((ui->lineEdit_AddrNumber->text().toInt() < 0) ||
      (ui->lineEdit_AddrNumber->text().toInt() > 999999)) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      Messages_->set(MessagesNS::Tokens::GEN_WARN_00001).text(),
      QMessageBox::Close);
    ui->lineEdit_AddrNumber->setFocus();
    return false;
  }
  // ------------------------------------------------------------------------
  ui->lineEdit_District->setFieldDescription(
    tr("%0").arg(ui->lineEdit_District->placeholderText()));
  if (ui->lineEdit_District->emptyField()) {
    ui->lineEdit_District->setFocus();
    return false;
  }
  if (ui->lineEdit_District->invalidChars()) {
    ui->lineEdit_District->setFocus();
    return false;
  }
  // ------------------------------------------------------------------------
  ui->lineEdit_City->setFieldDescription(
    tr("%0").arg(ui->lineEdit_City->placeholderText()));
  if (ui->lineEdit_City->emptyField()) {
    ui->lineEdit_City->setFocus();
    return false;
  }
  if (ui->lineEdit_City->invalidChars()) {
    ui->lineEdit_City->setFocus();
    return false;
  }
  // ------------------------------------------------------------------------
  ui->lineEdit_Contact_1->setFieldDescription(
    tr("%0").arg(ui->lineEdit_Contact_1->placeholderText()));
  if (ui->lineEdit_Contact_1->invalidChars()) {
    ui->lineEdit_Contact_1->setFocus();
    return false;
  }
  // ------------------------------------------------------------------------
  ui->lineEdit_Contact_2->setFieldDescription(
    tr("%0").arg(ui->lineEdit_Contact_2->placeholderText()));
  if (ui->lineEdit_Contact_2->invalidChars()) {
    ui->lineEdit_Contact_1->setFocus();
    return false;
  }
  // ------------------------------------------------------------------------

  BR_CPF* cpf_ = new BR_CPF;
  cpf_->setData(ui->lineEdit_BR_CPF->text().simplified());

  auto cnpjExists_ = [](const QString& cnpj_) {
    QSqlQuery qry_(
      QString("SELECT ET_IDENT FROM DOCENTITY WHERE ET_BR_CNPJ='%0'")
        .arg(cnpj_.simplified()));
    if (qry_.exec()) {
      bool ok_ = qry_.next();
      return ok_;
    }
    return false;
  };

  BR_CNPJ_V1 cnpj_v1_;
  switch (cnpj_v1_.whichCNPJModel()) {
    case BR_CNPJ_V1::TRModel::V1: {
      cnpj_v1_.setData(ui->lineEdit_BR_CNPJ->text().simplified());

      if (cnpj_v1_.isNullOrEmpty()) {
        QMessageBox::warning(
          this,
          ProgId::Name,
          Messages_->set(MessagesNS::Tokens::GEN_WARN_00022).text(),
          QMessageBox::Close);
        ui->lineEdit_BR_CNPJ->setFocus();
        return false;
      }

      if (cnpj_v1_.isValid()) {
        bool exits_ = cnpjExists_(cnpj_v1_.data());
        if (exits_ && (control_ == ExecSaveOrUpdate::Update)) {
          return true;
        }
        if (exits_) {
          QMessageBox::warning(
            this,
            ProgId::Name,
            QString(Messages_->set(MessagesNS::Tokens::DB_WARN_RECALREADYEXISTS)
                      .text())
              .arg(cnpj_v1_.data()),
            QMessageBox::Close);
          ui->lineEdit_BR_CNPJ->setFocus();
          return false;
        }
      } else {
        QMessageBox::warning(
          this,
          ProgId::Name,
          QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00008).text())
            .arg(tr("CNPJ"),
                 cnpj_v1_.section().V1_dv_,
                 QString::number(cnpj_v1_.VD())),
          QMessageBox::Close);
        ui->lineEdit_BR_CNPJ->setFocus();
        return false;
      }
      break;
    }
    default: {
      BR_CNPJ_ALPHA cnpj_alpha_;
      cnpj_alpha_.setData(ui->lineEdit_BR_CNPJ->text().simplified());
      if (cnpj_alpha_.isNullOrEmpty()) {
        QMessageBox::warning(
          this,
          ProgId::Name,
          Messages_->set(MessagesNS::Tokens::GEN_WARN_00022).text(),
          QMessageBox::Close);
        ui->lineEdit_BR_CNPJ->setFocus();
        return false;
      }

      if (cnpj_alpha_.isValid()) {
        bool exits_ = cnpjExists_(cnpj_alpha_.data());
        if (exits_ && (control_ == ExecSaveOrUpdate::Update)) {
          return true;
        }
        if (exits_) {
          QMessageBox::warning(
            this,
            ProgId::Name,
            QString(Messages_->set(MessagesNS::Tokens::DB_WARN_RECALREADYEXISTS)
                      .text())
              .arg(cnpj_alpha_.data()),
            QMessageBox::Close);
          ui->lineEdit_BR_CNPJ->setFocus();
          return false;
        }
      } else {
        QMessageBox::warning(
          this,
          ProgId::Name,
          QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00008).text())
            .arg(tr("CNPJ"),
                 cnpj_alpha_.section().dv_,
                 QString::number(cnpj_alpha_.VD())),
          QMessageBox::Close);
        ui->lineEdit_BR_CNPJ->setFocus();
        return false;
      }
    }
  }
  // ------------------------------------------------------------------------
  if (ui->radioButton_Projects->isChecked()) {
    if ((ui->lineEdit_ProjId->text().toInt() == 0) ||
        (ui->lineEdit_ProjId->text().toInt() >
         std::numeric_limits<short>::max())) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        Messages_->set(MessagesNS::Tokens::GEN_WARN_00003).text(),
        QMessageBox::Close);
      ui->lineEdit_ProjId->setFocus();
      return false;
    }
    if ((ui->lineEdit_ProjYear->text().toInt() > QDate::currentDate().year()) ||
        (ui->lineEdit_ProjYear->text().toInt() <
         QDate::currentDate().year() - 1)) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        Messages_->set(MessagesNS::Tokens::GEN_WARN_00004).text(),
        QMessageBox::Close);
      ui->lineEdit_ProjYear->setFocus();
      return false;
    }
  }
  // ------------------------------------------------------------------------
  if (ui->radioButton_Custom->isChecked()) {
    ui->lineEdit_CustomSpecTitle->setFieldDescription(
      tr("%0").arg(ui->lineEdit_CustomSpecTitle->placeholderText()));
    if (ui->lineEdit_CustomSpecTitle->emptyField()) {
      ui->lineEdit_CustomSpecTitle->setFocus();
      return false;
    }
    if (ui->lineEdit_CustomSpecTitle->invalidChars()) {
      ui->lineEdit_CustomSpecTitle->setFocus();
      return false;
    }

    if (!ui->lineEdit_CustomDataFld_1->text().simplified().isEmpty() &&
        ui->lineEdit_CustomNameFld_1->text().simplified().isEmpty()) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00005).text())
          .arg(1),
        QMessageBox::Close);
      ui->lineEdit_CustomNameFld_1->setFocus();
      return false;
    }

    if (!ui->lineEdit_CustomDataFld_2->text().simplified().isEmpty() &&
        ui->lineEdit_CustomNameFld_2->text().simplified().isEmpty()) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00005).text())
          .arg(2),
        QMessageBox::Close);
      ui->lineEdit_CustomNameFld_2->setFocus();
      return false;
    }

    if (ui->lineEdit_CustomComments->invalidChars()) {
      ui->lineEdit_CustomComments->setFocus();
      return false;
    }
  }
  return true;
}

/*!
 * \brief FormEntities::clearRecord
 */
void
FormEntities::clearRecord()
{
  const QList<QLineEdit*> allLineEdits = this->findChildren<QLineEdit*>();
  for (QLineEdit* le_ : allLineEdits) {
    le_->clear();
  }
  ui->textEdit_Comments->clear();
  ui->checkBox_RegActive->setChecked(true);
  ui->radioButton_Company->setChecked(true);

  ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageECS, true);
  ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageProject, false);
  ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageCustom, false);

  ui->lcdNumber_counter->display(QString::number(Globals::lastEntityId()));
  ui->label_CurrDate->setText(QDate::currentDate().toString(
    (Globals::is_ptBR()) ? "dd/MM/yyyy" : "MM-dd-yyyy"));

  ui->lineEdit_ProjId->clear();
  ui->lineEdit_ProjYear->setText(
    std::move(QDate::currentDate().toString("yyyy")));

  ui->lineEdit_AddrNumber->setText("0");

  ui->comboBox_Customer->setCurrentIndex(0);
}

/*!
 * \brief FormEntities::printRecords(bool isActive_)
 */
void
FormEntities::printRecords(bool isActive_)
{
  if (!Globals::isCategoryChecked(this)) {
    return;
  }

  const QString stmnt_{
    QString("SELECT ET_IDENT, DATE_FORMAT(ET_DATA,'%0') AS ET_DATA, ")
      .arg((Globals::is_ptBR() ? "%d/%m/%Y" : "%m-%d-%Y")) +
    QString(" CASE WHEN ET_CATEG=0 THEN '%0' WHEN ET_CATEG=1 THEN '%1' WHEN "
            "ET_CATEG=2 THEN '%2' "
            "WHEN ET_CATEG=3 THEN '%3' WHEN ET_CATEG=4 THEN '%4' WHEN "
            "ET_CATEG=5 THEN '%5' END AS NAME_CATEGORY, ")
      .arg(Globals::categoryName(Globals::Categories::Company),
           Globals::categoryName(Globals::Categories::Customer),
           Globals::categoryName(Globals::Categories::Supplier),
           Globals::categoryName(Globals::Categories::Project),
           Globals::categoryName(Globals::Categories::Custom),
           Globals::categoryName(Globals::Categories::Basic)) +
    QString(
      "ET_NOME, ET_ALIAS, ET_ENDER, ET_ADDRNUMBER, ET_BAIR, ET_CITY, ET_UF, "
      "ET_POSTALCODE, "
      "ET_CATPHONE1, ET_TELNUM1, ET_CATPHONE2, ET_TELNUM2, "
      "ET_CATPHONE3, ET_TELNUM3, ET_CATPHONE4, ET_TELNUM4, "
      "ET_CONTATO1, ET_EMAIL1, ET_CONTATO2, ET_EMAIL2, "
      "ET_BR_CPF, ET_BR_RG, ET_BR_CNPJ, ET_BR_IE, ET_BR_IM, ET_OTHERDOCTYPE, "
      "ET_URL, CONCAT_WS('-',LPAD(ET_OBCOD,5,'0'),LPAD(ET_OBANO,2,'0')) AS "
      "OBRA_CODIGO, "
      "(SELECT B.ET_NOME FROM DOCENTITY B WHERE B.ET_IDENT=A.ET_OBCLI) AS "
      "ET_OBCLI, ET_OBIDPROJ, ET_OBCRONO, "
      "ET_OUTESPEC, ET_OUTNFLD1, ET_OUTDFLD1, ET_OUTNFLD2, ET_OUTDFLD2, "
      "ET_OUTOBS ") +
    QString("FROM DOCENTITY A WHERE ET_ATIVO=%0 AND ET_CATEG=%1")
      .arg((isActive_ ? 1 : 0), Globals::whatCategory(this))
  };

  PrintActiveEntities print_;
  print_.setQueryModel(stmnt_);
  print_.setTitle(ProgId::strName());
  print_.setSubTitle(tr("Listagem do Cadastro de Entidades - Ativas"));
  print_.setOrientation(PrintActiveEntities::PageOrientation::Landscape);
  print_.preview();
}

/*!
 * \brief FormEntities::setTabCategories
 * \param categ_
 */
void
FormEntities::setTabCategories(Globals::Categories categ_)
{
  switch (categ_) {
    case Globals::Categories::Company:
    case Globals::Categories::Customer:
    case Globals::Categories::Supplier: {
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageECS, true);
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageProject, false);
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageCustom, false);
      ui->toolButton_NicknameSuggestion->setDisabled(false);
      break;
    }
    case Globals::Categories::Project: {
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageECS, false);
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageProject, true);
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageCustom, false);
      ui->tabWidget_Entities->setCurrentIndex((int)TabPages::PageProject);
      ui->toolButton_NicknameSuggestion->setDisabled(false);
      break;
    }
    case Globals::Categories::Custom: {
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageECS, false);
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageProject, false);
      ui->tabWidget_Entities->setTabEnabled((int)TabPages::PageCustom, true);
      ui->tabWidget_Entities->setCurrentIndex((int)TabPages::PageCustom);
      ui->toolButton_NicknameSuggestion->setDisabled(false);
      break;
    }
    case Globals::Categories::Basic: {
      disableFields();
      ui->toolButton_NicknameSuggestion->setDisabled(true);
      ui->comboBox_Phone_1->setDisabled(true);
      ui->comboBox_Phone_2->setDisabled(true);
      ui->comboBox_Phone_3->setDisabled(true);
      ui->comboBox_Phone_4->setDisabled(true);
    }
    case Globals::Categories::None: {
    }
  }
}

/*!
 * \brief FormEntities::enableFields
 */
void
FormEntities::enableFields()
{
  const QList<QLineEdit*> allLineEdits = this->findChildren<QLineEdit*>();
  for (QLineEdit* le_ : allLineEdits) {
    le_->setEnabled(true);
  }
  ui->tabWidget_Entities->setEnabled(true);
  ui->comboBox_Phone_1->setDisabled(false);
  ui->comboBox_Phone_2->setDisabled(false);
  ui->comboBox_Phone_3->setDisabled(false);
  ui->comboBox_Phone_4->setDisabled(false);
}

/*!
 * \brief FormEntities::disableFields
 */
void
FormEntities::disableFields()
{
  const QList<QLineEdit*> allLineEdits = this->findChildren<QLineEdit*>();
  for (QLineEdit* le_ : allLineEdits) {
    if (le_->objectName() != "lineEdit_Name") {
      le_->setEnabled(false);
    }
  }
  ui->tabWidget_Entities->setEnabled(false);
}

// ==========================================================================
#define PRINTREPORT_
#ifdef PRINTREPORT_
void
PrintActiveEntities::printReport(QPrinter* prt_)
{
  painter_.begin(prt_);

  QFont font10("Courier New", 10);
  font10 = QFont(font10, painter_.device());

  QFont font8("Courier New", 8);
  font8 = QFont(font8, painter_.device());
  QFontMetricsF fontMetrics8(font8);

  QFont font8Bold("Courier New", 8, QFont::Bold);
  font8Bold = QFont(font8Bold, painter_.device());

  int npag_{ 1 };
  qreal y = printDefaultHeader(npag_);
  const qreal pageHeight_ = pageHeight();
  const qreal leftMargin_ = getLeftMargin();

  painter_.setFont(font8);

  QProgressDialog progress(
    tr("Gerando Relatório ..."), tr("Cancelar"), 0, sqlModel_->rowCount() - 1);
  progress.setWindowModality(Qt::ApplicationModal);

  for (int i = 0; i <= sqlModel_->rowCount() - 1; ++i) {
    qApp->processEvents();
    if (progress.wasCanceled()) {
      break;
    }
    progress.setValue(i);

    const QString l1 =
      QString(tr("#ID: %0 DATA: %1 CATEGORIA: %2"))
        .arg(sqlModel_->record(i)
               .value("ET_IDENT")
               .toString()
               .rightJustified(10, ' '))
        .arg(sqlModel_->record(i).value("ET_DATA").toString())
        .arg(sqlModel_->record(i).value("NAME_CATEGORY").toString());

    const QString l2 =
      QString(tr("NOME: %0 APELIDO: %1"))
        .arg(sqlModel_->record(i).value("ET_NOME").toString().leftJustified(
          60, ' '))
        .arg(sqlModel_->record(i).value("ET_ALIAS").toString());

    const QString l3 =
      QString(tr("ENDEREÇO: %0 BAIRRO: %1"))
        .arg(sqlModel_->record(i)
               .value("ET_ENDER")
               .toString()
               .leftJustified(60, ' '))
        .arg(sqlModel_->record(i).value("ET_BAIR").toString().leftJustified(
          60, ' '));

    const QString l4 =
      QString(tr("CIDADE: %0 UF: %1 CEP: %2"))
        .arg(sqlModel_->record(i).value("ET_CITY").toString().leftJustified(
          60, ' '))
        .arg(
          sqlModel_->record(i).value("ET_UF").toString().leftJustified(2, ' '))
        .arg(sqlModel_->record(i)
               .value("ET_POSTALCODE")
               .toString()
               .leftJustified(10, ' '));

    const QString l5 = QString(tr("TELS: [%0-%1] [%2-%3]"))
                         .arg(sqlModel_->record(i)
                                .value("ET_TELMOD1")
                                .toString()
                                .leftJustified(25, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_TELNUM1")
                                .toString()
                                .leftJustified(30, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_TELMOD2")
                                .toString()
                                .leftJustified(25, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_TELNUM2")
                                .toString()
                                .leftJustified(30, ' '));

    const QString l6 = QString(tr("TELS: [%0-%1] [%2-%3]"))
                         .arg(sqlModel_->record(i)
                                .value("ET_TELMOD3")
                                .toString()
                                .leftJustified(25, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_TELNUM3")
                                .toString()
                                .leftJustified(30, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_TELMOD4")
                                .toString()
                                .leftJustified(25, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_TELNUM4")
                                .toString()
                                .leftJustified(30, ' '));

    const QString l7 =
      QString(tr("CONTATOS: %0 EMAIL: %1"))
        .arg(sqlModel_->record(i)
               .value("ET_CONTATO1")
               .toString()
               .leftJustified(40, ' '))
        .arg(sqlModel_->record(i).value("ET_EMAIL1").toString());

    const QString l8 =
      QString(tr("CONTATOS: %0 EMAIL: %1"))
        .arg(sqlModel_->record(i)
               .value("ET_CONTATO2")
               .toString()
               .leftJustified(40, ' '))
        .arg(sqlModel_->record(i).value("ET_EMAIL2").toString());

    const QString l9 = QString(tr("CPF/RG: %0 / %1 CPNJ/IE/IM: %2 / %3 / %4"))
                         .arg(sqlModel_->record(i)
                                .value("ET_BR_CPF")
                                .toString()
                                .leftJustified(19, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_BR_RG")
                                .toString()
                                .leftJustified(21, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_BR_CNPJ")
                                .toString()
                                .leftJustified(19, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_BR_IE")
                                .toString()
                                .leftJustified(20, ' '))
                         .arg(sqlModel_->record(i)
                                .value("ET_BR_IM")
                                .toString()
                                .leftJustified(20, ' '));

    const QString l10 =
      tr("URL:") + sqlModel_->record(i).value("ET_URL").toString();

    const QString l11 = QString(tr("CÓDIGO: %0 CLIENTE: %1"))
                          .arg(sqlModel_->record(i)
                                 .value("OBRA_CODIGO")
                                 .toString()
                                 .leftJustified(8, ' '))
                          .arg(sqlModel_->record(i)
                                 .value("ET_OBCLI")
                                 .toString()
                                 .leftJustified(60, ' '));
    const QString l12 = QString(tr("CEI: %0 CRONOGRAMA: %1"))
                          .arg(sqlModel_->record(i)
                                 .value("ET_OBIDPROJ")
                                 .toString()
                                 .leftJustified(20, ' '))
                          .arg(sqlModel_->record(i)
                                 .value("ET_OBCRONO")
                                 .toString()
                                 .rightJustified(6, ' '));

    const QString l13 = QString(tr("ESPECIFICAÇÃO: %0"))
                          .arg(sqlModel_->record(i)
                                 .value("ET_OUTESPEC")
                                 .toString()
                                 .rightJustified(50, ' '));

    const QString l14 =
      QString(tr("%0: %1"))
        .arg(sqlModel_->record(i)
               .value("ET_OUTNFLD1")
               .toString()
               .rightJustified(15, ' '))
        .arg(sqlModel_->record(i).value("ET_OUTDFLD1").toString());

    const QString l15 =
      QString(tr("%0: %1"))
        .arg(sqlModel_->record(i)
               .value("ET_OUTNFLD2")
               .toString()
               .rightJustified(15, ' '))
        .arg(sqlModel_->record(i).value("ET_OUTDFLD2").toString());

    const QString l16 = QString(tr("%0:")).arg(
      sqlModel_->record(i).value("ET_OUTOBS1").toString());

    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l1)), l1);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l2)), l2);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l3)), l3);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l4)), l4);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l5)), l5);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l6)), l6);

    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l7)), l7);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l8)), l8);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l9)), l9);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l10)), l10);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l11)), l11);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l12)), l12);

    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l13)), l13);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l14)), l14);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l15)), l15);
    painter_.drawText(
      QPointF(leftMargin_, y += getFontRectHeight(fontMetrics8, l16)), l16);

    qreal y1_ = y += getFontRectHeight(fontMetrics8, "X");
    painter_.drawLine(QPointF(leftMargin_, y1_), QPointF(pageWidth(), y1_));

    if (y >= pageHeight_ - 1500) {
      prt_->newPage();
      y = printDefaultHeader(++npag_);
      painter_.setFont(font8);
    }
  }
  painter_.end();
  progress.setValue(sqlModel_->rowCount() - 1);
}
#endif

// EOF
