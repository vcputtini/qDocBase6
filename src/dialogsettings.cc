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
#include "dialogsettings.h"
#include "progid.h"
#include "qevent.h"
#include "ui_dialogsettings.h"

/*!
 * \brief DialogSettings::DialogSettings
 * \param parent
 */
DialogSettings::DialogSettings(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::DialogSettings)
  , saved_(false)
{
  ui->setupUi(this);

  ui->tabWidgetSettings->setCurrentIndex(0);
  ui->tabWidgetSettings->setTabText(0, tr("Autenticação"));
  ui->tabWidgetSettings->setTabText(1, tr("Configurações"));
  ui->tabWidgetSettings->setTabText(2, tr("Visualizações"));

  ui->tabWidgetSettings->setTabEnabled(1, false);
  ui->tabWidgetSettings->setTabEnabled(2, false);

  ui->pushButtonSave->setEnabled(false);

  readSettings();
}

/*!
 * \brief DialogSettings::~DialogSettings
 */
DialogSettings::~DialogSettings()
{
  delete ui;
}

/*!
 * \brief DialogSettings::closeEvent
 * \param event
 */
void
DialogSettings::closeEvent(QCloseEvent* event)
{
  event->accept();
  if (!saved_)
    QMessageBox::information(this,
                             ProgId::Name,
                             tr("As configurações não foram salvas."),
                             QMessageBox::Close);
  close();
}

/*!
 * \brief DialogSettings::pushButtonSave_clicked
 */
void
DialogSettings::pushButtonSave_clicked()
{
  writeSettings();
  close();
}

/*!
 * \brief DialogSettings::pushButtonOk_clicked
 */
void
DialogSettings::pushButtonOk_clicked()
{
  QSqlDatabase::database().transaction();
  QSqlQuery qry_(QString("SELECT DES_DECRYPT(OP_PASSWD,'%0') FROM DOCSECURITY "
                         "WHERE ID=1 AND OP_NAME='admin'")
                   .arg(SECURITY_ENCRYPT_KEY_));
  qry_.exec();
  qry_.next();
  if (qry_.value(0).toString() == ui->lineEdit_Auth->text().toLower()) {
    ui->lineEdit_HostName->setEnabled(true);
    ui->lineEdit_User->setEnabled(true);
    ui->lineEdit_Password->setEnabled(true);
    ui->radioButton_Free->setEnabled(true);
    ui->radioButton_ReadOnly->setEnabled(true);
    ui->radioButton_Free->setEnabled(true);
    for (int i = 1; i <= ui->tabWidgetSettings->count(); ++i) {
      ui->tabWidgetSettings->setTabEnabled(i, true);
    }
    ui->tabWidgetSettings->setCurrentIndex(1);
    ui->pushButtonSave->setEnabled(true);
  } else {
    QMessageBox::critical(this,
                          ProgId::Name,
                          tr("Senha não confere!\nSomente o administrador pode "
                             "alterar as informações."),
                          QMessageBox::Close);
    ui->lineEdit_HostName->setEnabled(false);
    ui->lineEdit_User->setEnabled(false);
    ui->lineEdit_Password->setEnabled(false);
    ui->radioButton_Free->setEnabled(true);
    ui->radioButton_ReadOnly->setEnabled(false);
    ui->radioButton_Free->setEnabled(false);
    for (int i = 1; i <= ui->tabWidgetSettings->count(); ++i) {
      ui->tabWidgetSettings->setTabEnabled(i, false);
    }
  }

  QSqlDatabase::database().commit();
}

/*!
 * \brief DialogSettings::readSettings
 */
void
DialogSettings::readSettings()
{
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());

  ui->lineEdit_RooDir->setText(settings.value("rootdir").toString());
  ui->lineEdit_ExportDir->setText(settings.value("exportdir").toString());

  if (settings.value("tempdir").toString().isEmpty())
    ui->lineEdit_TempDir->setText(QDir::tempPath());
  else
    ui->lineEdit_TempDir->setText(settings.value("tempdir").toString());

  ui->lineEdit_HostName->setText(
    settings.value("hostname").toString().simplified());
  ui->lineEdit_DBName->setText(
    settings.value("dbname").toString().simplified());
  ui->lineEdit_User->setText(settings.value("dbuser").toString().simplified());
  ui->lineEdit_Password->setText(
    settings.value("dbpass").toString().simplified());

  if (settings.value("dbNativeDateFormatISO").toBool()) {
    ui->radioButton_DbDateISO->setChecked(true);
  } else if (settings.value("dbNativeDateFormatEUA").toBool()) {
    ui->radioButton_DbDateEUA->setChecked(true);
  } else {
    ui->radioButton_DbDateEUR->setChecked(true);
  }

  ui->radioButton_Free->setChecked(settings.value("free").toInt());
  ui->radioButton_ReadOnly->setChecked(settings.value("readonly").toInt());
  ui->radioButton_Limited->setChecked(settings.value("rosave").toInt());

  ui->checkBox_DefaultDocId->setChecked(
    settings.value("defaultdocid").toInt() ? true : false);
  ui->checkBox_SetDirImg->setChecked(
    settings.value("setDirImgDflt").toInt() ? true : false);
  ui->checkBox_RemoveImageFromDir->setChecked(
    settings.value("removeImageFromDir").toInt() ? true : false);
  ui->checkBox_KeepNameEntity->setChecked(
    settings.value("keepNameEntity").toInt() ? true : false);

  ui->lineEdit_DefaultDownload->setText(
    settings.value("dirDownload").toString().simplified());

  if (!settings.value("field01Name").toString().isEmpty()) {
    ui->lineEdit_Fld01Name->setText(settings.value("field01Name").toString());
    ui->spinBox_Len_01->setValue(settings.value("field01Len").toInt());
    ui->checkBox_Activated_01->setChecked(
      (settings.value("field01Act").toInt() == 1 ? true : false));
  }
  if (!settings.value("field02Name").toString().isEmpty()) {
    ui->lineEdit_Fld02Name->setText(settings.value("field02Name").toString());
    ui->spinBox_Len_02->setValue(settings.value("field02Len").toInt());
    ui->checkBox_Activated_02->setChecked(
      (settings.value("field02Act").toInt() == 1 ? true : false));
  }
  if (!settings.value("field03Name").toString().isEmpty()) {
    ui->lineEdit_Fld03Name->setText(settings.value("field03Name").toString());
    ui->spinBox_Len_03->setValue(settings.value("field03Len").toInt());
    ui->checkBox_Activated_03->setChecked(
      (settings.value("field03Act").toInt() == 1 ? true : false));
  }
  if (!settings.value("field04Name").toString().isEmpty()) {
    ui->lineEdit_Fld04Name->setText(settings.value("field04Name").toString());
    ui->spinBox_Len_04->setValue(settings.value("field04Len").toInt());
    ui->checkBox_Activated_04->setChecked(
      (settings.value("field04Act").toInt() == 1 ? true : false));
  }
  if (!settings.value("field05Name").toString().isEmpty()) {
    ui->lineEdit_Fld05Name->setText(settings.value("field05Name").toString());
    ui->spinBox_Len_05->setValue(settings.value("field05Len").toInt());
    ui->checkBox_Activated_05->setChecked(
      (settings.value("field05Act").toInt() == 1 ? true : false));
  }
}

/*!
 * \brief DialogSettings::writeSettings
 */
void
DialogSettings::writeSettings()
{
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());

  // Create directories if necessary
  QDir rootDir(ui->lineEdit_RooDir->text());
  QDir exportDir(ui->lineEdit_ExportDir->text());
  QDir tempDir((ui->lineEdit_TempDir->text().isEmpty()
                  ? QDir::tempPath()
                  : ui->lineEdit_TempDir->text()));

  QString dirErro_;
  if (!rootDir.exists()) {
    if (!rootDir.mkpath(ui->lineEdit_RooDir->text())) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        tr("Não foi possível criar o diretório informado. Tente criá-lo "
           "manualmente ou verifique o motivo."),
        QMessageBox::Close);
    }
  } else
    dirErro_ = ui->lineEdit_RooDir->text() + " " + tr("Já existe.") + "<br>";

  if (!exportDir.exists()) {
    if (!exportDir.mkpath(ui->lineEdit_ExportDir->text())) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        tr("Não foi possível criar o diretório informado. Tente criá-lo "
           "manualmente ou verifique o motivo."),
        QMessageBox::Close);
    }
  } else
    dirErro_ += ui->lineEdit_ExportDir->text() + " " + tr("Já existe.") + "\n";

  if (!tempDir.exists()) {
    if (!tempDir.mkpath(ui->lineEdit_TempDir->text())) {
      QMessageBox::warning(
        this,
        ProgId::Name,
        tr("Não foi possível criar o diretório informado. Tente criá-lo "
           "manualmente ou verifique o motivo."),
        QMessageBox::Close);
    }
  } else
    dirErro_ += ui->lineEdit_RooDir->text() + " " + tr("Já existe.") + "\n";

  if (ui->lineEdit_HostName->text().isEmpty()) {
    QMessageBox::critical(this,
                          ProgId::Name,
                          tr("Nome da Host não pode ser deixado em branco."),
                          QMessageBox::Close);
    return;
  } else {
    settings.setValue("hostname", ui->lineEdit_HostName->text());
  }

  if (ui->lineEdit_DBName->text().isEmpty()) {
    QMessageBox::critical(
      this,
      ProgId::Name,
      tr("Nome do banco de dados não pode ser deixado em branco."),
      QMessageBox::Close);
    return;
  } else {
    settings.setValue("dbname", ui->lineEdit_DBName->text().simplified());
  }

  if (ui->lineEdit_User->text().isEmpty()) {
    QMessageBox::critical(this,
                          ProgId::Name,
                          tr("Nome do Usuário não pode ser deixado em branco."),
                          QMessageBox::Close);
    return;
  } else {
    settings.setValue("dbuser", ui->lineEdit_User->text());
  }

  settings.setValue("dbNativeDateFormatISO",
                    ui->radioButton_DbDateISO->isChecked());
  settings.setValue("dbNativeDateFormatEUA",
                    ui->radioButton_DbDateEUA->isChecked());
  settings.setValue("dbNativeDateFormatEUR",
                    ui->radioButton_DbDateEUR->isChecked());

  if (ui->lineEdit_Password->text().isEmpty()) {
    QMessageBox::critical(this,
                          ProgId::Name,
                          tr("Senha não pode ser deixada em branco."),
                          QMessageBox::Close);
    return;
  } else {
    settings.setValue("dbpass", ui->lineEdit_Password->text());
  }

  if (ui->lineEdit_RooDir->text().endsWith("/") ||
      ui->lineEdit_RooDir->text().endsWith("\\"))
    settings.setValue("rootdir",
                      ui->lineEdit_RooDir->text().left(
                        ui->lineEdit_RooDir->text().length() - 1));
  else
    settings.setValue("rootdir", ui->lineEdit_RooDir->text());

  if (ui->lineEdit_ExportDir->text().endsWith("/") ||
      ui->lineEdit_ExportDir->text().endsWith("\\"))
    settings.setValue("exportdir",
                      ui->lineEdit_ExportDir->text().left(
                        ui->lineEdit_ExportDir->text().length() - 1));
  else
    settings.setValue("exportdir", ui->lineEdit_ExportDir->text());

  if (ui->lineEdit_TempDir->text().endsWith("/") ||
      ui->lineEdit_TempDir->text().endsWith("\\"))
    settings.setValue("tempdir",
                      ui->lineEdit_TempDir->text().left(
                        ui->lineEdit_TempDir->text().length() - 1));
  else
    settings.setValue("tempdir", ui->lineEdit_TempDir->text());

  settings.setValue("free", (ui->radioButton_Free->isChecked() ? 1 : 0));
  settings.setValue("readonly",
                    (ui->radioButton_ReadOnly->isChecked() ? 1 : 0));
  settings.setValue("rosave", (ui->radioButton_Limited->isChecked() ? 1 : 0));

  settings.setValue("defaultdocid",
                    (ui->checkBox_DefaultDocId->isChecked() ? 1 : 0));

  settings.setValue("setDirImgDflt",
                    (ui->checkBox_SetDirImg->isChecked() ? 1 : 0));

  settings.setValue("removeImageFromDir",
                    (ui->checkBox_RemoveImageFromDir->isChecked() ? 1 : 0));

  /*!
   * \note If this option is selected, the name of the main entity will not be
   * erased in the save and update operations so that the user does not always
   * have to choose the same one.
   */
  settings.setValue("keepNameEntity",
                    (ui->checkBox_KeepNameEntity->isChecked() ? 1 : 0));

  if (ui->lineEdit_DefaultDownload->text().endsWith("/")) {
    QMessageBox::information(
      this,
      ProgId::Name,
      tr("Por favor remova todos os caracteres '/' ao final da linha: [%0]")
        .arg(ui->lineEdit_DefaultDownload->text()),
      QMessageBox::Close);
    return;
  }

  if (ui->lineEdit_DefaultDownload->text().isEmpty()) {
    QString m;
#ifdef Q_OS_WIN32
    m = "C:\\temp";
#else
    m = "/tmp";
#endif
    QMessageBox::information(
      this,
      ProgId::Name,
      tr("Diretório padrão para download colocado como <b>%0</b>").arg(m),
      QMessageBox::Close);
    settings.setValue("dirDownload", m);
    return;
  } else {
    settings.setValue("dirDownload", ui->lineEdit_DefaultDownload->text());
  }

  if (ui->checkBox_Activated_01->isChecked()) {
    if (!ui->lineEdit_Fld01Name->text().isEmpty() &&
        ui->spinBox_Len_01->value() > 0) {
      settings.setValue("field01Name", ui->lineEdit_Fld01Name->text());
      settings.setValue("field01Len", ui->spinBox_Len_01->value());
      settings.setValue("field01Act",
                        (ui->checkBox_Activated_01->isChecked() ? 1 : 0));
    } else {
      QMessageBox::warning(
        this,
        ProgId::Name,
        tr("Você não pode definir um campo com tamanho zero."),
        QMessageBox::Close);
    }
  } else {
    settings.setValue("field01Name", "");
    settings.setValue("field01Len", "0");
    settings.setValue("field01Act", "0");
  }

  if (ui->checkBox_Activated_02->isChecked()) {
    if (!ui->lineEdit_Fld02Name->text().isEmpty() &&
        ui->spinBox_Len_02->value() > 0) {
      settings.setValue("field02Name", ui->lineEdit_Fld02Name->text());
      settings.setValue("field02Len", ui->spinBox_Len_02->value());
      settings.setValue("field02Act",
                        (ui->checkBox_Activated_02->isChecked() ? 1 : 0));
    } else {
      QMessageBox::warning(
        this,
        ProgId::Name,
        tr("Você não pode definir um campo com tamanho zero."),
        QMessageBox::Close);
    }
  } else {
    settings.setValue("field02Name", "");
    settings.setValue("field02Len", "0");
    settings.setValue("field02Act", "0");
  }

  if (ui->checkBox_Activated_03->isChecked()) {
    if (!ui->lineEdit_Fld03Name->text().isEmpty() &&
        ui->spinBox_Len_03->value() > 0) {
      settings.setValue("field03Name", ui->lineEdit_Fld03Name->text());
      settings.setValue("field03Len", ui->spinBox_Len_03->value());
      settings.setValue("field03Act",
                        (ui->checkBox_Activated_03->isChecked() ? 1 : 0));
    } else {
      QMessageBox::warning(
        this,
        ProgId::Name,
        tr("Você não pode definir um campo com tamanho zero."),
        QMessageBox::Close);
    }
  } else {
    settings.setValue("field03Name", "");
    settings.setValue("field03Len", "0");
    settings.setValue("field03Act", "0");
  }

  if (ui->checkBox_Activated_04->isChecked()) {
    if (!ui->lineEdit_Fld04Name->text().isEmpty() &&
        ui->spinBox_Len_01->value() > 0) {
      settings.setValue("field04Name", ui->lineEdit_Fld04Name->text());
      settings.setValue("field04Len", ui->spinBox_Len_04->value());
      settings.setValue("field04Act",
                        (ui->checkBox_Activated_04->isChecked() ? 1 : 0));
    } else {
      QMessageBox::warning(
        this,
        ProgId::Name,
        tr("Você não pode definir um campo com tamanho zero."),
        QMessageBox::Close);
    }
  } else {
    settings.setValue("field04Name", "");
    settings.setValue("field04Len", "0");
    settings.setValue("field04Act", "0");
  }

  if (ui->checkBox_Activated_05->isChecked()) {
    if (!ui->lineEdit_Fld05Name->text().isEmpty() &&
        ui->spinBox_Len_05->value() > 0) {
      settings.setValue("field05Name", ui->lineEdit_Fld05Name->text());
      settings.setValue("field05Len", ui->spinBox_Len_05->value());
      settings.setValue("field05Act",
                        (ui->checkBox_Activated_05->isChecked() ? 1 : 0));
    } else {
      QMessageBox::warning(
        this,
        ProgId::Name,
        tr("Você não pode definir um campo com tamanho zero."),
        QMessageBox::Close);
    }
  } else {
    settings.setValue("field05Name", "");
    settings.setValue("field05Len", "0");
    settings.setValue("field05Act", "0");
  }

  // -------------------------------------------------------------------------
  QString msg = tr("Configurações salvas com sucesso.");
  if (!dirErro_.isEmpty()) {
    msg += tr("Porém a(s) pasta(s) abaixo já existem e não foram criadas.") +
           dirErro_;
  }
  QMessageBox::information(this, ProgId::Name, msg, QMessageBox::Close);
  saved_ = true;
}
