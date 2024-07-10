/***************************************************************************
 * Copyright (c) 2007-2024                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase6 project, a images/documents           *
 * database programdl_.                                                       *
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

#include "printdocanalitic.h"
#include "globals.h"

/*!
 * \brief PrintDocAnalitic::printReport
 * \param prt_
 */
void
PrintDocAnalitic::printReport(QPrinter* prt_)
{
  painter_.begin(prt_);

  QFont font_("Courier New", 10);
  font_ = QFont(font_, painter_.device());
  QFontMetricsF fontMetrics_(font_);

  setColumnHeaders();

  int npag_{ 1 };
  qreal y_{ 0 };
  y_ = printDefaultHeader(npag_);

  const qreal leftMargin_ = getLeftMargin();
  qreal pageHeigth_ = pageHeight();

  painter_.setFont(font_);

  QProgressDialog progress(
    tr("Gerando Relatório ..."), tr("Cancelar"), 0, sqlModel_->rowCount() - 1);
  progress.setWindowModality(Qt::ApplicationModal);

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  for (int i_ = 0; i_ <= sqlModel_->rowCount(); ++i_) {
    if (sqlModel_->record(i_).value("ST_ETRELA").toInt() == 0) {
      continue;
    }

    qApp->processEvents();
    if (progress.wasCanceled()) {
      break;
    }
    progress.setValue(i_);

    QString relaNome =
      tr("Entidade: ") + sqlModel_->record(0).value("ENTITY").toString();
    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, relaNome)),
      relaNome);

    const QString stmnt_{
      QString(
        "SELECT HIGH_PRIORITY ST_IDENT, ET_IDENT,"
        "DATE_FORMAT(ST_DOCDATE,'%d/%m/%Y') AS ST_DOCDATE, "
        "ST_NUMBER, ST_VALUE, "
        "(SELECT A.ET_NOME FROM  DOCENTITY A WHERE A.ET_IDENT=B.ST_ETRELA) AS "
        "LINK_NAME, "
        "(SELECT A.ET_CATEG FROM DOCENTITY A WHERE A.ET_IDENT=B.ST_ETRELA) AS "
        "ET_CATEG "
        "FROM DOCSTORAGE B WHERE ET_IDENT=%0 AND ST_ETRELA=%1 AND ST_VALUE > "
        "0.0 %2")
        .arg(sqlModel_->record(i_).value("ET_IDENT").toString())
        .arg(sqlModel_->record(i_).value("ST_ETRELA").toString())
        .arg(where_clause_)
    };

    QSqlQueryModel mdl_;
    mdl_.setQuery(stmnt_);
    if (mdl_.rowCount() == 0) {
      continue;
    }

    double sum_{ 0.0 };

    QString reportName_{ tr("Categoria: ") };
    if (mdl_.record(0).value("ET_CATEG").toInt() == 0) {
      reportName_ += tr("Empresa");
    } else if (mdl_.record(0).value("ET_CATEG").toInt() == 1) {
      reportName_ += tr("Cliente");
    } else if (mdl_.record(0).value("ET_CATEG").toInt() == 2) {
      reportName_ += tr("Fornecedor");
    } else if (mdl_.record(0).value("ET_CATEG").toInt() == 3) {
      reportName_ += tr("Projeto");
    } else if (mdl_.record(0).value("ET_CATEG").toInt() == 4) {
      reportName_ += tr("Personalizado");
    } else if (mdl_.record(0).value("ET_CATEG").toInt() == 5) {
      reportName_ += tr("Basico");
    }
    reportName_ +=
      " " + tr("Vinculo: ") + mdl_.record(0).value("LINK_NAME").toString();
    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, reportName_)),
      "   " + reportName_);

    const QString subTitle_ =
      QString(40, ' ') + tr("ID\tDocumento\tData Emissão\tValor %0")
                           .arg(Globals::currencySimbol());
    const QString subTitleSep_ =
      QString(40, ' ') + "---------- ---------- ------------ --------------";

    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, subTitle_)),
      subTitle_);
    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, subTitleSep_)),
      subTitleSep_);

    int lineCount_{ 0 };
    for (int j_ = 0; j_ <= mdl_.rowCount(); ++j_) {
      QString c0_ = QString("%0")
                      .arg(mdl_.record(j_).value("ST_IDENT").toString())
                      .leftJustified(10, ' ');
      QString c1_ = QString("%0")
                      .arg(mdl_.record(j_).value("ST_NUMBER").toString())
                      .rightJustified(10, ' ');
      QString c2_ = QString("%0")
                      .arg(mdl_.record(j_).value("ST_DOCDATE").toString())
                      .leftJustified(10, ' ');
      double c3_ = mdl_.record(j_).value("ST_VALUE").toFloat();
      QString detail = QString(40, ' ') + QString("%0 %1 %2   %L3")
                                            .arg(c0_)
                                            .arg(c1_)
                                            .arg(c2_)
                                            .arg(c3_, 14, 'f', 2);
      sum_ += mdl_.record(j_).value("ST_VALUE").toFloat();

      if (c3_ > 0.0) {
        painter_.drawText(
          QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, detail)),
          detail);
      }
      ++lineCount_;
      if (lineCount_ == 55) {
        printer_->newPage();
        y_ = printDefaultHeader(++npag_);
        painter_.setFont(font_);
        painter_.drawText(
          QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, relaNome)),
          relaNome);
        painter_.drawText(
          QPointF(leftMargin_,
                  y_ += getFontRectHeight(fontMetrics_, reportName_)),
          "   " + reportName_);
        lineCount_ = 0;
      }
    }
    const QString qsTotal = QString(55, ' ') + QString("=======> Total R$");
    QString qssum_ = QString("%L0").arg(sum_, 14, 'f', 2);
    qssum_ = QString(75, ' ') + qssum_.rightJustified(14, ' ');
    painter_.drawText(
      QPointF(leftMargin_, y_ += getFontRectHeight(fontMetrics_, qsTotal)),
      qsTotal);
    painter_.drawText(QPointF(leftMargin_, y_), qssum_);

    if (y_ < pageHeigth_ - 1000) {
      printer_->newPage();
      y_ = printDefaultHeader(++npag_);
      painter_.setFont(font_);
    }
  }
  painter_.end();
  progress.setValue(sqlModel_->rowCount() - 1);
  QApplication::restoreOverrideCursor();
}
