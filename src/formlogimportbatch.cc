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
#include "formlogimportbatch.h"
#include "ui_formlogimportbatch.h"

#include "progid.h"

FormLogImportBatch::FormLogImportBatch(QStringList log_, QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::FormLogImportBatch)
  , strList_(log_)
  , saveLoc_(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))
{
  ui->setupUi(this);

  ui->label_SaveLocation->setText(tr("Local: %0").arg(saveLoc_));

  connect(ui->pushButton_SaveToFile,
          SIGNAL(clicked(bool)),
          this,
          SLOT(saveToFile(bool)));

  for (auto& a : strList_) {
    ui->textEdit_Log->append(a);
  }
  ui->textEdit_Log->show();
}

FormLogImportBatch::~FormLogImportBatch()
{
  delete ui;
}

void
FormLogImportBatch::saveToFile(bool clicked)
{
  const QString fn_ =
    QString("%0/%1_%2_qdocbase6_importbatch.log")
      .arg(std::move(saveLoc_))
      .arg(std::move(QDate::currentDate().toString("yyyy-MM-dd")))
      .arg(std::move(QTime::currentTime().toString("hhmmss")));

  QFile file(fn_);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::warning(this,
                         ProgId::Name,
                         tr("Erro: [%0 - %1]\nTentando criar o arquivo: %2")
                           .arg(std::move(file.error()))
                           .arg(std::move(file.errorString()))
                           .arg(std::move(fn_)),
                         QMessageBox::Close);
    return;
  }

  QTextStream out(&file);
  out << ui->textEdit_Log->toPlainText();
}
