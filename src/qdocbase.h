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
#ifndef QDOCBASE_H
#define QDOCBASE_H

#include <QMainWindow>
#include <QMessageBox>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDebug>
#include <QIcon>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QModelIndex>
#include <QSettings>
#include <QShowEvent>
#include <QString>
#include <QStyle>
#include <QToolBar>
#include <QToolButton>

#include "dialogabout.h"
#include "dialogsettings.h"
#include "formdocuments.h"
#include "formentities.h"
#include "formsearch.h"
#include "formtopsecret2.h"

#include "globals.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class qDocBase;
}
QT_END_NAMESPACE

/*!
 * \brief The qDocBase class
 * Main class
 */
class qDocBase : public QMainWindow
{
  Q_OBJECT

public:
  qDocBase(QWidget* parent = nullptr);
  ~qDocBase();

public:
  QSqlDatabase sqlDb_;
  QSqlError sqlError_;

public:
  FormEntities* formEntities_;
  FormDocuments* formDocuments_;
  FormSearch* formSearch_;

public slots:
  void action_Entities_triggered();
  void action_Search_triggered();

  void action_About_triggered();
  void action_AboutQt_triggered();

  void action_Exit_triggered();

  void dialogSettings_triggered();
  void formTopSecret_triggered();
  void formDocuments_triggered();
  void formSearch_triggered();

  virtual void modules_listView_doubleClicked(QModelIndex);

  void deleteToolBarEntities(bool);

  void deleteMenu(Globals::ModulesOfApp);

  void showStatusMessage(const QString& message_);

private:
  Ui::qDocBase* ui;

  DialogSettings* dlgSettings;
  FormTopSecret2* formTopSecret;
  DialogAbout* dialogAbout;

private:
  Globals globals_;

  QAction* action_Modules;
  QAction* action_Tools;
  QAction* action_About;
  QAction* action_AboutQt;
  QAction* action_Exit;

  QAction* action_DialogSettings;
  QAction* action_FormTopSecret;

  QAction* action_export;
  QAction* actionPrintOneImage;
  QAction* actionPrintImages;
  QAction* actionPrintListDocs;
  QAction* actionPrintListValDocs;

  QAction* actionExportOneImage;
  QAction* actionExportImages;

  QAction* actionPrintTable;
  QAction* actionPrintDoc;

  QAction* actionPrintActiveEntities;
  QAction* actionPrintInactiveEntities;

  QToolBar* toolBarEntities;
  QToolBar* toolBarDocuments;

  QToolButton* toolButtonExec;
  QToolButton* toolButtonSave;
  QToolButton* toolButtonUpdate;
  QToolButton* toolButtonClear;
  QToolButton* toolButtonDelete;

  QToolButton* toolButtonImportBatch;
  QToolButton* toolButtonExport;
  QToolButton* toolButtonPrint;
  QToolButton* toolButtonPrintMenu;

  QDateEdit* toolBarDateEditBegin;
  QDateEdit* toolBarDateEditEnd;
  QCheckBox* toolBarCheckBoxDatePurpose;
  const struct DatePurpose
  {
    const QString registration_;
    const QString document_;
  } datePurpose_;

  QToolButton* toolButtonZoomIn;
  QToolButton* toolButtonZoomOut;
  QToolButton* toolButtonOriginal;

  QMenu* popupPrintMenu;
  QMenu* popupExportMenu;

  void CreateActions();
  void CreateActionsPrint(Globals::ModulesOfApp mod_);

  void CreateMenus();
  void CreateMenusPrint(Globals::ModulesOfApp mod_);

  void OpenWindow(QWidget* parent_, Globals::ModulesOfApp modu_);
  void CreateToolBars(Globals::ModulesOfApp mod_);

protected:
  bool isActiveWindows(const QString&& title_ = QString());
  void closeEvent(QCloseEvent* event);
  void showEvent(QShowEvent* e);

signals:
  void printActiveEntities(bool);
  void printInactiveEntities(bool);
};
#endif // QDOCBASE_H
