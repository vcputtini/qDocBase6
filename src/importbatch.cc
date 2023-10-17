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
#include "importbatch.h"
#include "globals.h"
#include "multifiles.h"

// #include "progid.h"
// #include "qtablewidget.h"

/*!
 * \brief ImportBatch::ImportBatch
 * \param tab_m_
 * \param parent
 */
ImportBatch::ImportBatch(QMap<int, std::array<QString, 5>> tab_m_,
                         QObject* parent)
  : QObject{ parent }
{
  table_m_ = std::move(tab_m_);
  Messages_ = &Messages_->instance();
}

/*!
 * \brief ImportBatch::ImportBatch
 * \param list_
 * \param parent
 */
ImportBatch::ImportBatch(QModelIndexList list_, QObject* parent)
  : QObject{ parent }
{
  indexList_ = std::move(list_);
  Messages_ = &Messages_->instance();
}

/*!
 * \brief ImportBatch::~ImportBatch
 */
ImportBatch::~ImportBatch() {}

/*!
 * \brief ImportBatch::setID
 * \param baseId_
 * \param storageId_
 * \param depart_
 */
void
ImportBatch::setID(const quint64 baseId_,
                   const quint64 storageId_,
                   const QString oper_,
                   const QString depart_) noexcept
{
  baseID_ = std::move(baseId_);
  storageID_ = std::move(storageId_);
  operator_ = std::move(oper_);
  department_ = std::move(depart_);
}

/*!
 * \public
 * \brief ImportBatch::exec
 */
void
ImportBatch::exec()
{
  save();
}

/*!
 * \private
 * \brief ImportBatch::save
 */
void
ImportBatch::save()
{
  auto procSql = [this](int counter_,
                        const QString fname_,
                        const QString internalFileNamePattern_,
                        const QString number_,
                        const QString date_,
                        QString value_,
                        const QString iddoc_) {
    QSqlDatabase::database().transaction();
    QSqlQuery qry_doc_;
    qry_doc_.prepare(stmnt_insert_);
    qry_doc_.bindValue(":ST_OPERATOR", std::move(operator_));
    qry_doc_.bindValue(":ET_IDENT", std::move(baseID_));
    qry_doc_.bindValue(":ST_ETRELA", std::move(storageID_));

    switch (settings_.getDbNativeDateFormat()) {
      case Globals::SupportedDateFormats::ISO: {
        if (Globals::is_ptBR()) {
          qry_doc_.bindValue(":ST_DOCDATE",
                             std::move(Globals::toISODate(date_)));
        } else {
          qry_doc_.bindValue(":ST_DOCDATE", std::move(date_));
        }
        break;
      }
      case Globals::SupportedDateFormats::EUA: {
        if (Globals::is_ptBR()) {
          qry_doc_.bindValue(":ST_DOCDATE",
                             std::move(Globals::toEUADate(date_)));
        } else {
          qry_doc_.bindValue(":ST_DOCDATE", std::move(date_));
        }
        break;
      }
      case Globals::SupportedDateFormats::UK: {
        qry_doc_.bindValue(":ST_DOCDATE", std::move(date_));
      }
    }

    qry_doc_.bindValue(":ST_NUMBER", std::move(number_));
    QLocale loc_;
    bool ok{};
    loc_.toFloat(value_, &ok);
    if (ok) {
      qry_doc_.bindValue(":ST_VALUE", std::move(loc_.toFloat(value_)));
    } else {
      qry_doc_.bindValue(":ST_VALUE", 0.0);
    }
    qry_doc_.bindValue(":ST_DOCIDENT", std::move(iddoc_)); // ST_DOCIDENT
    qry_doc_.bindValue(":ST_DEPART", std::move(department_));
    qry_doc_.bindValue(
      ":ST_FILE",
      std::move(Globals::normalizeFileName(internalFileNamePattern_)));
    qry_doc_.bindValue(":ST_FILETYPE", true);
    qry_doc_.bindValue(":ST_REMARK", tr("Importação em lote"));
    qry_doc_.bindValue(":ST_TOPSEC", false);
    qry_doc_.bindValue(":ST_FIELD01DATA", "");
    qry_doc_.bindValue(":ST_FIELD02DATA", "");
    qry_doc_.bindValue(":ST_FIELD03DATA", "");
    qry_doc_.bindValue(":ST_FIELD04DATA", "");
    qry_doc_.bindValue(":ST_FIELD05DATA", "");

    qry_doc_.exec();
    if (qry_doc_.lastError().isValid()) {
      emit sqlError(
        qry_doc_.lastError().text() + "\n\n" +
        Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text());
      QSqlDatabase::database().rollback();
      QApplication::restoreOverrideCursor();
      return;
    }
    quint64 last_insert_id_ = qry_doc_.lastInsertId().toULongLong();
    QSqlDatabase::database().commit();

    if (last_insert_id_ == 0) {
      emit sqlError(
        Messages_->set(MessagesNS::Tokens::DB_WARN_RECNOTSAVED).text() +
        "lastInsert_ID");
      QSqlDatabase::database().rollback();
      QApplication::restoreOverrideCursor();
      return;
    }
    QSqlDatabase::database().commit();

    /* ---------------------------------------------------------------------- */
    MultiFiles* multiFilesInput_ = new MultiFiles;
    connect(multiFilesInput_,
            SIGNAL(errorText(const QString&)),
            this,
            SLOT(multiFilesErrorText(const QString&)));
    multiFilesInput_->setFileName(std::move(fname_));
    multiFilesInput_->execute();
    QByteArray baInputFile_(multiFilesInput_->data());

    QSqlDatabase::database().transaction();
    QSqlQuery qry_img_;
    qry_img_.prepare(insert_img_stmnt_);
    qry_img_.bindValue(":ST_IDENT", std::move(last_insert_id_));
    qry_img_.bindValue(
      ":IM_IMAGE", qCompress(baInputFile_), QSql::In | QSql::Binary);
    qry_img_.exec();
    if (qry_img_.lastError().isValid()) {
      QSqlDatabase::database().transaction();
      QSqlQuery delInvalidReg_;
      delInvalidReg_.prepare(QString("DELETE FROM DOCSTORAGE WHERE ST_IDENT=%0")
                               .arg(std::move(last_insert_id_)));
      delInvalidReg_.exec();
      QSqlDatabase::database().commit();
      QApplication::restoreOverrideCursor();
      return;
    }
    QSqlDatabase::database().commit();

    emit progressBar(counter_); // progressBar value
  };

  // -------------------------------------------------------------------------

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  if (table_m_.count() > 0) {
    for (int i_ = 0; i_ < table_m_.count(); ++i_) {
      const QString fname_ = std::move(table_m_.value(i_).at(0));
      const QString number_ = std::move(table_m_.value(i_).at(1));
      const QString date_ = std::move(table_m_.value(i_).at(2));
      QString value_ = std::move(table_m_.value(i_).at(3));
      const QString iddoc_ = std::move(table_m_.value(i_).at(4));

      const QFileInfo fInfo_(fname_);
      const QString internalFileNamePattern_ =
        fInfo_.fileName().replace(" ", "_");

      procSql(
        i_, fname_, internalFileNamePattern_, number_, date_, value_, iddoc_);
    }
    QApplication::restoreOverrideCursor();
    return;
  }

  for (int i_ = 0; i_ < indexList_.count(); ++i_) {
    auto column_id_ = std::move(indexList_.data()->sibling(i_, 0));

    const QString fname_ = std::move(column_id_.data().toString());
    const QString number_ = std::move(QString::number(i_ + 100));
    const QString date_ =
      std::move(QDate::currentDate().toString("dd/MM/yyyy"));
    QString value_ = "0.0";
    const QString iddoc_ = "";

    const QFileInfo fInfo_(fname_);
    const QString internalFileNamePattern_ =
      fInfo_.fileName().replace(" ", "_");

    procSql(
      i_, fname_, internalFileNamePattern_, number_, date_, value_, iddoc_);
  }
  QApplication::restoreOverrideCursor();
}

void
ImportBatch::multiFilesErrorText(const QString& msg_)
{
  emit sqlError(msg_);
}
