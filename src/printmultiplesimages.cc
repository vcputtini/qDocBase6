/***************************************************************************
 * Copyright (c) 2007-2023                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase2 project, a images/documents           *
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

#include "printmultiplesimages.h"
#include "globals.h"
#include "progid.h"

/*!
 * \brief PrintMultiplesImages::printReport
 * \param printer
 */
void
PrintMultiplesImages::printReport(QPrinter* printer)
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  QProgressDialog progress(
    tr("Gerando Relatório ..."), tr("Cancelar"), 0, dataStringList_.count());
  progress.setWindowModality(Qt::ApplicationModal);

  if (!painter_.begin(printer)) {
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

  qreal lineSpacing = fontM14B.lineSpacing();

  QRectF r1(0, 0, pageWidth(), pageHeight());
  int i{ 0 };
  header(r1, title());

  for (auto& strL_ : dataStringList_) {
    QSqlQuery qry_(strQuery_ + strL_);
    qry_.next();
    if (qry_.lastError().isValid()) {
      emit sqlLastError(qry_.lastError().text());
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
    progress.setValue(i);
    ++i;

    QByteArray ba_ = Globals::unCompressImage(qry_.value(0).toByteArray());

    QImage image;
    image.loadFromData(ba_);
    QSizeF size = image.size();
    size.scale(r1.size(), Qt::KeepAspectRatio);
    if (size.width() > pageWidth() || size.height() > pageHeight() - 300.0) {
      qreal fact = fitToPage(image, r1);
      if (fact == 0.0) {
        continue;
      }
      size.scale(r1.size() * fact, Qt::KeepAspectRatio);
    }

    painter_.setViewport(
      r1.x(), r1.y() + lineSpacing + 350.0, size.width(), size.height());
    painter_.setWindow(image.rect());
    painter_.drawImage(0, 0, image);

    if (i < dataStringList_.count()) {
      if (!printer->newPage()) {
        QMessageBox::critical(widget_,
                              ProgId::Name,
                              tr("Problemas em alocar nova pagina no disco. "
                                 "Disco pode estar cheio."),
                              QMessageBox::Close);
        break;
      }
      header(r1, title());
    }

  } // end foreach()
  painter_.end();
  progress.setValue(i);
  QApplication::restoreOverrideCursor();
}

/*!
 * \brief PrintMultiplesImages::header
 * \param r
 * \param title
 */
void
PrintMultiplesImages::header(QRectF r, const QString title)
{
  Q_UNUSED(title);

  QFont font14B("Arial", 14, QFont::Bold);
  font14B = QFont(font14B, painter_.device());
  QFont font11B("Arial", 11, QFont::Bold);
  font11B = QFont(font11B, painter_.device());
  QFont font9("Arial", 9, QFont::Normal);
  font9 = QFont(font9, painter_.device());
  QFontMetricsF fontM14B(font14B);
  qreal lineSpacing = fontM14B.lineSpacing();

  painter_.setViewport(r.x(), r.y(), pageWidth(), lineSpacing + 300.0);
  painter_.setWindow(r.x(), r.y(), pageWidth(), lineSpacing + 300.0);
  painter_.setBrush(
    QColor(230, 230, 230)); // gray 10% HTML #e6e6e6 RGB 230,230,230
  painter_.drawRect(r.x(), r.y(), pageWidth(), lineSpacing + 300.0);

  painter_.setFont(QFont(font14B, painter_.device()));
  painter_.drawText(50.0, lineSpacing, ProgId::strName());

  painter_.setFont(QFont(font9, painter_.device()));
  painter_.drawText(50.0, fontM14B.height() + lineSpacing, subTitle());

  painter_.setBrush(Qt::NoBrush);
}
