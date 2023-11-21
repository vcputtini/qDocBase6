/***************************************************************************
 * Copyright (c) 2007-2023                                                 *
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
#ifndef FORMIMPORTBATCH_H
#define FORMIMPORTBATCH_H

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QModelIndexList>
#include <QProgressDialog>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

#include <array>

#include "formlogimportbatch.h"
#include "messages.h"

namespace Ui {
class FormImportBatch;
}

/*!
 * \brief The FormImportBatch class
 */
class FormImportBatch : public QWidget
{
  Q_OBJECT

public:
  explicit FormImportBatch(const quint64 baseId_ = 0,
                           const quint64 storageId_ = 0,
                           const QString oper_ = QString(),
                           const QString depart_ = QString(),
                           QWidget* parent = nullptr);
  ~FormImportBatch();

  void addItems(QModelIndexList mdlIndexList_);

private:
  Ui::FormImportBatch* ui;

private:
  MessagesNS::Messages* Messages_;

  QStringList strListLog_;

  QMainWindow* mainWindowToolBar;
  QToolBar* toolBarImport;
  QToolButton* toolButtonExec;
  QToolButton* toolButtonLog;

  quint64 baseID_;
  quint64 storageID_;
  QString department_;
  QString operator_;

  void createToolBar();
  void setupTable();

  void execImport();
  void viewLog();

public slots:
  void sqlErrorImportBatch(const QString& errText_);
  void progressBar(const int& value_); // ImportBatch

signals:

  void errorText(const QString& function_,
                 const int& line_,
                 const QString& text_);
};

#endif // FORMIMPORTBATCH_H
