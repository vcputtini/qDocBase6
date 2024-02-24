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
#include "tableview_documents_p.h"

#include "progid.h"

/*!
 * \brief TableView_Documents_P::TableView_Documents_P
 * \param parent
 */
TableView_Documents_P::TableView_Documents_P(QWidget* parent)
  : QTableView(parent)
  , siModel(new QStandardItemModel)
{
  fillView();
}

/*!
 * \brief TableView_Documents_P::~TableView_Documents_P
 */
TableView_Documents_P::~TableView_Documents_P() {}

/*!
 * \brief TableView_Documents_P::setCategorie
 * \param categ_
 */
void
TableView_Documents_P::setCategorie(Globals::Categories categ_)
{
  categorie_ = std::move(categ_);
}

/*!
 * \brief TableView_Documents_P::setEntityId
 * \param id_
 */
void
TableView_Documents_P::setEntityId(quint64 id_)
{
  idEntity_ = std::move(id_);
}

/*!
 * \brief TableView_Documents_P::setEntityiDStorage
 * \param id_
 */
void
TableView_Documents_P::setEntityIdStorage(quint64 id_)
{
  idStoEntity_ = std::move(id_);
}

void
TableView_Documents_P::setDateRange(int bYear_, int eYear_)
{
  beginYear_ = std::move(bYear_);
  endYear_ = std::move(eYear_);
}

/*!
 * \brief TableView_Documents_P::fillView
 */
void
TableView_Documents_P::fillView()
{
  siModel->clear();
  siModel->setHorizontalHeaderItem(0, new QStandardItem(tr("#ID")));
  siModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Data\nCadastro")));
  siModel->setHorizontalHeaderItem(2, new QStandardItem(tr("Operador")));
  siModel->setHorizontalHeaderItem(3, new QStandardItem(tr("Número\nDoc.")));
  siModel->setHorizontalHeaderItem(4, new QStandardItem(tr("Data Doc.")));
  siModel->setHorizontalHeaderItem(5, new QStandardItem(tr("Valor")));
  siModel->setHorizontalHeaderItem(6, new QStandardItem(tr("Arquivo")));
  siModel->setHorizontalHeaderItem(7, new QStandardItem(tr("Vínculo")));
  siModel->setProperty("colFileName", 6);

  QString stmnt_{ QString("SELECT B.ST_IDENT, "
                          "B.ST_RECDATE, "
                          "B.ST_OPERATOR, "
                          "B.ST_NUMBER, "
                          "B.ST_DOCDATE, "
                          "FORMAT(B.ST_VALUE,2,'%0') AS ST_VALUE, "
                          "B.ST_TOPSEC, B.ST_FILE, "
                          "(SELECT A.ET_NOME FROM DOCENTITY A WHERE "
                          "A.ET_IDENT=B.ST_ETRELA) AS LINK_NAME "
                          "FROM DOCSTORAGE B "
                          "WHERE B.ET_IDENT=%1 "
                          "AND YEAR(B.ST_RECDATE) BETWEEN %2 AND %3 "
                          "ORDER BY B.ST_IDENT")
                    .arg(std::move(Globals::whatLocale().first))
                    .arg(std::move(idEntity_))
                    .arg(std::move(beginYear_))
                    .arg(std::move(endYear_)) };

  QSqlDatabase::database().transaction();
  QSqlQuery qry_;
  qry_.prepare(std::move(stmnt_));
  qry_.exec();
  if (qry_.lastError().isValid()) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         qry_.lastError().text() +
                           tr("<br>Não foi possível obter os dados."),
                         QMessageBox::Close);
    QSqlDatabase::database().rollback();
    QApplication::restoreOverrideCursor();
    return;
  }

  while (Q_LIKELY(qry_.next())) {

    QStandardItem* st_ident_;
    QStandardItem* st_regdate_;
    QStandardItem* st_operator_;
    QStandardItem* st_docdate_;
    QStandardItem* st_number;
    QStandardItem* st_value;
    QStandardItem* st_fname;
    QStandardItem* st_link_name;

    st_ident_ = new QStandardItem(qry_.value(0).toString());

    if (Globals::is_ptBR()) {
      st_regdate_ = new QStandardItem(
        std::move(Globals::to_ptBR_Date(qry_.value(1).toString())));
    } else {
      st_regdate_ = new QStandardItem(std::move(qry_.value(1).toString()));
    }
    if (!qry_.value(6).toBool()) {
      st_operator_ = new QStandardItem(std::move(qry_.value(2).toString()));
      st_number = new QStandardItem(std::move(qry_.value(3).toString()));
      if (Globals::is_ptBR()) {
        st_docdate_ = new QStandardItem(
          std::move(Globals::to_ptBR_Date(qry_.value(4).toString())));
      } else {
        st_docdate_ = new QStandardItem(std::move(qry_.value(4).toString()));
      }
      st_value = new QStandardItem(std::move(qry_.value(5).toString()));
      st_fname = new QStandardItem(std::move(qry_.value(7).toString()));
      st_link_name = new QStandardItem(std::move(qry_.value(8).toString()));
    } else {
      st_operator_ = new QStandardItem(tr("Blocked by Operator"));
      st_number = new QStandardItem("");
      st_docdate_ = new QStandardItem("");
      st_value = new QStandardItem("");
      st_fname = new QStandardItem("");
      st_link_name = new QStandardItem("");
    }

    siModel->appendRow(QList<QStandardItem*>()
                       << std::move(st_ident_) << std::move(st_regdate_)
                       << std::move(st_operator_) << st_number
                       << std::move(st_docdate_) << std::move(st_value)
                       << std::move(st_fname) << std::move(st_link_name)

    );
  }
  QSqlDatabase::database().commit();

  setModel(siModel);
  setColumnWidth(6, 100); // file name
  setColumnWidth(7, 500); // Entity linked to the document
  show();
}
