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
#include "departments_p.h"
#include "progid.h"

/*!
 * \brief Departments_P::Departments_P
 * \param parent_
 */
Departments_P::Departments_P(QWidget* parent_)
  : QComboBox(parent_)
  , lineEdit_Depart_(new QLineEdit)
{
  lineEdit_Depart_->setMaxLength(15);
  setLineEdit(lineEdit_Depart_);

  update();
}

/*!
 * \brief Departments_P::~Departments_P
 */
Departments_P::~Departments_P() {}

/*!
 * \brief Departments_P::save
 */
void
Departments_P::save()
{
  if (currentText().isEmpty()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      tr("Nenhum nome de departamento informado.").arg(currentText()),
      QMessageBox::Close);
    return;
  }
  const QString sql = "INSERT INTO DOCDEPARTMENTS (DP_NAME) VALUES (:DP_NAME)";

  QSqlDatabase::database().transaction();
  qry_.clear();
  qry_.prepare(sql);
  qry_.bindValue(":DP_NAME", currentText()); // New departement
  qry_.exec();
  if (qry_.lastError().isValid()) {
    if (qry_.lastError().nativeErrorCode().toInt() == 1062) { // Duplicated
      QMessageBox::information(
        this,
        ProgId::Name,
        tr("[<b>%0</b>] Este departamento já está cadastrado.")
          .arg(currentText()),
        QMessageBox::Close);
      QSqlDatabase::database().rollback();
      return;
    } else
      QSqlDatabase::database().commit();
  }
  update();
}

/*!
 * \brief Departments_P::update
 */
void
Departments_P::update()
{
  clear(); // Delete before insert new items

  QSqlDatabase::database().transaction();
  qry_.prepare("SELECT DP_NAME FROM DOCDEPARTMENTS ORDER BY DP_NAME");
  qry_.exec();
  if (qry_.lastError().isValid()) {
    QSqlDatabase::database().rollback();
    return;
  }

  int i{ 0 };
  while (qry_.next()) {
    insertItem(i, std::move(qry_.value(0).toString()));
    ++i;
  }
  QSqlDatabase::database().commit();
  qry_.clear();
}

/*!
 * \brief Departments_P::erase
 */
void
Departments_P::erase()
{
  if (currentText().isEmpty()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      tr("Nenhum nome de departamento informado.").arg(currentText()),
      QMessageBox::Close);
    return;
  }
  QSqlDatabase::database().transaction();
  qry_.prepare(QString("DELETE FROM DOCDEPARTMENTS WHERE DP_NAME='%0'")
                 .arg(currentText()));
  qry_.exec();
  if (qry_.lastError().isValid()) {
    QMessageBox::information(
      this, ProgId::Name, qry_.lastError().text(), QMessageBox::Close);
    QSqlDatabase::database().rollback();
    return;
  }
  QSqlDatabase::database().commit();
  update();
}
