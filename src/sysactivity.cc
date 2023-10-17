/***************************************************************************
 * Copyright (c) 2007-2023                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase6 project, a images/documents          *
 * database program.                                                       *
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

#include "sysactivity.h"
#include "globals.h"
#include "progid.h"

/*!
 * \brief SysActivity::SysActivity
 * \param parent
 */
SysActivity::SysActivity(QWidget* parent)
  : QWidget(parent)
{
}

/*!
 * \brief SysActivity::~SysActivity
 */
SysActivity::~SysActivity() {}

/*!
 * \brief SysActivity::logger
 * \param modules_
 * \param operations_
 * \param docid_
 * \return bool
 */
bool
SysActivity::logger(Globals::ModulesOfApp modules_,
                    Operations operations_,
                    qint64 docid_)
{
  QInputDialog* iDialog = new QInputDialog(this, Qt::Dialog);
  iDialog->setWindowModality(Qt::ApplicationModal);
  iDialog->resize(QSize(300, 300));
  connect(iDialog,
          SIGNAL(textValueSelected(QString)),
          this,
          SLOT(inputDialog_operatorSelected(QString)));
  iDialog->setWindowTitle(tr("Seguranca do Sistema"));
  iDialog->setCancelButtonText(tr("Cancelar"));
  iDialog->setOkButtonText(tr("Confirmar"));
  iDialog->setInputMode(QInputDialog::TextInput);
  iDialog->setLabelText(tr("Esta operação requer que "
                           "você informe o nome do Operador:"));
  iDialog->setTextEchoMode(QLineEdit::Normal);

  if (int ok_ = iDialog->exec(); ok_ == QDialog::Accepted) {
    if (!Globals::isOperatorValid(
          operatorsName_.simplified())) { // only user's name
      QMessageBox::warning(this,
                           ProgId::Name,
                           tr("Nome do Operador inválido."
                              "Tente novamente."),
                           QMessageBox::Close);
      operatorsName_ = QString();
      disconnect(
        iDialog, SIGNAL(textValueSelected(const QString&)), nullptr, nullptr);
      return false;
    }
    save(operatorsName_, modules_, operations_, docid_);
    disconnect(
      iDialog, SIGNAL(textValueSelected(const QString&)), nullptr, nullptr);
    return true;
  }
  disconnect(
    iDialog, SIGNAL(textValueSelected(const QString&)), nullptr, nullptr);
  return false;
}

/*!
 * \brief SysActivity::inputDialog_operatorSelected
 * \param text
 */
void
SysActivity::inputDialog_operatorSelected(const QString& text_)
{
  operatorsName_ = std::move(text_);
}

/*!
 * \brief SysActivity::save
 * \param user_
 * \param modules_
 * \param operations_
 * \param docid_
 */
void
SysActivity::save(const QString user_,
                  Globals::ModulesOfApp modules_,
                  Operations operations_,
                  qint64 docid_)
{
  Globals::ModulesOfApp m_ = std::move(modules_);
  Operations op_ = std::move(operations_);

  QSqlDatabase::database().transaction();
  QSqlQuery qry_;
  qry_.prepare("INSERT INTO DOCSYSACTIVITY "
               "(ACT_HOSTNAME, ACT_USER, ACT_MODULE, ACT_OPERATION, ACT_DOCID) "
               "VALUES (:ACT_HOSTNAME, :ACT_USER, :ACT_MODULE, :ACT_OPERATION, "
               ":ACT_DOCID)");
  qry_.bindValue(":ACT_HOSTNAME", std::move(QHostInfo::localHostName()));
  qry_.bindValue(":ACT_USER", std::move(user_));
  qry_.bindValue(":ACT_MODULE", std::move(static_cast<int>(m_)));
  qry_.bindValue(":ACT_OPERATION", std::move(static_cast<int>(op_)));
  qry_.bindValue(":ACT_DOCID", std::move(docid_));
  qry_.exec();
  if (qry_.lastError().isValid()) {
    emit lastError(qry_.lastError().text());
    QSqlDatabase::database().rollback();
  }

  QSqlDatabase::database().commit();
}
