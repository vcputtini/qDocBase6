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
#include "baseentities_p.h"

#include "globals.h"
#include "progid.h"

/*!
 * \brief BaseEntities_P::BaseEntities_P
 * \param parent
 */
BaseEntities_P::BaseEntities_P(QWidget* parent)
  : QTreeView(parent)
  , siModel(new QStandardItemModel)
{
  fillTree();
}

/*!
 * \brief BaseEntities_P::~BaseEntities_P
 */
BaseEntities_P::~BaseEntities_P() {}

/*!
 * \brief BaseEntities_P::fillTree
 */
void
BaseEntities_P::fillTree()
{
  Globals globals_;

  siModel->clear();
  siModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Categorias")));
  siModel->setHorizontalHeaderItem(1, new QStandardItem(tr("#ID")));
  siModel->setHorizontalHeaderItem(2, new QStandardItem(tr("Nome")));
  siModel->setHorizontalHeaderItem(3, new QStandardItem(tr("ID Documento")));

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  for (auto& a : Globals::categories_list_) {

    QStandardItem* categories_ =
      new QStandardItem(globals_.categories_map_.value(a));
    categories_->setEditable(false);
    categories_->setData(static_cast<int>(a), Qt::UserRole);
    siModel->appendRow(QList<QStandardItem*>() << categories_);

    QString stmnt_{ QString(
                      "SELECT ET_IDENT, ET_NOME, "
                      "IF(ISNULL(ET_BR_CNPJ), ET_BR_CPF, ET_BR_CNPJ) AS DOCID_ "
                      "FROM DOCENTITY WHERE "
                      "ET_ATIVO=1 AND ET_CATEG = %0 ORDER BY ET_NOME")
                      .arg(std::move(static_cast<int>(a))) };

    QSqlDatabase::database().transaction();
    QSqlQuery qry_;
    qry_.prepare(stmnt_);
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

    int row{ 0 };
    while (Q_LIKELY(qry_.next())) {
      QStandardItem* blank_ = new QStandardItem("");
      blank_->setEditable(false);
      QStandardItem* id_ =
        new QStandardItem(std::move(qry_.value(0).toString()));
      id_->setEditable(false);
      QStandardItem* name_ =
        new QStandardItem(std::move(qry_.value(1).toString()));
      name_->setEditable(false);
      QStandardItem* docid_ =
        new QStandardItem(std::move(qry_.value(2).toString()));
      docid_->setEditable(false);

      categories_->appendRow(QList<QStandardItem*>()
                             << blank_ << id_ << name_ << docid_);
    }
    QSqlDatabase::database().commit();
    siModel->setItem(row, 0, categories_);
    ++row;
  }
  setModel(siModel);
  setColumnWidth(2, 500);
  show();
  QApplication::restoreOverrideCursor();
}
