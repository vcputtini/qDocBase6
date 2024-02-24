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
#include "formtopsecret2.h"
#include "customuppervalidator.h"
#include "progid.h"
#include "ui_formtopsecret2.h"

/*!
 * \brief FormTopSecret2::FormTopSecret2
 * \param parent
 */
FormTopSecret2::FormTopSecret2(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormTopSecret2)
{
  ui->setupUi(this);

  ui->lineEdit_Operator->setValidator(new CustomUpperValidator);
  ui->lineEdit_CurrentPass->setEnabled(false);
  ui->lineEdit_NewPass->setEnabled(false);
  ui->lineEdit_ConfirmPass->setEnabled(false);

  ui->pushButton_UpdateAdm->setIcon(QIcon::fromTheme("edit-redo"));
  ui->pushButton_UpdateAdm->setEnabled(false);
  ui->pushButton_Save->setIcon(QIcon::fromTheme("document-save"));
  ui->pushButton_Update->setIcon(QIcon::fromTheme("edit-redo"));
  ui->pushButton_Delete->setIcon(QIcon::fromTheme("edit-delete"));
}

/*!
 * \brief FormTopSecret2::~FormTopSecret2
 */
FormTopSecret2::~FormTopSecret2()
{
  delete ui;
}

/*!
 * \private
 * \brief FormTopSecret2::reload
 */
void
FormTopSecret2::reload()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  pModel_ = new QSqlQueryModel;
  pModel_->setQuery("SELECT OP_NAME FROM DOCSECURITY WHERE OP_NAME<>'admin' "
                    "ORDER BY OP_NAME");
  pModel_->setHeaderData(0, Qt::Horizontal, tr("Nome do Operador"));
  ui->tableView_Operators->setModel(pModel_);
  ui->tableView_Operators->resizeColumnToContents(0);
  QApplication::restoreOverrideCursor();
}

/*!
 * \public SLOT
 * \brief FormTopSecret2::pushButton_Release_clicked
 */
void
FormTopSecret2::pushButton_Release_clicked()
{
  if (ui->lineEdit_UserName->text().isEmpty() ||
      ui->lineEdit_Password->text().isEmpty()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      tr("Nome do usuário Administrador e senha não foram informados"),
      QMessageBox::Close);
    return;
  }
  QSqlDatabase::database().transaction();
  QSqlQuery qry_;
  qry_.prepare(QString("SELECT DES_DECRYPT(OP_PASSWD,'%0') FROM DOCSECURITY "
                       "WHERE ID=1 AND OP_NAME=:OPERATOR")
                 .arg(SECURITY_ENCRYPT_KEY_));
  qry_.bindValue(":OPERATOR", ui->lineEdit_UserName->text().simplified());
  qry_.exec();
  qry_.next();
  if (qry_.value(0).toString() == ui->lineEdit_Password->text().simplified()) {
    ui->groupBox_Operators->setEnabled(true);
    ui->lineEdit_CurrentPass->setEnabled(true);
    ui->lineEdit_NewPass->setEnabled(true);
    ui->lineEdit_ConfirmPass->setEnabled(true);
    ui->pushButton_UpdateAdm->setEnabled(true);
    reload();
  } else
    QMessageBox::warning(
      this,
      ProgId::Name,
      tr("Usuário e/ou senha não combinam. Tente novamente."),
      QMessageBox::Close);
  QSqlDatabase::database().commit();
}

/*!
 * \public SLOT
 * \brief FormTopSecret2::pushButton_UpdateAdm_clicked
 */
void
FormTopSecret2::pushButton_UpdateAdm_clicked()
{
  QString currentPass_ = ui->lineEdit_CurrentPass->text().simplified();
  QString newPass_ = ui->lineEdit_NewPass->text().simplified();
  QString confirmPass_ = ui->lineEdit_ConfirmPass->text().simplified();

  if (currentPass_.isEmpty() || newPass_.isEmpty() || confirmPass_.isEmpty()) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Todos os campos devem ser informados."),
                         QMessageBox::Close);
    return;
  }
  if (newPass_ != confirmPass_) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Nova senha não é igual a senha e confirmação."),
                         QMessageBox::Close);
    return;
  }
  QSqlDatabase::database().transaction();
  QSqlQuery qry_(QString("SELECT DES_DECRYPT(OP_PASSWD,'%0') FROM DOCSECURITY "
                         "WHERE ID=1 AND OP_NAME='admin'")
                   .arg(SECURITY_ENCRYPT_KEY_));
  qry_.exec();
  qry_.next();
  if (qry_.value(0).toString() == currentPass_) {
    QSqlDatabase::database().commit();
    QSqlDatabase::database().transaction();
    QString stmnt_ =
      QString("UPDATE DOCSECURITY SET OP_PASSWD=DES_ENCRYPT(:PASSWORD,'%0') "
              "WHERE ID=1 AND OPERATOR='admin'")
        .arg(SECURITY_ENCRYPT_KEY_);
    QSqlQuery qry_;
    qry_.prepare(stmnt_);
    qry_.bindValue(":PASSWORD", newPass_);
    qry_.exec();
    if (qry_.lastError().isValid()) {
      QMessageBox::warning(this,
                           ProgId::Name,
                           qry_.lastError().text() +
                             tr("Senha não pode ser atualizada."),
                           QMessageBox::Close);
      QSqlDatabase::database().rollback();
    }
    QSqlDatabase::database().commit();
    QMessageBox::information(this,
                             ProgId::Name,
                             tr("Senha atualizada com sucesso."),
                             QMessageBox::Close);
  } else {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Senha não pode ser encontrada."),
                         QMessageBox::Close);
    QSqlDatabase::database().rollback();
  }
}

/*!
 * \public SLOT
 * \brief FormTopSecret2::pushButton_Delete_clicked
 */
void
FormTopSecret2::pushButton_Delete_clicked()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  QSqlDatabase::database().transaction();
  QSqlQuery qry_;
  qry_.prepare(QString("SELECT ST_OPERATOR FROM DOCSTORAGE WHERE "
                       "ST_OPERATOR='%0' LIMIT 1")
                 .arg(ui->lineEdit_Operator->text().simplified()));
  qry_.exec();
  qry_.next();
  if (!qry_.value(0).toString().isEmpty()) {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(this,
                          ProgId::Name,
                          tr("Não é permitido apagar este operador pois "
                             "existem documentos relacionados a ele."),
                          QMessageBox::Close);
    return;
  }
  QSqlDatabase::database().commit();
  QApplication::restoreOverrideCursor();

  QString stmnt_ = "DELETE FROM DOCSECURITY WHERE OP_NAME=:OPERATOR";
  QSqlDatabase::database().transaction();
  qry_.clear();
  qry_.prepare(stmnt_);
  qry_.bindValue(":OPERATOR", ui->lineEdit_Operator->text().simplified());
  qry_.exec();
  if (qry_.lastError().isValid()) {
    QMessageBox::critical(this,
                          ProgId::Name,
                          qry_.lastError().text() +
                            tr("Operador não pode ser apagado."),
                          QMessageBox::Close);
    QSqlDatabase::database().rollback();
    return;
  }
  QSqlDatabase::database().commit();
  QMessageBox::information(this,
                           ProgId::Name,
                           qry_.lastError().text() +
                             tr("Operação executada com sucesso!"),
                           QMessageBox::Close);
  clearOperatorsFields();
  reload();
}

/*!
 * \public SLOT
 * \brief FormTopSecret2::pushButton_Update_clicked
 */
void
FormTopSecret2::pushButton_Update_clicked()
{
  if (ui->lineEdit_Operator->text().isEmpty()) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Nome do Operador é obrigatório."),
                         QMessageBox::Close);
    return;
  }

  if (ui->lineEdit_Operator_Password->text().isEmpty()) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Senha do Operador é obrigatória."),
                         QMessageBox::Close);
    return;
  }

  if (ui->lineEdit_Operator_PassRepeat->text().isEmpty()) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Senha de confirmação é obrigatória."),
                         QMessageBox::Close);
    return;
  }

  if (ui->lineEdit_Operator_Password->text().simplified() !=
      ui->lineEdit_Operator_PassRepeat->text().simplified()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      tr("Senha de confirmação não combina com a senha nova."),
      QMessageBox::Close);
    return;
  }

  QSqlDatabase::database().transaction();
  QSqlQuery qry_;
  qry_.prepare(
    QString("UPDATE DOCSECURITY SET OP_PASSWD=DES_ENCRYPT(:PASSWORD,'%0') "
            "WHERE OP_NAME=:OPERATOR")
      .arg(SECURITY_ENCRYPT_KEY_));
  qry_.bindValue(":OPERATOR", ui->lineEdit_Operator->text().simplified());
  qry_.bindValue(":PASSWORD",
                 ui->lineEdit_Operator_Password->text().simplified());
  qry_.exec();
  if (qry_.lastError().isValid()) {
    QMessageBox::critical(this,
                          ProgId::Name,
                          qry_.lastError().text() +
                            tr("Operador não pode ser gravado."),
                          QMessageBox::Close);
    QSqlDatabase::database().rollback();
    return;
  }
  QSqlDatabase::database().commit();
  QMessageBox::information(this,
                           ProgId::Name,
                           qry_.lastError().text() +
                             tr("Operação executada com sucesso!"),
                           QMessageBox::Close);
  clearOperatorsFields();
}

/*!
 * \public SLOT
 * \brief FormTopSecret2::pushButton_Save_clicked
 */
void
FormTopSecret2::pushButton_Save_clicked()
{
  if (ui->lineEdit_Operator->text().simplified() == "ADMIN") {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Nome do Operador não pode ser <b>ADMIN</b>"),
                         QMessageBox::Close);
    return;
  }

  if (ui->lineEdit_Operator->text().isEmpty()) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Nome do Operador é obrigatório."),
                         QMessageBox::Close);
    return;
  }

  if (ui->lineEdit_Operator_Password->text().isEmpty()) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Senha do Operador é obrigatória."),
                         QMessageBox::Close);
    return;
  }

  if (ui->lineEdit_Operator_PassRepeat->text().isEmpty()) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Confirme a senha informada para o Operador."),
                         QMessageBox::Close);
    return;
  }

  QSqlDatabase::database().transaction();
  QSqlQuery qry_;
  qry_.prepare(QString("INSERT INTO DOCSECURITY (OP_NAME, OP_PASSWD) VALUES "
                       "(:OPERATOR, DES_ENCRYPT(:PASSWORD,'%0'))")
                 .arg(SECURITY_ENCRYPT_KEY_));

  qry_.bindValue(":OPERATOR", ui->lineEdit_Operator->text().simplified());
  qry_.bindValue(":PASSWORD",
                 ui->lineEdit_Operator_Password->text().simplified());
  qry_.exec();
  if (qry_.lastError().isValid()) {
    QMessageBox::critical(this,
                          ProgId::Name,
                          qry_.lastError().text() +
                            tr("Operador não pode ser gravado."),
                          QMessageBox::Close);
    QSqlDatabase::database().rollback();
    return;
  }
  QSqlDatabase::database().commit();
  clearOperatorsFields();
  reload();
}

/*!
 * \public SLOT
 * \brief FormTopSecret2::tableView_Operators_doubleClicked
 * \param idx
 */
void
FormTopSecret2::tableView_Operators_doubleClicked(QModelIndex idx)
{
  ui->lineEdit_Operator->setText(
    pModel_->data(pModel_->index(idx.row(), 0)).toString());
  ui->lineEdit_Operator_Password->clear();
  ui->lineEdit_Operator_PassRepeat->clear();
}

/*!
 * \private
 * \brief FormTopSecret2::clearOperatorsFields
 */
void
FormTopSecret2::clearOperatorsFields()
{
  ui->lineEdit_Operator->clear();
  ui->lineEdit_Operator_Password->clear();
  ui->lineEdit_Operator_PassRepeat->clear();
}
