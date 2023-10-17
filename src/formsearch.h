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
#ifndef FORMSEARCH_H
#define FORMSEARCH_H

#include <QApplication>
#include <QBitArray>
#include <QByteArray>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QDateEdit>
#include <QDesktopServices>
#include <QFocusEvent>
#include <QFuture>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QObject>
#include <QPixmap>
#include <QPrinter>
#include <QWidget>
#include <QtConcurrent>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>

#include "globals.h"
#include "messages.h"
#include "temporaryfiles.h"

namespace Ui {
class FormSearch;
}

/*!
 * \brief The FormSearch class
 */
class FormSearch : public QWidget
{
  Q_OBJECT
public:
  explicit FormSearch(QWidget* parent = nullptr);
  ~FormSearch();

private:
  Ui::FormSearch* ui;

private:
  QSqlQueryModel* qryModel_;
  QGraphicsScene* scene_;

  TemporaryFiles* tempFiles_;
  MessagesNS::Messages* Messages_;

  struct Identifications_t
  {
    int Categorie;
    quint64 BaseId;
    QString mainEntity_;
    QString linkedEntity_;
  } Ident_t_;

  static constexpr const char* SORT_01 = QT_TR_NOOP("#ID");
  static constexpr const char* SORT_02 = QT_TR_NOOP("Nome");
  static constexpr const char* SORT_03 = QT_TR_NOOP("Operador");
  static constexpr const char* SORT_04 = QT_TR_NOOP("Data do Cadastro");
  static constexpr const char* SORT_05 = QT_TR_NOOP("Data do Documento");
  static constexpr const char* SORT_06 = QT_TR_NOOP("Departamento");

  void fillComboBoxDeparts();
  void fillSortingFields();
  void fillCustomFields();

  inline void showTitle(const QString& mainEntity_ = QString());

protected:
  virtual void closeEvent(QCloseEvent* event_) override;

public slots:
  virtual void treeView_BaseEntities_clicked(QModelIndex index_);
  virtual void clearFields_clicked();
  virtual void toolButtonExport_clicked();
  virtual void checkBox_AscOrDesc_clicked(bool status_);
  virtual void checkBox_DateType_clicked(bool status_);
  virtual void tableView_Results_clicked(QModelIndex index_);

  void execSearch();
  void zoomIn_clicked();
  void zoomOut_clicked();
  void printCurrentDoc_triggered();
  void printDocsList_triggered();

  void urlError(const QString e_);

  virtual void exportDocuments_clicked();

signals:
  void open(bool);
  void closed(bool);
  void deleteMenu(Globals::ModulesOfApp);
  void reloadDepartments_sig(bool);

  void showStatusMessage(const QString&);
};

/*!
 * \class
 * \brief The SqlBuilder class
 */
class SqlBuilder : public QObject
{
  Q_OBJECT
public:
  explicit SqlBuilder(const quint64 base_id_ = 0);
  ~SqlBuilder();

  SqlBuilder& setSorting(QGroupBox* gb_ = nullptr,
                         QComboBox* cb_ = nullptr,
                         QCheckBox* asc_or_desc_ = nullptr);
  SqlBuilder& setDepartments(QGroupBox* gb_ = nullptr,
                             QComboBox* cb_ = nullptr);

  SqlBuilder& setDateRange(QGroupBox* gb_ = nullptr,
                           QDateEdit* deFrom_ = nullptr,
                           QDateEdit* deTo_ = nullptr,
                           QCheckBox* datetype_ = nullptr);

  SqlBuilder& setUnLock(QLineEdit* user_ = nullptr,
                        QLineEdit* passwd_ = nullptr);

  SqlBuilder& setHistorical(QGroupBox* gb_ = nullptr,
                            QLineEdit* hist_ = nullptr,
                            QCheckBox* equal_ = nullptr);

  SqlBuilder& setCustomField(QGroupBox* gb_ = nullptr,
                             QComboBox* fldname_ = nullptr,
                             QLineEdit* flddata_ = nullptr,
                             QCheckBox* equal_ = nullptr);

  bool isUnlocked() const;

  const QString result();

private:
  quint64 baseId_;
  QString stmnt_;
  QString order_by_;

  bool unlocked_;

  QMultiMap<const int, const QString> sortFields_m_;

  // The # character is used as an internal marker.
  constexpr static const char* select_base_ =
    "SELECT A.ST_IDENT, A.ST_RECDATE, A.ST_OPERATOR, A.ST_DEPART, "
    "(SELECT C.ET_NOME FROM DOCENTITY C WHERE "
    "C.ET_IDENT=A.ST_ETRELA) AS ET_NOME, "
    "A.ST_DOCDATE, A.ST_NUMBER, FORMAT(A.ST_VALUE,2) AS ST_VALUE, A.ST_FILE # "
    "FROM DOCSTORAGE A "
    "WHERE "
    "A.ET_IDENT=%0";
};

/*!
 * \brief The ItemInterceptDoubleClick class
 * Intercept double-click on the data table
 *
 */
class ItemInterceptDoubleClick
  : public QObject
  , public QGraphicsPixmapItem
{
  Q_OBJECT
public:
  ItemInterceptDoubleClick();

  void setProperty(const qint64 ident_ = 0,
                   const QString fileName_ = QString());

private:
  QString fname_{};
  qint64 id_{};

  MessagesNS::Messages* Messages_;

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

public slots:
  void internalError(const QString err_);

signals:
  void openUrlError(const QString);
  void fileError(const QString);
  void queryError(const QString);
};

#endif // FORMSEARCH_H
