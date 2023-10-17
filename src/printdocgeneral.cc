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

#include "printdocgeneral.h"

/*!
 * \brief PrintDocGeneral::printReport
 * \param printer
 */
void
PrintDocGeneral::printReport(QPrinter* printer)
{

  painter_.begin(printer);

  QFont font_("Courier New", 10);
  font_ = QFont(font_, painter_.device());
  QFontMetricsF fontMetrics_(font_);

  setColumnHeaders();

  int npag_ = 1;
  qreal y_ = 0;
  y_ = printDefaultHeader(npag_);

  const qreal leftMargin_ = getLeftMargin();
  qreal pageHeight_ = pageHeight();

  painter_.setFont(font_);

  QProgressDialog progress(
    tr("Gerando Relatório ..."), tr("Cancelar"), 0, sqlModel_->rowCount() - 1);
  progress.setWindowModality(Qt::ApplicationModal);

  QString baseEntity_ =
    "Entidade Base: [" + sqlModel_->record(0).value("ENTITY").toString() + "]";

  double grandTotal_ = 0.0;

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  for (int i_ = 0; i_ <= sqlModel_->rowCount() - 1; ++i_) {
    qApp->processEvents();
    if (progress.wasCanceled()) {
      break;
    }
    progress.setValue(i_);

    QString st_ident_ = sqlModel_->record(i_).value("ST_IDENT").toString();
    QString st_regdate_ = sqlModel_->record(i_).value("ST_RECDATE").toString();
    QString st_operator_ =
      sqlModel_->record(i_).value("ST_OPERATOR").toString();
    QString st_depart_ = sqlModel_->record(i_).value("ST_DEPART").toString();
    QString st_docdate_ = sqlModel_->record(i_).value("ST_DOCDATE").toString();
    QString st_number_ = sqlModel_->record(i_).value("ST_NUMBER").toString();
    QString st_value_ = sqlModel_->record(i_).value("ST_VALUE").toString();
    QString st_file_ = sqlModel_->record(i_).value("ST_FILE").toString();
    QString link_name_ = sqlModel_->record(i_).value("LINK_NAME").toString();
    grandTotal_ += st_value_.toFloat();

    QString l1_ =
      tr("Ident   : %0 Cadastro: %1").arg(st_ident_, 10, ' ').arg(st_regdate_);
    QString l2_ = tr("Operador: %0").arg(st_operator_);
    QString l21_ = tr("N.Fiscal: %0").arg(st_number_);
    QString l22_ = tr("Emissão : %0").arg(st_docdate_);
    QString l23_ =
      tr("Valor R$: %0").arg(QString::number(st_value_.toFloat(), 'f', 2));
    QString l3_ = tr("Depart. : %0").arg(st_depart_);
    QString l4_ = tr("Arquivo : %0").arg(st_file_);
    QString l6_ = tr("Vínculo : %0").arg(link_name_);

    if (y_ < pageHeight_ - 1000) {
      if (i_ == 0) {
        painter_.drawText(
          QPointF(leftMargin_,
                  y_ += getFontRectHeight(fontMetrics_, baseEntity_)),
          baseEntity_);
        y_ += getFontRectHeight(fontMetrics_, baseEntity_);
      }

      painter_.drawText(
        QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, l1_)), l1_);
      painter_.drawText(
        QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, l6_)), l6_);
      painter_.drawText(
        QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, l2_)), l2_);
      const qreal x_ = pageWidth() - 2500.0;
      painter_.drawText(QPointF(x_, y_), l21_);
      painter_.drawText(
        QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, l3_)), l3_);
      painter_.drawText(QPointF(x_, y_), l22_);
      painter_.drawText(
        QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, l4_)), l4_);
      painter_.drawText(QPointF(x_, y_), l23_);

      painter_.setPen(
        QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      painter_.drawLine(QLineF(leftMargin_, y_ + 50, pageWidth(), y_ + 50));
    }
    if (y_ >= pageHeight_ - 1000) {
      printer->newPage();
      y_ = printDefaultHeader(++npag_);
      painter_.setFont(font_);
      painter_.drawText(
        QPointF(leftMargin_,
                y_ += getFontRectHeight(fontMetrics_, baseEntity_)),
        baseEntity_);
      y_ += getFontRectHeight(fontMetrics_, baseEntity_);
    }
  }

  if (grandTotal_ > 0.0) {
    const QString totalDocs_ =
      QString(tr("Total de Documentos...: %L0")).arg(sqlModel_->rowCount());
    const QString totalValues_ = QString(tr("Total Geral.........R$: %L0"))
                                   .arg(QString::number(grandTotal_, 'f', 2));
    printer->newPage();
    y_ = printDefaultHeader(++npag_);
    painter_.setFont(font_);
    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, baseEntity_)),
      baseEntity_);
    y_ += getFontRectHeight(fontMetrics_, baseEntity_) * 2;
    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, baseEntity_)),
      tr("* RESUMO GERAL *"));
    y_ += getFontRectHeight(fontMetrics_, baseEntity_) * 2;
    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, baseEntity_)),
      totalDocs_);
    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, baseEntity_)),
      totalValues_);
  }

  painter_.end();
  progress.setValue(sqlModel_->rowCount() - 1);
  QApplication::restoreOverrideCursor();
}
