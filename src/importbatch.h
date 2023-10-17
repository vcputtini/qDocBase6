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
#ifndef IMPORTBATCH_H
#define IMPORTBATCH_H

#include <QApplication>
#include <QDate>
#include <QFileInfo>
#include <QItemSelectionModel>
#include <QMap>
#include <QMessageBox>
#include <QModelIndexList>
#include <QObject>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include <array>

#include "messages.h"
#include "settings.h"

namespace Ui {
class ImportBatch;
}

/*!
 * \brief The ImportBatch class
 */
class ImportBatch : public QObject
{
  Q_OBJECT
public:
  explicit ImportBatch(QMap<int, std::array<QString, 5>> tab_m_,
                       QObject* parent = nullptr);
  explicit ImportBatch(QModelIndexList list_, QObject* parent = nullptr);
  ~ImportBatch();

  void setID(const quint64 baseId_,
             const quint64 storageId_,
             const QString oper_,
             const QString depart_) noexcept;

  void exec();

private:
  MessagesNS::Messages* Messages_;

  QMap<int, std::array<QString, 5>> table_m_;
  QModelIndexList indexList_;

  QTableWidget* table_;
  QStandardItemModel* modelThumbs_;

  Settings settings_;

  quint64 baseID_;
  quint64 storageID_;
  QString operator_;
  QString department_;

  void save();

  static constexpr const char* stmnt_insert_ =
    "INSERT INTO DOCSTORAGE "
    "(ST_OPERATOR, ET_IDENT, ST_ETRELA, "
    "ST_DOCDATE, ST_NUMBER, ST_VALUE, "
    "ST_DOCIDENT, ST_DEPART,"
    "ST_FILE, ST_FILETYPE, ST_REMARK, ST_TOPSEC, "
    "ST_FIELD01DATA, ST_FIELD02DATA, ST_FIELD03DATA, ST_FIELD04DATA, "
    "ST_FIELD05DATA) VALUES ("
    ":ST_OPERATOR, :ET_IDENT, :ST_ETRELA, :ST_DOCDATE, :ST_NUMBER, "
    ":ST_VALUE, :ST_DOCIDENT, :ST_DEPART, :ST_FILE, "
    ":ST_FILETYPE, "
    ":ST_REMARK, :ST_TOPSEC, "
    ":ST_FIELD01DATA, :ST_FIELD02DATA, :ST_FIELD03DATA, :ST_FIELD04DATA, "
    ":ST_FIELD05DATA)";

  static constexpr const char* insert_img_stmnt_ =
    "INSERT INTO DOCIMAGES (ST_IDENT, IM_IMAGE) "
    "VALUES (:ST_IDENT, :IM_IMAGE)";

public slots:
  void multiFilesErrorText(const QString&);

signals:
  void sqlError(const QString& text_);
  void progressBar(const int& value_);
};

#endif // IMPORTBATCH_H
