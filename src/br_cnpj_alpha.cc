/***************************************************************************
 * Copyright (c) 2025                                                *
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
#include "br_cnpj_alpha.h"

BR_CNPJ_ALPHA::BR_CNPJ_ALPHA() {}

/*!
 * \brief BR_CNPJ_ALPHA::data
 * \param format_
 * \return
 */
const QString
BR_CNPJ_ALPHA::data(TRFormat format_)
{
  return QString();
}

/*!
 * \public
 * \brief BR_CNPJ_ALPHA::VD
 * \return
 */
const int
BR_CNPJ_ALPHA::VD()
{
  return calcVD();
}

/*!
 * \public
 * \brief BR_CNPJ_ALPHA::section
 * \return
 */
BR_CNPJ_ALPHA::Section
BR_CNPJ_ALPHA::section()
{
  Section sec;
  return sec;
}

bool
BR_CNPJ_ALPHA::isNullOrEmpty()
{
  return true;
}

void
BR_CNPJ_ALPHA::normalize()
{
}

/*!
 * \private
 * \brief BR_CNPJ_ALPHA::charToValue
 * \param c
 * \return
 */
int
BR_CNPJ_ALPHA::charToValue(QChar c) const
{
  if (c.isDigit()) {
    return c.digitValue();
  } else if (c.isLetter()) {
    // A=17, B=18,... Z=42
    return c.toUpper().unicode() - 48;
  } else {
    // valid_ = false;
    // data_ = "Caractere inválido no CNPJ alfanumérico.";
  }
  return '#'; // Invalid
}

/*!
 * \private
 * \brief BR_CNPJ_ALPHA::removeMask
 * \param cnpj_
 * \return
 */
QString
BR_CNPJ_ALPHA::removeMask(QString cnpj_) const
{
  return cnpj_.remove(QRegularExpression("[./-]"));
}

/*!
 * \protected
 * \brief BR_CNPJ_ALPHA::calcVD
 * \return
 */
const int
BR_CNPJ_ALPHA::calcVD() const
{
  QString cnpj = removeMask(data_);

  if (cnpj.length() != lengthWithoutVD_) {
    throw std::invalid_argument(
      "CNPJ deve ter 12 caracteres alfanuméricos para cálculo de DV.");
  }

  QVector<int> values_;
  values_.reserve(lengthWithoutVD_ + 1);

  for (int i = 0; i < lengthWithoutVD_; ++i) {
    values_.append(charToValue(cnpj[i]));
  }

  int dv1_ = calculateVD(values_.constData(), lengthWithoutVD_);
  values_.append(dv1_);

  int dv2_ = calculateVD(values_.constData(), lengthWithoutVD_ + 1);

  // qDebug() << dv1_ << "  " << dv2_ << QString::number(dv1_);
  QString retVD_ = QString::number(dv1_) + QString::number(dv2_);

  return retVD_.toInt();
}

/*!
 * \private
 * \brief BR_CNPJ_ALPHA::calculateVD
 * \param values_
 * \param len_
 * \return
 */
int
BR_CNPJ_ALPHA::calculateVD(const int* values_, int len_) const
{
  int sum_ = 0;
  // Weights do not start at zero (index 0 ignored)
  for (int i = 0; i < len_; ++i) {
    sum_ += values_[i] * weights[i + 1];
  }
  int mod_ = sum_ % 11;
  return (mod_ < 2) ? 0 : 11 - mod_;
}
