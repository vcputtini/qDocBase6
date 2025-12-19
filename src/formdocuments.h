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
#ifndef FORMDOCUMENTS_H
#define FORMDOCUMENTS_H

#include <QApplication>
#include <QCloseEvent>
#include <QObject>
#include <QPrinter>
#include <QWidget>

#include <QAbstractButton>
#include <QBitArray>
#include <QByteArray>
#include <QComboBox>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFuture>
#include <QFutureWatcher>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QIcon>
#include <QImage>
#include <QImageReader>
#include <QInputDialog>
#include <QItemSelectionModel>
#include <QKeyEvent>
#include <QLineEdit>
#include <QList>
#include <QListView>
#include <QMessageBox>
#include <QModelIndex>
#include <QPainter>
#include <QPair>
#include <QPixmap>
#include <QProgressDialog>
#include <QPushButton>
#include <QSettings>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QtConcurrent/QtConcurrentMap>
#include <QtConcurrent/QtConcurrentRun>

// #include <QtPdfWidgets/QPdfView>

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>

#include "globals.h"
#include "messages.h"
#include "settings.h"
#include "typeimage.h"

namespace Ui {
class FormDocuments;
}

/*!
 * \class FormDocuments
 * \brief The FormDocuments class
 */
class FormDocuments : public QWidget
{
  Q_OBJECT
public:
  explicit FormDocuments(QWidget* parent = nullptr);
  ~FormDocuments();

private:
  Ui::FormDocuments* ui;

private:
  MessagesNS::Messages* Messages_;

  static constexpr const char* insert_doc_stmnt_ =
    "INSERT INTO DOCSTORAGE "
    "(ST_RECDATE, ST_OPERATOR, ET_IDENT, ST_ETRELA, "
    "ST_DOCDATE, ST_NUMBER, ST_VALUE, "
    "ST_DOCTYPE, ST_DOCIDENT, ST_DEPART,"
    "ST_FILE, ST_FILETYPE, ST_REMARK, ST_TOPSEC, "
    "ST_FIELD01DATA, ST_FIELD02DATA, ST_FIELD03DATA, ST_FIELD04DATA, "
    "ST_FIELD05DATA) VALUES (:ST_RECDATE, "
    ":ST_OPERATOR, :ET_IDENT, :ST_ETRELA, :ST_DOCDATE, :ST_NUMBER, "
    ":ST_VALUE, :ST_DOCTYPE, :ST_DOCIDENT, :ST_DEPART, :ST_FILE, "
    ":ST_FILETYPE, :ST_REMARK, :ST_TOPSEC, "
    ":ST_FIELD01DATA, :ST_FIELD02DATA)";

  static constexpr const char* stmnt_update_ =
    "UPDATE DOCSTORAGE SET "
    "ST_RECDATE=:ST_RECDATE, ST_OPERATOR=:ST_OPERATOR,"
    "ST_ETRELA=:ST_ETRELA, ST_DOCDATE=:ST_DOCDATE, ST_NUMBER=:ST_NUMBER,"
    "ST_VALUE=:ST_VALUE, ST_DOCTYPE=:ST_DOCTYPE, ST_DOCIDENT=:ST_DOCIDENT,"
    "ST_DEPART=:ST_DEPART,ST_FILE=:ST_FILE,"
    "ST_FILETYPE=:ST_FILETYPE, ST_REMARK=:ST_REMARK, ST_TOPSEC=:ST_TOPSEC,"
    "ST_FIELD01DATA=:ST_FIELD01DATA, ST_FIELD02DATA=:ST_FIELD02DATA "
    "WHERE ST_IDENT=:ST_IDENT";

  static constexpr const char* insert_img_stmnt_ =
    "INSERT INTO DOCIMAGES (ST_IDENT, IM_IMAGE) "
    "VALUES (:ST_IDENT, :IM_IMAGE)";

  struct FlagControls
  {
    bool FirstRun;
    bool Identify;
    bool Exportable;
    bool ImageSelected;
    bool IsViewable;
  } flagControls{ false };

  struct CacheIdentifications
  {
    int Categorie;
    quint64 BaseId;
    quint64 StorageId;
    quint64 DocId;
    QString mainEntity_;
    QString linkedEntity_;
  } Ident_t_;

  Settings settings_;

  QDate dateBegin_;
  QDate dateEnd_;
  bool datePurpose_;

  QString newFileName_;

  TypeImage typeImage_;
  QGraphicsScene* scene_;

  QString fullFileName_;
  QString filePicName_;
  int filePicSize_;

  typedef QFutureWatcher<QPair<QImage, QString>> imagesWatcher_;
  QStandardItemModel* modelThumbs_;
  QImage image_;
  QPainter paint_;

  QProgressDialog progress_;

  struct Record
  {
    quint64 st_ident;
    QString st_regdate;
    // int st_type;
    QString st_operator;
    quint64 et_ident;
    quint64 st_etrela;
    QString st_docdate;
    int st_number;
    double st_value;
    int st_doctype;
    QString st_docident;
    QString st_depart;
    QString st_file;
    bool st_filetype;
    QString st_remark;
    bool st_topsec;
    QString st_field01data;
    QString st_field02data;
    QString st_field03data;
    QString st_field04data;
    QString st_field05data;

    QByteArray im_image;
  } recDoc{};

  void loadListThumbs(const QString text_ = QString());

  inline void showDataFieldsOnForm();
  inline void showTitle(const QString mainEntity_ = QString(),
                        const QString linkedEntity_ = QString());

protected:
  virtual void closeEvent(QCloseEvent* event_) override;

public slots:
  virtual void comboBox_DocType_currentIndexChanged(int index_);
  virtual void comboBox_Files_currentTextChanged(QString text_);
  virtual void toolButton_FindFiles_clicked();
  virtual void toolButton_FilesReload_clicked();

  virtual void listView_Files_clicked(QModelIndex index_);
  virtual void listView_Files_doubleClicked(QModelIndex index_);
  virtual void listView_Files_entered(QModelIndex index_);
  virtual void treeView_BaseEntities_clicked(QModelIndex index_);
  virtual void treeView_StorageEntities_clicked(QModelIndex index_);

  virtual void toolBarDateEditBegin_dateChanged(QDate date_);
  virtual void toolBarDateEditEnd_dateChanged(QDate date_);
  virtual void checkBoxDatePurpose_triggered(bool state_);

  virtual void tableView_Documents_doubleClicked(const QModelIndex& index_);

  virtual void iDialog_finished(int);
  virtual void iDialog_textValueSelected(const QString& passwd_);

  void saveRecord();
  void updateRecord();
  void deleteRecord();
  void clearRecord(bool clearTitle_ = false);

  void saveDepartments_slot();
  void reloadDepartments_slot(bool);
  void deleteDepartments_slot();

  virtual void toolButtonExport_clicked();
  virtual void toolButtonImportBatch_clicked();
  virtual void printOneImage_triggered();
  virtual void printImages_triggered();
  virtual void printListDocs_triggered();
  virtual void printListValDocs_triggered();

  void multiFilesErrorText(const QString& errText_);

  void inputDialog_operatorSelected(const QString& text_);

  void errorExportFiles(const QString& errText_);

  // ImportBatch()
  void sqlErrorImportBatch(const QString& errText_);
  void progressBar(const int& value_);

signals:
  void open(bool);
  void closed(bool);
  void deleteMenu(Globals::ModulesOfApp);
  void reloadDepartments_sig(bool);
  void showStatusMessage(const QString&);
};

#endif // FORMDOCUMENTS_H
