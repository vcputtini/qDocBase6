/***************************************************************************
 * Copyright (c) 2007-2023                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase6 project, a images/documents           *
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

#include "printimage.h"

#include "progid.h"

/*!
 * \public
 * \brief PrintImage::printReport
 * \param prt_
 */
void
PrintImage::printReport(QPrinter* prt_)
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  QProgressDialog progress(
    tr("Gerando Relatório ..."), tr("Cancelar"), 0, dataStringList_.count());
  progress.setWindowModality(Qt::ApplicationModal);

  if (!painter_.begin(prt_)) {
    QMessageBox::critical(
      widget_,
      ProgId::Name,
      tr("Não foi possível abrir impressora para escrita.<br>Pode ser que o "
         "dispositivo não esteja instalado."),
      QMessageBox::Close);
    QApplication::restoreOverrideCursor();
    return;
  }
  QFont font14B("Arial", 14, QFont::Bold);
  font14B = QFont(font14B, painter_.device());
  QFontMetricsF fontM14B(font14B);

  const qreal lineSpacing_ = fontM14B.lineSpacing();

  QRectF r1_(0, 0, pageWidth(), pageHeight());

  header(r1_, tr("Listagem de Documentos Digitalizados"));

  int counter_{ 0 };
  for (const auto& strL : dataStringList_) {
    QString stmt = strQuery_ + strL;

    QSqlQuery qry_;
    qry_.prepare(stmt);
    qry_.exec();
    qry_.next();
    if (qry_.lastError().isValid()) {
      QMessageBox::warning(
        widget_, ProgId::Name, qry_.lastError().text(), QMessageBox::Close);
      qry_.clear();
      break;
    }

    if (qry_.value(0).toByteArray().isEmpty()) {
      continue;
    }
    qApp->processEvents();
    if (progress.wasCanceled()) {
      break;
    }
    progress.setValue(counter_);
    ++counter_;

    auto unCompress_ = [](const QByteArray ba_) {
      return qUncompress(std::move(ba_));
    };
    QFuture<QByteArray> future_ =
      QtConcurrent::run(unCompress_, qry_.value(0).toByteArray());
    QByteArray ba_ = future_.result();

    QImage image_;
    image_.loadFromData(std::move(ba_));
    QSizeF size = image_.size();
    size.scale(r1_.size(), Qt::KeepAspectRatio);
    if (size.width() > pageWidth() || size.height() > pageHeight() - 300.0) {
      const qreal fact_ = fitToPage(image_, r1_);
      if (fact_ == 0.0) { // doesn't fit on the page
        continue;
      }
      size.scale(r1_.size() * fact_, Qt::KeepAspectRatio);
    }

    painter_.setViewport(
      r1_.x(), r1_.y() + lineSpacing_ + 200.0, size.width(), size.height());
    painter_.setWindow(image_.rect());
    painter_.drawImage(0, 0, image_);

    if (counter_ < dataStringList_.count()) {
      if (!prt_->newPage()) {
        QMessageBox::warning(
          widget_,
          ProgId::Name,
          tr("Problemas ao tentar saltar para próxima página."),
          QMessageBox::Close);
        break;
      }
      header(r1_, tr("Listagem de Documentos Digitalizados"));
    }
  }
  painter_.end();
  progress.setValue(counter_);
  QApplication::restoreOverrideCursor();
}

/*!
 * \private
 * \brief PrintImage::header
 * \param rect_
 * \param title_
 */
void
PrintImage::header(QRectF rect_, const QString title_)
{
  QFont font14B_("Arial", 14, QFont::Bold);
  font14B_ = QFont(font14B_, painter_.device());
  QFont font11B_("Arial", 11, QFont::Bold);
  font11B_ = QFont(font11B_, painter_.device());

  QFontMetricsF fontM14B_(font14B_);
  const qreal vertLineSpacing_ = fontM14B_.lineSpacing();

  painter_.setViewport(
    rect_.x(), rect_.y(), pageWidth(), vertLineSpacing_ + 100.0);
  painter_.setWindow(
    rect_.x(), rect_.y(), pageWidth(), vertLineSpacing_ + 100.0);
  painter_.setBrush(
    QColor(230, 230, 230)); // gray 10% HTML #e6e6e6 RGB 230,230,230
  painter_.drawRect(
    rect_.x(), rect_.y(), pageWidth(), vertLineSpacing_ + 100.0);
  painter_.setFont(QFont(font14B_, painter_.device()));
  painter_.drawText(50.0, vertLineSpacing_, tr(ProgId::Name));

  painter_.setFont(QFont(font11B_, painter_.device()));
  const qreal fw_ = fontM14B_.horizontalAdvance(ProgId::Name);
  painter_.drawText(fw_ + 60.00, vertLineSpacing_, " - " + title_);

  painter_.setBrush(Qt::NoBrush);
}
