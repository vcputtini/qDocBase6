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
#include "br_cnpj_v1.h"

BR_CNPJ_V1::BR_CNPJ_V1() {}

/*!
 * \brief BR_CNPJ_V1::data
 * Returns the CNPJ value passed via the type's constructor.
 * Allows the user to choose between two formats
 * of presentation
 * \param format
 * \return QString(CNPJ)
 */
const QString
BR_CNPJ_V1::data(TRFormat format_)
{
  if (valid_) {
    stDelimiter stDel_[][2]{ { 2, "." }, { 6, "." }, { 10, "/" }, { 15, "-" } };

    switch (format_) {
      case NumericOnly:
        if (data_.count(".") == 2 && data_.count("/") == 1 &&
            data_.count("-") == 1) {
          data_.replace(".", "").replace("/", "").replace("-", "");
        }
        break;
      case Delimited:
        if (data_.length() == lengthWithVD_) {
          for (int i_ = 0; i_ < 4; i_++) {
            data_.insert(stDel_[i_][0].pos_, stDel_[i_][0].delemiter_);
          }
        }
    }
  }

  return data_;
}

/*!
 * \brief BR_CNPJ::VD
 * \return integer representing VD
 */
const int
BR_CNPJ_V1::VD()
{
  return calcVD();
}

/*!
 * \brief BR_CNPJ::section
 * Returns the significant parts of the CNPJ
 * via a structure.
 *
 * Members:
 * first = Registration Number
 * secont = Head office or branch number
 * vd = verifying digit value
 *
 * \return struct Section
 */
BR_CNPJ_V1::Section
BR_CNPJ_V1::section()
{
  Section sec;
  if (valid_) {
    sec.V1_first_ = std::move(data_.left(8));
    sec.V1_second_ = std::move(data_.mid(8, 4));
    sec.V1_dv_ = std::move(data_.right(2));
  } else {
    sec.V1_first_ = "";
    sec.V1_second_ = "";
    sec.V1_dv_ = "";
  }
  return sec;
}

/*!
 * \protected
 * \brief BR_CNPJ_V1::normalize
 *
 * Performs several checks with the entered value
 * in order to ensure that the value is valid for
 * be calculated. In general try to make sure
 * the value, and if it is correct, convert it to the
 * internal format to be calculated.
 */
void
BR_CNPJ_V1::normalize()
{
  if (isNullOrEmpty()) {
    valid_ = false;
    return;
  }

  /* Tests whether the passed cnpj is composed of
   * numbers in the range of 00000000000000 to 99999999999999
   *                         00111222333344
   */
  for (int i = 0; i < 10; i++) {
    if (data_.count(QString::number(i)) == data_.length()) {
      valid_ = false;
      data_ = "BR_CNPJ::invalid";
      return;
    }
  }

  if (data_.length() < 14 || data_.length() > 18) {
    valid_ = false;
    data_ = "BR_CNPJ::invalid";
    return;
  }

  if (data_.contains(".") && data_.contains("/") &&
      data_.contains("-")) { // perhaps formated
    if (data_.count(".") == 2 && data_.count("/") == 1 &&
        data_.count("-") == 1) { // right
      valid_ = true;
      data_.replace(".", "").replace("/", "").replace("-", "");
      return;
    } else { // wrong
      valid_ = false;
      data_ = "BR_CNPJ::invalid";
      return;
    }
  }

  valid_ = true;
}

/*!
 * \protected
 * \brief CNPJ_V1::calcVD
 * Compute the value of the check digit
 * \return inteiro com o valor do DV
 */
const int
BR_CNPJ_V1::calcVD() const
{
  if (data_.isEmpty()) {
    return false;
  }

  QString aux_ = std::move(data_.left(12));

  // seed
  int weight1_[12] = { 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2 };
  int weight2_[13] = { 6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2 };

  /* DV 1 */
  int p_[12];
  int sum_{ 0 };
  for (int i_ = 0; i_ < 12; i_++) {
    p_[i_] = aux_.mid(i_, 1).toInt() * weight1_[i_];
    sum_ += std::move(p_[i_]);
  }
  int mod1_ = std::move(sum_ % 11);
  int dig1_{ 0 };
  if (mod1_ < 2) {
    dig1_ = 0;
  } else {
    dig1_ = std::move(11 - mod1_);
  }

  /* DV 2 */
  aux_ += std::move(QString::number(dig1_)); // join VD 1
  sum_ = 0;
  int p1_[13];
  for (int i_ = 0; i_ < 13; i_++) {
    p1_[i_] = aux_.mid(i_, 1).toInt() * weight2_[i_];
    sum_ += std::move(p1_[i_]);
  }
  int mod2_ = std::move(sum_ % 11);
  int dig2_ = 0;
  if (mod2_ < 2) {
    dig2_ = 0;
  } else {
    dig2_ = std::move(11 - mod2_);
  }

  return QString("%0%1").arg(dig1_).arg(dig2_).toInt();
}
