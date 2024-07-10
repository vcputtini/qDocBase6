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
#include "abstractprint.h"

/*!
 * \brief AbstractPrint::AbstractPrint
 * \param parent
 */
AbstractPrint::AbstractPrint(QObject* parent)
  : QObject{ parent }
  , printer_{ new QPrinter(QPrinter::HighResolution) }
  , sqlModel_{ new QSqlQueryModel }
  , widget_{ nullptr }
  , rowsCount_{ 0 }
  , leftMargin_{ 10 }
  , rightMargin_{ 10 }
  , topMargin_{ 10 }
  , bottomMargin_{ 10 }
  , dataStringList_{}
  , strQuery_{}
  , subSqlQuery_{}
{
}

/*!
 * \brief AbstractPrint::~AbstractPrint
 */
AbstractPrint::~AbstractPrint()
{
  delete sqlModel_;
  delete printer_;
}

// ========================================================================

/*!
 * \pure
 * \brief AbstractPrint::printReport
 */
void
AbstractPrint::printReport(QPrinter* prt_)
{
  if (painter_.begin(prt_)) {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QFont font_("Courier New", 10);
    font_ = QFont(font_, painter_.device());
    QFontMetricsF fontMetricsF_(font_);

    columnHeader();

    qreal pageHeight_ = prt_->pageRect(QPrinter::Point).height() -
                        (prt_->pageRect(QPrinter::Point).height() -
                         prt_->pageRect(QPrinter::Point).height());

    painter_.setFont(font_);

    int nPage_ = 1;
    qreal y_axis_{ 0 };
    y_axis_ = printDefaultHeader(nPage_, true);
    QString text_{};

    for (int index_ = 0; index_ != sqlModel_->rowCount(); ++index_) {
      text_.clear();
      for (int col_ = 0; col_ < sqlModel_->columnCount(); ++col_) {
        text_ += sqlModel_->record(index_).value(col_).toString() + '\t';
      }
      QRectF rectF_ = fontMetricsF_.boundingRect(text_);
      y_axis_ += rectF_.height();

      // current page
      if (y_axis_ <= pageHeight_) {
        painter_.drawText(QPointF(leftMargin_, y_axis_), text_);
      } else { /*(y_axis_ > pageHeight_) */
        prt_->newPage();
        y_axis_ = printDefaultHeader(++nPage_, true);
        painter_.setFont(font_);
        painter_.drawText(QPointF(leftMargin_, y_axis_ += rectF_.height()),
                          text_);
      }
    }

    painter_.end();
    // progress
    QApplication::restoreOverrideCursor();
  }
}

// ========================================================================

/*!
 * \public
 * \brief AbstractPrint::preview
 */
void
AbstractPrint::preview()
{
  printer_->setPageOrientation(pgLayout_.orientation());

  QPrintPreviewDialog* d_ = new QPrintPreviewDialog(
    printer_,
    widget_,
    Qt::Widget | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
  d_->setWindowTitle(tr("Visualização de Relatórios"));

  connect(
    d_, SIGNAL(paintRequested(QPrinter*)), this, SLOT(printSlot(QPrinter*)));
  d_->exec();
}

/*!
 * \public
 * \brief AbstractPrint::setOrientation
 * \param pgOrientation_
 */
void
AbstractPrint::setOrientation(const PageOrientation pgOrientation_)
{
  pgLayout_.setOrientation((pgOrientation_ == PageOrientation::Portrait
                              ? QPageLayout::Portrait
                              : QPageLayout::Landscape));
}

void
AbstractPrint::setTitle(const QString& title)
{
  reportTitle_ = std::move(title);
}

void
AbstractPrint::setSubTitle(const QString& subTitle_)
{
  reportSubTitle_ = std::move(subTitle_);
}

void
AbstractPrint::setOutputFilename(QString ofname_)
{
  if (!ofname_.isEmpty()) {
    if (!ofname_.endsWith(".pdf"))
      ofname_ += ".pdf";
    printer_->setOutputFileName(ofname_);
  }
}

const QString
AbstractPrint::title()
{
  return reportTitle_;
}

const QString
AbstractPrint::subTitle()
{
  return reportSubTitle_;
}

const int
AbstractPrint::rowsCount()
{
  return rowsCount_;
}

/*!
 * \public
 * \brief AbstractPrint::pageHeight
 * \return
 */
qreal
AbstractPrint::pageHeight() const
{
  return printer_->height();
}

/*!
 * \public
 * \brief AbstractPrint::pageWidth
 * \return
 */
qreal
AbstractPrint::pageWidth() const
{
  return printer_->width();
}

qreal
AbstractPrint::getFontRectHeight(QFontMetricsF fontMetrics, QString str) const
{
  QRectF rect = fontMetrics.boundingRect(str);
  return rect.height();
}

qreal
AbstractPrint::getFontRectWidth(QFontMetricsF fontMetrics, QString str) const
{
  QRectF rect = fontMetrics.boundingRect(str);
  return rect.width();
}

/*!
 * \public
 * \brief AbstractPrint::setQueryModel
 * \param query_
 */
void
AbstractPrint::setQueryModel(const QString query_)
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  emit message(tr("Aguarde Processando SQL ..."));
  sqlModel_->setQuery(query_);
  if (sqlModel_->lastError().isValid()) {
    emit sqlLastError(tr("#") + sqlModel_->lastError().text());
    rowsCount_ = 0;
    QApplication::restoreOverrideCursor();
    return;
  }
  emit message("");
  rowsCount_ = sqlModel_->rowCount();
  QApplication::restoreOverrideCursor();
}

/*!
 * \public
 * \brief AbstractPrint::setStringList()
 * \notePasses a string list containing values that will be
 * used to generate the report.
 * For example, you can pass the search value of
 * records in the table.
 * E.g.: stringList << "10" << "15" << "23" << "1";
 * \param strList_
 * \param query_
 */
void
AbstractPrint::setStringList(QStringList strLst_, const QString query_)
{
  dataStringList_ = std::move(strLst_);
  strQuery_ = std::move(query_);
}

/*!
 * \brief AbstractPrint::setData
 * \param data_
 */
void
AbstractPrint::setData(QVariant data_)
{
  if (data_.typeId() == QMetaType::QString) {
    subSqlQuery_ = data_.toString();
  } else if (data_.typeId() == QMetaType::QStringList) {
    QStringList list(data_.toStringList());
    for (int i = 0; i < list.size(); ++i) {
      dataStringList_ << list.at(i).toUtf8();
    }
  }
}

/*!
 * \brief AbstractPrint::data
 * \return
 */
QVariant
AbstractPrint::data(void)
{
  if (!subSqlQuery_.isEmpty()) {
    return subSqlQuery_;
  } else if (!dataStringList_.isEmpty()) {
    return dataStringList_;
  }

  return QVariant();
}

/*!
 * \public
 * \brief AbstractPrint::setColumnHeaders
 * \note Passes a list containing the names of the columns that
 * will be printed.
 * If this function is not used, the field names
 * from the SQL table will be used.
 * \param list_
 */
void
AbstractPrint::setColumnHeaders(QStringList list_)
{
  lstHeaders_ = std::move(list_);
}

/*!
 * \private
 * \brief AbstractPrint::columnHeader
 */
void
AbstractPrint::columnHeader()
{
  columnsHeader_.clear();
  if (lstHeaders_.count() == 0) {
    for (int i = 0; i < sqlModel_->columnCount(); ++i) {
      columnsHeader_ += sqlModel_->record(0).fieldName(i) + '\t';
    }
  } else {
    if (lstHeaders_.count() == sqlModel_->columnCount()) {
      for (auto& it_ : lstHeaders_) {
        columnsHeader_ += it_ + '\t';
      }
    }
  }
}

/*!
 * \private
 * \brief AbstractPrint::printDefaultHeader
 * \param page_
 * \param canPrint_ (true | false)
 * \return qreal - The height in pixels occupied by the header.
 */
const qreal
AbstractPrint::printDefaultHeader(const int page_, bool canPrint_)
{

  QFont font14B_("Arial", 14, QFont::Bold);
  QFont font11N_("Arial", 11);
  QFont font8N_("Arial", 8);

  font14B_ = QFont(font14B_, painter_.device());
  QFontMetricsF fontM14B_(font14B_);
  font11N_ = QFont(font11N_, painter_.device());
  QFontMetricsF fontM11N_(font11N_);
  font8N_ = QFont(font8N_, painter_.device());
  //[[maybe_unused]] QFontMetricsF fontM8N_(font8N_);

  painter_.setFont(font14B_);
  painter_.drawText(
    leftMargin_, leftMargin_ + fontM14B_.lineSpacing(), reportTitle_);

  qreal line_ = leftMargin_ + fontM14B_.lineSpacing();

  painter_.setFont(font11N_);
  painter_.drawText(
    leftMargin_, line_ + fontM11N_.lineSpacing(), reportSubTitle_);

  const qreal maxPageWidth_ = pageWidth();

  painter_.setPen(
    QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

  line_ += fontM11N_.lineSpacing() / 2;

  painter_.drawLine(QLineF(leftMargin_,
                           line_ + fontM11N_.lineSpacing(),
                           maxPageWidth_,
                           line_ + fontM11N_.lineSpacing()));

  const QString date_{ QString("%0/%1/%2")
                         .arg(QDate::currentDate().toString("dd"))
                         .arg(QDate::currentDate().toString("MM"))
                         .arg(QDate::currentDate().toString("yyyy")) };
  const QString pageNumber_ = QString::number(page_);
  const QString stamp_ =
    QString("%0-%1-P:%2")
      .arg(date_)
      .arg(QString("%0").arg(QTime::currentTime().toString()))
      .arg(pageNumber_);

  painter_.setFont(font8N_);
  line_ += fontM11N_.lineSpacing();

  const qreal textWidth = fontM11N_.maxWidth() * stamp_.size();

  painter_.drawText(
    QRectF(maxPageWidth_ - textWidth, line_, textWidth, fontM11N_.height()),
    Qt::AlignRight,
    stamp_);

  // Column's Header
  if (canPrint_) {
    line_ += fontM11N_.lineSpacing();
    painter_.setFont(font11N_);
    painter_.drawText(
      leftMargin_, line_ + fontM11N_.lineSpacing(), columnsHeader_);
  }

  return (line_ + fontM11N_.lineSpacing());
}

/*!
 * \public
 * \brief AbstractPrint::fitToPage
 * \param image
 * \param r1
 * \return qreal
 */
const qreal
AbstractPrint::fitToPage(QImage image, QRectF r1)
{
  static qreal factor = 1.0;
  QSizeF size = image.size();
  size.scale(r1.size() * factor, Qt::KeepAspectRatio);
  if (size.height() <= pageHeight() - 300.0 && size.width() <= pageWidth()) {
    return (factor);
  } else {
    if (factor == 0.0)
      return (factor);
    factor -= 0.1;
    factor = fitToPage(image, r1);
  }
  return (factor);
}

/*!
 * \protected SLOT
 * \brief AbstractPrint::printSlot
 * \param prt_
 */
void
AbstractPrint::printSlot(QPrinter* prt_)
{
  printReport(prt_);
}
