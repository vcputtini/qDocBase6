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
#ifndef ABSTRACTPRINT_H
#define ABSTRACTPRINT_H

#include <QApplication>
#include <QDate>
#include <QFont>
#include <QFontMetricsF>
#include <QMessageBox>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QPrintPreviewDialog>
#include <QPrintPreviewWidget>
#include <QPrinter>
#include <QProgressDialog>
#include <QRectF>
#include <QSizeF>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVariant>
#include <QWidget>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>

#include <QDebug>

/*!
 * \brief The AbstractPrint class
 */
class AbstractPrint : public QObject
{
  Q_OBJECT
public:
  explicit AbstractPrint(QObject* parent = nullptr);
  ~AbstractPrint();

  virtual void printReport(QPrinter* prt_ = nullptr) = 0;
  void preview();

  enum class PageOrientation
  {
    Portrait = 0x00,
    Landscape = 0x01
  };
  void setOrientation(
    const PageOrientation pgOrientation_ = PageOrientation::Portrait);

  void setTitle(const QString& title_ = QString());
  void setSubTitle(const QString& subTitle_ = QString());
  void setOutputFilename(QString ofname_ = QString());

  const QString title();
  const QString subTitle();
  const int rowsCount();

  qreal pageHeight() const;
  qreal pageWidth() const;
  qreal getFontRectHeight(QFontMetricsF fontMetrics, QString str) const;
  qreal getFontRectWidth(QFontMetricsF fontMetrics, QString str) const;

  inline qreal getLeftMargin() const { return (leftMargin_); }
  inline qreal getRightMargin() const { return (rightMargin_); }
  inline qreal getTopMargin() const { return (topMargin_); }
  inline qreal getBottomMargin() const { return (bottomMargin_); }

  const qreal fitToPage(QImage image, QRectF r1);

  void setQueryModel(const QString query_ = nullptr);
  void setStringList(QStringList strLst_ = QStringList(),
                     const QString query_ = QString());

  void setData(QVariant data_);
  QVariant data(void);

  void setColumnHeaders(QStringList list_ = QStringList());

private:
  qreal leftMargin_;
  qreal rightMargin_;
  qreal topMargin_;
  qreal bottomMargin_;
  int rowsCount_;

  QStringList lstHeaders_;

  QString columnsHeader_;
  QString reportTitle_;
  QString reportSubTitle_;

private slots:
  void printSlot(QPrinter* prt_);

protected:
  QSqlQueryModel* sqlModel_;

  QWidget* widget_;
  QPrinter* printer_;
  QPageLayout pgLayout_;
  QPainter painter_;

  // variables used to process individual queries
  QStringList dataStringList_; // List containing values for searching records
  QString strQuery_;           // List containing values for searching records
  QString subSqlQuery_;

  const qreal printDefaultHeader(const int page_ = 0, bool canPrint_ = false);
  void columnHeader();

signals:
  void sqlLastError(const QString& e_);
  void message(const QString& m_);
};

#endif // ABSTRACTPRINT_H
