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
#include "formimportbatch.h"
#include "importbatch.h"
#include "tablewidgetsheet_import_itemdelegate.h"
#include "ui_formimportbatch.h"

#include "progid.h"

/*!
 * \brief FormImportBatch::FormImportBatch
 * \param baseId_
 * \param storageId_
 * \param oper_
 * \param depart_
 * \param parent
 */
FormImportBatch::FormImportBatch(const quint64 baseId_,
                                 const quint64 storageId_,
                                 const QString oper_,
                                 const QString depart_,
                                 QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormImportBatch)
  , mainWindowToolBar(new QMainWindow)
  , toolBarImport(new QToolBar)
  , baseID_{ baseId_ }
  , storageID_{ storageId_ }
  , operator_{ oper_ }
  , department_{ depart_ }
{
  ui->setupUi(this);

  Messages_ = &Messages_->instance();

  ui->progressBar_ImportBatch->reset();

  createToolBar();
  setupTable();
}

/*!
 * \brief FormImportBatch::~FormImportBatch
 */
FormImportBatch::~FormImportBatch()
{
  delete ui;
}

/*!
 * \private
 * \brief FormImportBatch::createToolBar
 */
void
FormImportBatch::createToolBar()
{
  QVBoxLayout* mainLayout_ = new QVBoxLayout(ui->frameToolBar);
  mainLayout_->addWidget(toolBarImport);
  toolBarImport->setFloatable(false);
  toolBarImport->setMovable(false);
  toolButtonExec = new QToolButton;
  toolButtonExec->setIcon(QIcon::fromTheme("system-run"));
  toolButtonExec->setToolTip(tr("Executa Importação"));

  toolBarImport->addWidget(toolButtonExec);

  connect(toolButtonExec, &QToolButton::clicked, this, [this](bool click_) {
    execImport();
  });
}

/*!
 * \private
 * \brief FormImportBatch::setupTable
 */
void
FormImportBatch::setupTable()
{
  ImportBatchitemDelegate* import_ =
    new ImportBatchitemDelegate(ui->tableWidget_Sheet);
  ui->tableWidget_Sheet->setItemDelegate(import_);
}

/*!
 * \private
 * \brief FormImportBatch::execImport
 */
void
FormImportBatch::execImport()
{
  QMap<int, std::array<QString, 5>> table_m_{};
  std::array<QString, 5> item_arr_{};
  for (int i_ = 0; i_ < ui->tableWidget_Sheet->rowCount(); ++i_) {
    for (int j_ = 0; j_ < ui->tableWidget_Sheet->columnCount(); ++j_) {
      QTableWidgetItem* item_ = ui->tableWidget_Sheet->item(i_, j_);
      item_arr_[j_] = item_->text();
    }
    table_m_.insert(i_, item_arr_);
  }

  ui->progressBar_ImportBatch->setMaximum(table_m_.count() - 1);

  ImportBatch importBatch_(table_m_);
  importBatch_.setID(baseID_, storageID_, operator_, department_);
  connect(&importBatch_,
          SIGNAL(sqlError(const QString&)),
          this,
          SLOT(sqlErrorImportBatch(const QString&)));
  connect(
    &importBatch_, SIGNAL(progressBar(int)), this, SLOT(progressBar(int)));
  importBatch_.exec();
}

/*!
 * \public
 * \brief FormImportBatch::addItems
 * \param mdlIndexList_
 */
void
FormImportBatch::addItems(QModelIndexList mdlIndexList_)
{
  for (auto idx_ = mdlIndexList_.cbegin(); idx_ != mdlIndexList_.cend();
       ++idx_) {
    ui->tableWidget_Sheet->addItem(
      idx_->sibling(idx_->row(), 0).data().toString());
  }
  ui->tableWidget_Sheet->fillTable();
}

/*!
 * \public SLOT
 * \brief FormImportBatch::sqlErrorImportBatch
 * \param errText_
 */
void
FormImportBatch::sqlErrorImportBatch(const QString& errText_)
{
  QMessageBox::warning(this, ProgId::Name, errText_, QMessageBox::Ok);
}

/*!
 * \public SLOT
 * \brief FormImportBatch::progressBar
 * \param value_
 */
void
FormImportBatch::progressBar(const int& value_)
{
  ui->progressBar_ImportBatch->setValue(value_);
}
