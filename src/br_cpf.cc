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

#include "br_cpf.h"

BR_CPF::BR_CPF() {}

/*!
 * \brief BR_CPF::data
 * Returns the CPF value passed via the type's constructor.
 * Allows the user to choose between two formats
 * of presentation
 * \param format
 * \return QString(CPF)
 */
const QString
BR_CPF::data(DataFormats format_)
{
  if (valid_) {
    stDelimiter stDel_[][2]{ { 3, "." }, { 7, "." }, { 11, "-" } };
    switch (format_) {
      case NumericOnly:
        if (valid_) {
          if (data_.count(".") == 2 && data_.count("-") == 1) {
            data_.replace(".", "").replace("-", "");
          }
        }
        break;
      case Delimited:
        if (data_.length() == 11) {
          for (int i_ = 0; i_ < 3; i_++) {
            data_.insert(stDel_[i_][0].pos_, stDel_[i_][0].delemiter_);
          }
        }
    }
  } // _valid

  return data_;
}

/*!
 * \brief BR_CPF::VD
 * \return integer representing VD
 */
const int
BR_CPF::VD()
{
  return (valid_ ? calcVD() : -1);
}

/*!
 * \brief BR_CPF::section
 * Returns the significant parts of the data via a structure.
 *
 * Members:
 * first = Registration Number
 * vd = Value of the verifying digit
 *
 * \return struct Section
 */
BR_CPF::Section
BR_CPF::section()
{
  Section sec_;
  if (valid_) {
    int len_ = std::move(data_.length() - 2);
    sec_.first_ = std::move(data_.left(len_));
    sec_.vd_ = data_.right(2);
  } else {
    sec_.first_ = "";
    sec_.vd_ = "";
  }
  return sec_;
}

/*!
 * \protected
 * \brief BR_CPF::normalize
 *
 * Performs several checks with the entered value
 * in order to ensure that the value is valid for
 * be calculated. In general try to make sure
 * the value, and if it is correct, convert it to the
 * internal format to be calculated.
 */
void
BR_CPF::normalize()
{
  if (data_.isEmpty()) {
    valid_ = false;
    vd_ = -1;
    return;
  }

  if (data_.length() < 11) {
    valid_ = false;
    vd_ = -1;
    return;
  }

  /* tests whether the passed cpf is composed of
   * numbers in the range from 00000000000 to 99999999999
   */
  for (int i = 0; i < data_.length(); i++) {
    if (data_.count(QString::number(i)) == data_.length()) {
      valid_ = false;
      data_ = "BR_CPF::invalid";
      return;
    }
  }

  if (data_.length() < 9 || data_.length() > 14) {
    valid_ = false;
    data_ = "BR_CPF::invalid";
    return;
  }

  /* Checks if it was passed formatted or not */
  if (data_.contains(".") && data_.contains("-")) {
    if (data_.count(".") == 2 && data_.count("-") == 1) { // right
      valid_ = true;
      data_.replace(".", "").replace("-", "");
    } else { // wrong
      valid_ = false;
      data_ = "BR_CPF::invalid";
    }
  }
  valid_ = true;
}

/*!
 * \protected
 * \brief BR_CPF::calcVD
 * \return
 */
const int
BR_CPF::calcVD() const
{
  if (valid_) {
    const int len_ = std::move(data_.length());
    QString aux_ = std::move(data_.left(len_ - 2));

    // seed
    int weight1_[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2 };
    int weight2_[11] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 };

    /* DV 1 */
    int p_[10];
    int sum_{ 0 };
    for (int i_ = 0; i_ < 10; i_++) {
      p_[i_] = std::move(aux_.mid(i_, 1).toInt() * weight1_[i_]);
      sum_ += std::move(p_[i_]);
    }
    int mod1_ = std::move(sum_ % 11);
    int dig1_ = 0;
    if (mod1_ < 2) {
      dig1_ = 0;
    } else {
      dig1_ = std::move(11 - mod1_);
    }

    /* DV 2 */
    aux_ += std::move(QString::number(dig1_)); // join VD 1
    sum_ = 0;
    int p1_[11]; // Resolve bug de loop infinito
    for (int i_ = 0; i_ < 11; i_++) {
      p1_[i_] = std::move(aux_.mid(i_, 1).toInt() * weight2_[i_]);
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
  return -1;
}
