#include "qdocbase.h"
#include "progid.h"
#include "ui_qdocbase.h"

/*!
 * \brief Constructor qDocBase::qDocBase
 * \param parent
 */
qDocBase::qDocBase(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::qDocBase)
  , datePurpose_{ tr("Data do Cadastro"), tr("Data do Documento") }
{
  ui->setupUi(this);

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  sqlDb_ = QSqlDatabase::addDatabase("QMARIADB");

  QSettings settings_(ProgId::strOrganization(), ProgId::strInternalName());
  if (sqlDb_.isDriverAvailable("QMARIADB") && sqlDb_.isValid()) {
    sqlDb_.setHostName(settings_.value("hostname").toString());
    sqlDb_.setUserName(settings_.value("dbuser").toString());
    sqlDb_.setPassword(settings_.value("dbpass").toString());
    sqlDb_.setDatabaseName(settings_.value("dbname").toString());

    if (!sqlDb_.open()) {
      if (sqlDb_.lastError().nativeErrorCode().toInt() == 2003) {
        QMessageBox::critical(
          this,
          ProgId::Name,
          tr("You must configure the connection parameters whit the "
             "database\nOnce this is done, restart the application."),
          QMessageBox::Abort);
        QApplication::restoreOverrideCursor();
        qTerminate();
      } else {
        QMessageBox::critical(
          this, ProgId::Name, sqlDb_.lastError().text(), QMessageBox::Abort);
        QApplication::restoreOverrideCursor();
        qTerminate();
      }
    }
  } else {
    this->setWindowTitle(tr("%0 (Mode: No database connection avaliable)")
                           .arg(this->windowTitle()));
  }

  QApplication::restoreOverrideCursor();

  CreateActions();
  CreateMenus();
}

/*!
 * \brief qDocBase::~qDocBase
 */
qDocBase::~qDocBase()
{
  sqlDb_.close();
  delete ui;
}

/*!
 * \brief qDocBase::CreateMenus
 */
void
qDocBase::CreateMenus()
{
  ui->menu_Main_Actions->addAction(action_Modules);
  ui->menu_Main_Actions->addSeparator();
  ui->menu_Main_Actions->addAction(action_Exit);

  ui->menu_Main_Help->addAction(action_About);
  ui->menu_Main_Help->addAction(action_AboutQt);

  ui->menu_Tools->addAction(action_DialogSettings);
  ui->menu_Tools->addAction(action_FormTopSecret);

  popupPrintMenu = new QMenu;
}

/*!
 * \brief qDocBase::CreateActions
 */
void
qDocBase::CreateActions()
{
  action_Modules = new QAction(this);
  action_Modules = ui->modules_dockWidget->toggleViewAction();
  action_Modules->setText(tr("&Módulos"));
  action_Modules->setShortcut(QString(tr("Ctrl+M")));
  action_Modules->setStatusTip(tr("Acesso aos Módulos do Sistema"));

  action_Exit = new QAction(this);
  action_Exit->setText(tr("Saída"));
  action_Exit->setShortcut(QString(tr("Ctrl+Q")));
  action_Exit->setStatusTip(tr("Fecha o Programa"));
  connect(
    action_Exit, SIGNAL(triggered()), this, SLOT(action_Exit_triggered()));

  action_DialogSettings = new QAction(this);
  action_DialogSettings->setText(tr("C&onfigurações"));
  action_DialogSettings->setShortcut(QString(tr("Ctrl+O")));
  action_DialogSettings->setStatusTip(tr("Configurações do Sistema"));
  connect(action_DialogSettings,
          SIGNAL(triggered()),
          this,
          SLOT(dialogSettings_triggered()));

  action_FormTopSecret = new QAction(this);
  action_FormTopSecret->setText(tr("S&egurança"));
  action_FormTopSecret->setShortcut(QString(tr("Ctrl+E")));
  action_FormTopSecret->setStatusTip(tr("Segurança do Sistema"));
  connect(action_FormTopSecret,
          SIGNAL(triggered()),
          this,
          SLOT(formTopSecret_triggered()));

  action_About = new QAction(this);
  action_About->setText(tr("Sobre ..."));
  action_About->setStatusTip(tr("Sobre o DocBase"));
  connect(
    action_About, SIGNAL(triggered()), this, SLOT(action_About_triggered()));

  action_AboutQt = new QAction(this);
  action_AboutQt->setText(tr("Sobre Qt"));
  action_AboutQt->setStatusTip(tr("Informações sobre o Qt"));
  connect(action_AboutQt,
          SIGNAL(triggered()),
          this,
          SLOT(action_AboutQt_triggered()));
}

void
qDocBase::CreateActionsPrint(Globals::ModulesOfApp mod_)
{
  // Popup menus
  switch (mod_) {
    case Globals::ModulesOfApp::ModEntity: {
      actionPrintActiveEntities = new QAction(
        QIcon::fromTheme("document-print"), tr("Entidades Ativas"), this);
      actionPrintActiveEntities->setStatusTip(
        "Selecione a categoria que deseja imprimir.");
      actionPrintInactiveEntities = new QAction(
        QIcon::fromTheme("document-print"), tr("Entidades Inativas"), this);
      actionPrintInactiveEntities->setStatusTip(
        "Selecione a categoria que deseja imprimir.");
      break;
    }
    case Globals::ModulesOfApp::ModDocuments: {
      actionPrintOneImage = new QAction(QIcon::fromTheme("document-print"),
                                        tr("Imprime Documento Atual"),
                                        this);
      actionPrintOneImage->setShortcut(QString("Ctrl+P"));
      actionPrintImages = new QAction(QIcon::fromTheme("document-print"),
                                      tr("Imprime Documentos Selecionadas"),
                                      this);
      actionPrintListDocs = new QAction(
        QIcon::fromTheme("document-print"), tr("Listagem Geral"), this);
      actionPrintListDocs->setStatusTip(
        tr("Listagem do geral do cadastro de Entidades sem imagens"));
      actionPrintListValDocs = new QAction(
        QIcon::fromTheme("document-print"), tr("Listagem Analítica"), this);
      actionPrintListValDocs->setStatusTip(
        tr("Listagem do cadastro de Entidades para análise de valores"));
      break;
    }
    case Globals::ModulesOfApp::ModSearch: {
      actionPrintTable = new QAction(
        QIcon::fromTheme("document-print"), tr("Imprime Tabela Atual"), this);
      actionPrintTable = new QAction(QIcon::fromTheme("document-print"),
                                     tr("Imprissão completa da tabela"),
                                     this);

      actionPrintDoc = new QAction(QIcon::fromTheme("document-print"),
                                   tr("Imprime Documento Atual"),
                                   this);
      actionPrintTable = new QAction(QIcon::fromTheme("document-print"),
                                     tr("Imprime Documentos Selecionados"),
                                     this);
      break;
    }
    case Globals::ModulesOfApp::ModConfig:
    case Globals::ModulesOfApp::ModSecurity:
      break;
  }
}

/*!
 * \private
 * \brief qDocBase::CreateMenusPrint
 * \param mod_
 */
void
qDocBase::CreateMenusPrint(Globals::ModulesOfApp mod_)
{
  switch (mod_) {
    case Globals::ModulesOfApp::ModEntity: {
      popupPrintMenu->addAction(actionPrintActiveEntities);
      popupPrintMenu->addAction(actionPrintInactiveEntities);
      break;
    }
    case Globals::ModulesOfApp::ModDocuments: {
      popupPrintMenu->addAction(actionPrintOneImage);
      popupPrintMenu->addAction(actionPrintImages);
      popupPrintMenu->addSeparator();
      popupPrintMenu->addAction(actionPrintListDocs);
      popupPrintMenu->addAction(actionPrintListValDocs);
      break;
    }
    case Globals::ModulesOfApp::ModSearch: {
      popupPrintMenu->addAction(actionPrintDoc);
      popupPrintMenu->addAction(actionPrintTable);
      break;
    }
    case Globals::ModulesOfApp::ModConfig:
    case Globals::ModulesOfApp::ModSecurity:
      break;
  }
}

/*!
 * \brief qDocBase::CreateToolBars
 */

void
qDocBase::CreateToolBars(Globals::ModulesOfApp mod_)
{
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
  auto commons_ = [this, &mod_, &settings]() {
    toolButtonSave = new QToolButton;
    toolButtonSave->setIcon(QIcon::fromTheme("document-save"));
    toolButtonSave->setToolTip(tr("Grava o registro [F10]"));
    toolButtonSave->setShortcut(QKeySequence(Qt::Key_F10));
    toolButtonSave->setEnabled(!settings.value("readonly").toBool());

    toolButtonUpdate = new QToolButton;
    toolButtonUpdate->setIcon(QIcon::fromTheme("edit-redo"));
    toolButtonUpdate->setToolTip(tr("Grava as alterações [F9]"));
    toolButtonUpdate->setShortcut(QKeySequence(Qt::Key_F9));
    if (settings.value("readonly").toBool() ||
        settings.value("rosave").toBool()) {
      toolButtonUpdate->setEnabled(false);
    }

    toolButtonClear = new QToolButton;
    toolButtonClear->setIcon(QIcon::fromTheme("edit-clear"));
    toolButtonClear->setToolTip(tr("Limpa os campos\nna janela [F12]"));
    toolButtonClear->setShortcut(QKeySequence(Qt::Key_F12));

    toolButtonDelete = new QToolButton;
    toolButtonDelete->setIcon(QIcon::fromTheme("edit-delete"));
    toolButtonDelete->setToolTip(tr("Remove o registro\ndo banco de dados"));
    if (settings.value("readonly").toBool() ||
        settings.value("rosave").toBool()) {
      toolButtonDelete->setEnabled(false);
    }

    // -------------------------------------------------------------------------

    toolButtonExport = new QToolButton;
    toolButtonExport->setIcon(QIcon::fromTheme("document-save-as"));
    toolButtonExport->setToolTip(tr("Exportação ..."));
    toolButtonExport->setEnabled(true);
    if (mod_ == Globals::ModulesOfApp::ModEntity) {
      toolButtonExport->setEnabled(false);
      toolButtonExport->setVisible(false);
    }

    toolButtonImportBatch = new QToolButton;
    toolButtonImportBatch->setIcon(
      QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    toolButtonImportBatch->setToolTip(tr("Importação em Lotes ..."));
    toolButtonImportBatch->setEnabled(true);
    if (mod_ == Globals::ModulesOfApp::ModEntity) {
      toolButtonImportBatch->setEnabled(false);
      toolButtonImportBatch->setVisible(false);
    }

    // -------------------------------------------------------------------------
    toolBarEntities = addToolBar("Form");
    toolBarEntities->setFloatable(false);
    toolBarEntities->setMovable(false);

    toolBarEntities->addWidget(toolButtonSave);
    toolBarEntities->addWidget(toolButtonUpdate);
    toolBarEntities->addWidget(toolButtonClear);
    toolBarEntities->addWidget(toolButtonDelete);
    toolBarEntities->addSeparator();
    toolBarEntities->addWidget(toolButtonExport);
    toolBarEntities->addWidget(toolButtonImportBatch);
  };

  switch (mod_) {
    case Globals::ModulesOfApp::ModDocuments: {
      commons_();
      CreateActionsPrint(mod_);
      CreateMenusPrint(mod_);

      connect(toolButtonExport,
              SIGNAL(clicked()),
              formDocuments_,
              SLOT(toolButtonExport_clicked()));

      connect(toolButtonImportBatch,
              SIGNAL(clicked()),
              formDocuments_,
              SLOT(toolButtonImportBatch_clicked()));

      toolButtonPrintMenu = new QToolButton;
      toolButtonPrintMenu->setIcon(QIcon::fromTheme("document-print-preview"));
      toolButtonPrintMenu->setToolTip(tr("Impressão ..."));
      toolButtonPrintMenu->setMenu(popupPrintMenu);
      toolButtonPrintMenu->setPopupMode(QToolButton::InstantPopup);
      connect(toolButtonClear,
              SIGNAL(clicked()),
              formDocuments_,
              SLOT(clearRecord()));
      connect(formDocuments_,
              SIGNAL(deleteMenu(Globals::ModulesOfApp)),
              this,
              SLOT(deleteMenu(Globals::ModulesOfApp)));

      toolBarDateEditBegin = new QDateEdit;
      toolBarDateEditBegin->setToolTip(tr("Informe a data inicial"));
      toolBarDateEditBegin->setStatusTip(
        tr("Data inicial para seleção dos documentos."));
      toolBarDateEditBegin->setCalendarPopup(true);

      toolBarDateEditEnd = new QDateEdit;
      toolBarDateEditEnd->setToolTip(tr("Informe a data final"));
      toolBarDateEditEnd->setStatusTip(
        tr("Data final para seleção dos documentos."));
      toolBarDateEditEnd->setCalendarPopup(true);

      toolBarDateEditEnd->setDate(QDate::currentDate());
      connect(toolBarDateEditBegin,
              SIGNAL(dateChanged(QDate)),
              formDocuments_,
              SLOT(toolBarDateEditBegin_dateChanged(QDate)));
      connect(toolBarDateEditEnd,
              SIGNAL(dateChanged(QDate)),
              formDocuments_,
              SLOT(toolBarDateEditEnd_dateChanged(QDate)));

      toolBarCheckBoxDatePurpose = new QCheckBox;
      toolBarCheckBoxDatePurpose->setStatusTip(
        tr("Informa se deve selecionar os documentos "
           "pela data de cadastro ou do documento"));
      toolBarCheckBoxDatePurpose->setChecked(true);
      toolBarCheckBoxDatePurpose->setText(datePurpose_.registration_);
      connect(toolBarCheckBoxDatePurpose,
              SIGNAL(toggled(bool)),
              formDocuments_,
              SLOT(checkBoxDatePurpose_triggered(bool)));
      connect(toolBarCheckBoxDatePurpose, &QCheckBox::clicked, this, [this]() {
        (toolBarCheckBoxDatePurpose->isChecked())
          ? toolBarCheckBoxDatePurpose->setText(tr("Data do Cadastro"))
          : toolBarCheckBoxDatePurpose->setText(tr("Data do Documento"));
      });

      toolBarEntities->addWidget(toolButtonPrintMenu);
      toolBarEntities->addWidget(toolBarDateEditBegin);
      toolBarEntities->addWidget(toolBarDateEditEnd);
      toolBarEntities->addWidget(toolBarCheckBoxDatePurpose);
      break;
    }
    case Globals::ModulesOfApp::ModEntity: {
      commons_();
      CreateActionsPrint(mod_);
      CreateMenusPrint(mod_);

      toolButtonPrintMenu = new QToolButton;
      toolButtonPrintMenu->setIcon(QIcon::fromTheme("document-print-preview"));
      toolButtonPrintMenu->setToolTip(tr("Impressão ..."));
      toolButtonPrintMenu->setMenu(popupPrintMenu);
      toolButtonPrintMenu->setPopupMode(QToolButton::InstantPopup);

      connect(formEntities_,
              SIGNAL(deleteMenu(Globals::ModulesOfApp)),
              this,
              SLOT(deleteMenu(Globals::ModulesOfApp)));

      toolBarEntities->addWidget(toolButtonPrintMenu);
      break;
    }
    case Globals::ModulesOfApp::ModSearch: {
      CreateActionsPrint(mod_);
      CreateMenusPrint(mod_);

      toolButtonExec = new QToolButton;
      toolButtonExec->setIcon(QIcon::fromTheme("go-next"));
      toolButtonExec->setToolTip(tr("Executa a pesquisa [F10]"));
      toolButtonExec->setShortcut(QKeySequence(Qt::Key_F10));
      toolButtonExec->setEnabled(!settings.value("readonly").toBool());

      toolButtonClear = new QToolButton;
      toolButtonClear->setIcon(QIcon::fromTheme("edit-clear"));
      toolButtonClear->setToolTip(tr("Limpa os campos\nna janela [F12]"));
      toolButtonClear->setShortcut(QKeySequence(Qt::Key_F12));

      toolButtonExport = new QToolButton;
      toolButtonExport->setIcon(QIcon::fromTheme("document-save-as"));
      toolButtonExport->setToolTip(tr("Exportação ..."));
      toolButtonExport->setEnabled(true);

      toolButtonPrintMenu = new QToolButton;
      toolButtonPrintMenu->setIcon(QIcon::fromTheme("document-print-preview"));
      toolButtonPrintMenu->setToolTip(tr("Impressão ..."));
      toolButtonPrintMenu->setMenu(popupPrintMenu);
      toolButtonPrintMenu->setPopupMode(QToolButton::InstantPopup);

      toolButtonZoomIn = new QToolButton;
      toolButtonZoomIn->setIcon(QIcon::fromTheme("zoom-in"));
      toolButtonZoomIn->setToolTip(tr("Ampliar"));
      toolButtonZoomIn->setShortcut(QKeySequence(Qt::Key_ZoomIn));
      toolButtonZoomIn->setEnabled(!settings.value("readonly").toBool());

      toolButtonZoomOut = new QToolButton;
      toolButtonZoomOut->setIcon(QIcon::fromTheme("zoom-out"));
      toolButtonZoomOut->setToolTip(tr("Reduzir"));
      toolButtonZoomOut->setShortcut(QKeySequence(Qt::Key_ZoomOut));
      toolButtonZoomOut->setEnabled(!settings.value("readonly").toBool());

      toolBarEntities = addToolBar("Form");
      toolBarEntities->setFloatable(false);
      toolBarEntities->setMovable(false);

      connect(formSearch_,
              SIGNAL(deleteMenu(Globals::ModulesOfApp)),
              this,
              SLOT(deleteMenu(Globals::ModulesOfApp)));

      toolBarEntities->addWidget(toolButtonExec);
      toolBarEntities->addWidget(toolButtonClear);
      toolBarEntities->addSeparator();
      toolBarEntities->addWidget(toolButtonExport);
      toolBarEntities->addWidget(toolButtonPrintMenu);
      toolBarEntities->addSeparator();
      toolBarEntities->addWidget(toolButtonZoomIn);
      toolBarEntities->addWidget(toolButtonZoomOut);
      break;
    }
    case Globals::ModulesOfApp::ModConfig:
    case Globals::ModulesOfApp::ModSecurity:
      break;
  }
}

/*!
 * \brief qDocBase::action_Entities_triggered
 */
void
qDocBase::action_Entities_triggered()
{
}

/*!
 * \public SLOT
 * \brief qDocBase::action_Search_triggered
 */
void
qDocBase::action_Search_triggered()
{
}

/*!
 * \public SLOT
 * \brief qDocBase::actionAbout_triggered
 */
void
qDocBase::action_About_triggered()
{
  dialogAbout = new DialogAbout;
  dialogAbout->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
  dialogAbout->show();
}

/*!
 * \brief qDocBase::action_AboutQt_triggered
 */
void
qDocBase::action_AboutQt_triggered()
{
  QMessageBox::aboutQt(this, "About Qt");
}

/*!
 * \brief qDocBase::action_Exit_triggered
 */
void
qDocBase::action_Exit_triggered()
{
  close();
}

/*!
 * \brief qDocBase::dialogSettings_triggered
 */
void
qDocBase::dialogSettings_triggered()
{
  dlgSettings = new DialogSettings;
  dlgSettings->setModal(true);
  dlgSettings->show();
}

/*!
 * \brief qDocBase::formTopSecret_triggered
 */
void
qDocBase::formTopSecret_triggered()
{
  formTopSecret = new FormTopSecret2;
  formTopSecret->show();
}

/*!
 * \brief qDocBase::formDocuments_triggered
 */
void
qDocBase::formDocuments_triggered()
{
  formDocuments_ = new FormDocuments;
  formDocuments_->show();
}

/*!
 * \brief qDocBase::formSearch_triggered
 */
void
qDocBase::formSearch_triggered()
{
  formSearch_ = new FormSearch;
  formSearch_->show();
}

/*!
 * \brief qDocBase::modules_listView_doubleClicked
 * \param idx
 */
void
qDocBase::modules_listView_doubleClicked(QModelIndex idx)
{
  Globals::ModulesOfApp modules_ =
    static_cast<Globals::ModulesOfApp>(idx.data(Qt::UserRole).toInt());

  ui->modules_dockWidget->setHidden(true);

  switch (modules_) {
    case Globals::ModulesOfApp::ModEntity: {
      formEntities_ = new FormEntities;
      OpenWindow(formEntities_, Globals::ModulesOfApp::ModEntity);
      break;
    }
    case Globals::ModulesOfApp::ModDocuments: {
      formDocuments_ = new FormDocuments;
      OpenWindow(formDocuments_, Globals::ModulesOfApp::ModDocuments);
      break;
    }
    case Globals::ModulesOfApp::ModSearch: {
      formSearch_ = new FormSearch;
      OpenWindow(formSearch_, Globals::ModulesOfApp::ModSearch);
      break;
    }
    case Globals::ModulesOfApp::ModConfig: {
      break;
    }
    case Globals::ModulesOfApp::ModSecurity: {

      break;
    }
  }
}

/*!
 * \public SLOT
 * \brief qDocBase::deleteToolBarEntities
 * \param b_
 */
void
qDocBase::deleteToolBarEntities(bool b_)
{
  if (b_) {
    toolBarEntities->hide();
  }
}

/*!
 * \public SLOT
 * \brief qDocBase::deleteMenu
 * \param module_
 */
void
qDocBase::deleteMenu(Globals::ModulesOfApp module_)
{
  switch (module_) {
    case Globals::ModulesOfApp::ModEntity: {
      popupPrintMenu->removeAction(actionPrintActiveEntities);
      popupPrintMenu->removeAction(actionPrintInactiveEntities);
      break;
    }
    case Globals::ModulesOfApp::ModDocuments: {
      popupPrintMenu->removeAction(actionPrintOneImage);
      popupPrintMenu->removeAction(actionPrintImages);
      popupPrintMenu->removeAction(actionPrintListDocs);
      popupPrintMenu->removeAction(actionPrintListValDocs);
      break;
    }
    case Globals::ModulesOfApp::ModSearch: {
      popupPrintMenu->removeAction(actionPrintTable);
      popupPrintMenu->removeAction(actionPrintDoc);
      break;
    }
    case Globals::ModulesOfApp::ModConfig:
    case Globals::ModulesOfApp::ModSecurity:
      break;
  }
}

/*!
 * \public SLOT
 * \brief qDocBase::showStatusMessage
 * \param message_
 */
void
qDocBase::showStatusMessage(const QString& message_)
{
  statusBar()->showMessage(message_);
}

/*!
 * \brief Handles Mainwindow closing events. Takes all the buttons that close
 * the window.
 * \param event
 */
void
qDocBase::closeEvent(QCloseEvent* event)
{
  if (!isActiveWindows()) {
    event->accept();
    sqlDb_.close();
    close();
  } else
    event->ignore();
}

/*!
 * \brief qDocBase::showEvent
 * \param e
 */
void
qDocBase::showEvent(QShowEvent* e)
{
  e->accept();
}

/*!
 * \brief Auxiliary functions
 *
 */

/*!
 * \brief qDocBase::OpenWindow
 * \param parent_
 * \param module_
 */
void
qDocBase::OpenWindow(QWidget* parent_, Globals::ModulesOfApp mod_)
{

  switch (mod_) {
    case Globals::ModulesOfApp::ModEntity: {
      if (!isActiveWindows(tr("Entidades"))) {
        CreateToolBars(Globals::ModulesOfApp::ModEntity);
        /*!
         * \brief RecEntity, when closed by the user, emits a signal informing
         * the toolbar that it must be put in invisible mode. the toolbar should
         * only be visible when it is necessary to use it.
         */
        connect(parent_,
                SIGNAL(closed(bool)),
                toolBarEntities,
                SLOT(setVisible(bool)));

        connect(
          toolButtonSave, SIGNAL(clicked()), formEntities_, SLOT(saveRecord()));
        connect(toolButtonUpdate,
                SIGNAL(clicked()),
                formEntities_,
                SLOT(updateRecord()));
        connect(toolButtonDelete,
                SIGNAL(clicked()),
                formEntities_,
                SLOT(deleteRecord()));
        connect(toolButtonClear,
                SIGNAL(clicked()),
                formEntities_,
                SLOT(clearRecord()));

        connect(actionPrintActiveEntities,
                SIGNAL(triggered(bool)),
                formEntities_,
                SLOT(printActionEntities_triggered(bool)));
        connect(actionPrintInactiveEntities,
                SIGNAL(triggered(bool)),
                formEntities_,
                SLOT(printInactiveEntities_triggered(bool)));

        QMdiSubWindow* subWin_ = new QMdiSubWindow;
        subWin_->setWidget(parent_);
        subWin_->setAttribute(Qt::WA_DeleteOnClose);
        ui->main_mdiArea->addSubWindow(subWin_);
        subWin_->showMaximized();
      }
      break;
    }
    case Globals::ModulesOfApp::ModDocuments: {
      if (!isActiveWindows(tr("Documentos"))) {
        CreateToolBars(Globals::ModulesOfApp::ModDocuments);

        connect(parent_,
                SIGNAL(showStatusMessage(const QString&)),
                this,
                SLOT(showStatusMessage(const QString&)));

        connect(parent_,
                SIGNAL(closed(bool)),
                toolBarEntities,
                SLOT(setVisible(bool)));

        connect(toolButtonSave,
                SIGNAL(clicked()),
                formDocuments_,
                SLOT(saveRecord()));
        connect(toolButtonUpdate,
                SIGNAL(clicked()),
                formDocuments_,
                SLOT(updateRecord()));
        connect(toolButtonDelete,
                SIGNAL(clicked()),
                formDocuments_,
                SLOT(deleteRecord()));
        connect(toolButtonClear,
                SIGNAL(clicked()),
                formDocuments_,
                SLOT(clearRecord()));

        connect(actionPrintOneImage,
                SIGNAL(triggered()),
                formDocuments_,
                SLOT(printOneImage_triggered()));
        connect(actionPrintImages,
                SIGNAL(triggered()),
                formDocuments_,
                SLOT(printImages_triggered()));
        connect(actionPrintListDocs,
                SIGNAL(triggered()),
                formDocuments_,
                SLOT(printListDocs_triggered()));
        connect(actionPrintListValDocs,
                SIGNAL(triggered()),
                formDocuments_,
                SLOT(printListValDocs_triggered()));

        QMdiSubWindow* subWin_ = new QMdiSubWindow;
        subWin_->setWidget(parent_);
        subWin_->setAttribute(Qt::WA_DeleteOnClose);
        ui->main_mdiArea->addSubWindow(subWin_);
        subWin_->showMaximized();
      }
      break;
    }
    case Globals::ModulesOfApp::ModSearch: {
      if (!isActiveWindows(tr("Pesquisa")) || !isActiveWindows(tr("Search"))) {
        CreateToolBars(mod_);

        connect(parent_,
                SIGNAL(closed(bool)),
                toolBarEntities,
                SLOT(setVisible(bool)));

        connect(parent_,
                SIGNAL(showStatusMessage(const QString&)),
                this,
                SLOT(showStatusMessage(const QString&)));

        connect(
          toolButtonExec, SIGNAL(clicked()), formSearch_, SLOT(execSearch()));

        connect(toolButtonExport,
                SIGNAL(clicked()),
                formSearch_,
                SLOT(exportDocuments_clicked()));

        connect(toolButtonClear,
                SIGNAL(clicked()),
                formSearch_,
                SLOT(clearFields_clicked()));

        connect(actionPrintDoc,
                SIGNAL(triggered()),
                formSearch_,
                SLOT(printCurrentDoc_triggered()));

        connect(actionPrintTable,
                SIGNAL(triggered()),
                formSearch_,
                SLOT(printDocsList_triggered()));

        connect(toolButtonZoomIn,
                SIGNAL(clicked()),
                formSearch_,
                SLOT(zoomIn_clicked()));

        connect(toolButtonZoomOut,
                SIGNAL(clicked()),
                formSearch_,
                SLOT(zoomOut_clicked()));

        QMdiSubWindow* subWin_ = new QMdiSubWindow;
        subWin_->setWidget(parent_);
        subWin_->setAttribute(Qt::WA_DeleteOnClose);
        ui->main_mdiArea->addSubWindow(subWin_);
        subWin_->showMaximized();
      }
    }
    default:
      break;
  }
}

/*!
 * \protected
 * \brief Checks for open windows. Returns true if yes and false if not
 * @return true|false
 */
bool
qDocBase::isActiveWindows(const QString&& title_)
{
  if (title_.contains(title_)) {
    if (ui->main_mdiArea->activeSubWindow()) { // ha janelas abertas
      QMessageBox::warning(this,
                           tr(ProgId::Name),
                           tr("Para a segurança das suas informações "
                              "\nnão é permitido encerrar o programa "
                              "\nsem que todas as janelas estejam fechadas."),
                           QMessageBox::Close);
      return true;
    }
  }
  return false;
}
