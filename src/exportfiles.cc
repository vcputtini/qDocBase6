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
#include "exportfiles.h"

#include "globals.h"
#include "multifiles.h"
#include "progid.h"
#include "settings.h"

/*!
 * \brief ExportFiles::ExportFiles
 * \param parent
 * \param table_
 */
ExportFiles::ExportFiles(QWidget* parent, QTableView* table_)
  : QWidget{ parent }
  , tableView_{ table_ }
  , newFileName_{}
  , fullFileName_{}
  , baseId_{ 0 }
  , storageId_{ 0 }
{
}

/*!
 * \brief ExportFiles::~ExportFiles
 */
ExportFiles::~ExportFiles() {}

/*!
 * \public
 * \brief ExportFiles::setBaseId
 * \param id_
 */
void
ExportFiles::setBaseId(const quint64 id_)
{
  baseId_ = std::move(id_);
}

/*!
 * \public
 * \brief ExportFiles::setStorageId
 * \param id_
 */
void
ExportFiles::setStorageId(const quint64 id_)
{
  storageId_ = std::move(id_);
}

/*!
 * \public
 * \brief ExportFiles::validateIds
 * \return
 */
bool
ExportFiles::validateIds()
{
  if (baseId_ == 0) {
    emit error(tr("#ID da Entidade Base deve ser maior que zero."));
    return false;
  } else if (storageId_ == 0) {
    emit error(tr("#ID da Entidade Vinculada deve ser maior que zero."));
    return false;
  } else {
    emit error(tr("Nenhuma Entidade Base ou Vinculada foram selecionadas."));
    return false;
  }
  return true;
}

/*!
 * \public SLOT
 * \brief FormDocuments::multiFilesErrorText
 */
void
ExportFiles::multiFilesErrorText(const QString& errText_)
{
  QMessageBox::critical(this,
                        ProgId::Name,
                        tr("Houve um erro tentando ler:<br>") + fullFileName_ +
                          "<BR>" + errText_,
                        QMessageBox::Close);
}

/*!
 * \public
 * \brief ExportFiles::exec
 */
void
ExportFiles::exec()
{
  execute();
}

/*!
 * \private
 * \brief ExportFiles::execute
 */
void
ExportFiles::execute()
{
  QItemSelectionModel* selectionModel_ = tableView_->selectionModel();
  if (!selectionModel_) {
    return;
  }

  QModelIndexList indexList_ = selectionModel_->selectedRows();
  if (indexList_.count() == 0) {
    QMessageBox::information(
      this,
      ProgId::Name,
      tr("Nenhum documento selecionado para exportação."),
      QMessageBox::Close);
    return;
  }

  Settings settings_;

  QMessageBox msgBox;
  msgBox.setText(tr("Você está prestes a exportar [%0] documentos para [%1]")
                   .arg(indexList_.count())
                   .arg(settings_.getExportDir()));
  msgBox.setInformativeText(tr("Você confirma esta operação?"));
  QPushButton* cancelButton_ =
    msgBox.addButton(tr("Cance&lar"), QMessageBox::ActionRole);
  QPushButton* okButton_ =
    msgBox.addButton(tr("Con&firmar"), QMessageBox::ActionRole);
  Q_UNUSED(okButton_);
  msgBox.setDefaultButton(cancelButton_);
  msgBox.exec();
  if (msgBox.clickedButton() == cancelButton_) {
    return;
  }

  QProgressDialog progress(
    tr("Exportando ..."), tr("Cancelar"), 0, indexList_.count(), this);
  progress.setWindowModality(Qt::WindowModal);
  progress.show();

  for (QModelIndex index_ : indexList_) {
    auto column_id_ = index_.model()->sibling(index_.row(), 0, index_);
    auto column_file_name_ = index_.model()->sibling(
      index_.row(), index_.model()->property("colFileName").toInt(), index_);
    if (column_file_name_.data().toString().isEmpty()) {
      return;
    }
    const QString fileName_{ QString("%0/%1")
                               .arg(std::move(settings_.getExportDir()))
                               .arg(std::move(
                                 column_file_name_.data().toString())) };

    if (QFileInfo info_(fileName_); info_.exists()) {
      newFileName_ = "";
      QInputDialog* iDialog = new QInputDialog(this, Qt::Dialog);
      iDialog->setWindowModality(Qt::ApplicationModal);
      iDialog->resize(QSize(300, 300));
      connect(iDialog,
              SIGNAL(textValueSelected(QString)),
              this,
              SLOT(inputDialog_operatorSelected(QString)));
      iDialog->setWindowTitle(tr("Arquivo já existe"));
      iDialog->setCancelButtonText(tr("Sobrepor"));
      iDialog->setOkButtonText(tr("Trocar"));
      iDialog->setInputMode(QInputDialog::TextInput);
      QString infoText_{ QString("%0 [%1] %2")
                           .arg(tr("Arquivo:"))
                           .arg(fileName_)
                           .arg(tr(
                             "Deseja trocar o nome do arquivo ou sobrepor?")) };
      iDialog->setLabelText(infoText_);
      iDialog->setTextEchoMode(QLineEdit::Normal);
      if (int ok_ = iDialog->exec(); ok_ == QDialog::Accepted) {
        newFileName_ = info_.path() +
                       QString("/%0.%1").arg(newFileName_).arg(info_.suffix());
      } else {
        QDir dir_(fileName_);
        dir_.remove(fileName_);
      }
    }

    QStringList listExported_;
    if (!newFileName_.isEmpty()) {
      listExported_ << newFileName_;
    } else {
      listExported_ << fileName_;
    }

    QSqlQuery qry_(QString("SELECT IM_IMAGE FROM DOCIMAGES WHERE ST_IDENT=%0")
                     .arg(std::move(column_id_.data().toULongLong())));
    qry_.next();
    if (qry_.lastError().isValid()) {
      QMessageBox::information(
        this, ProgId::Name, qry_.lastError().text(), QMessageBox::Close);
      break;
    } else {
      QByteArray ba_ = Globals::unCompressImage(qry_.value(0).toByteArray());

      // Save files
      MultiFiles mf_;
      connect(&mf_,
              SIGNAL(errorText(const QString)),
              this,
              SLOT(multiFilesErrorText(const QString)));

      if (!newFileName_.isEmpty()) {
        mf_.setOutputFile(std::move(newFileName_));
      } else {
        mf_.setOutputFile(std::move(fileName_));
      }
      mf_.setData(std::move(ba_));
      mf_.execute();
    }

    if (indexList_.count() > 0) {
      progress.setValue(std::move(indexList_.count()));
    }

  } // end-for
}
