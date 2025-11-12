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
#include "taxypayregistrybase.h"

TaxypayRegistryBase::TaxypayRegistryBase() {}

/*!
 * \brief TaxypayRegistryBase::setData
 * \param other_
 */
void
TaxypayRegistryBase::setData(const QString& other_)
{
  data_ = std::move(other_);
  if (!isNullOrEmpty()) {
    normalize();
  }
}

// operator overload

/*!
 * \overload
 * \brief TaxypayRegistryBase::operator==()
 * This overload compares byte for byte between
 * two data.
 * If any of the bytes are different
 * stops processing and returns false
 * \param other_
 * \return true | false
 */
bool
TaxypayRegistryBase::operator==(const TaxypayRegistryBase& other_) const
{
  if (valid_) {
    const QString temp_ = std::move(data_);
    for (int i = 0; i < temp_.size(); i++) {
      if (temp_[i] != other_.data_[i]) {
        return false;
      }
    }
  }
  return true;
}

/*!
 * \overload
 * \brief TaxypayRegistryBase::operator!=()
 * \param other_
 * \return true | false
 */
bool
TaxypayRegistryBase::operator!=(const TaxypayRegistryBase& other_) const
{
  return !(*this == other_);
}

/*!
 * \public
 * \brief TaxypayRegistryBase::isValid
 * \return
 */
bool
TaxypayRegistryBase::isValid()
{
  if (valid_) {
    if (int vd_ = data_.right(2).toInt(); vd_ != calcVD()) {
      return false;
    }
  }
  return valid_;
}

TaxypayRegistryBase::TRModel
TaxypayRegistryBase::whichCNPJModel() const
{
  return data_.contains("A") ? TRModel::AlphaNumeric : TRModel::V1;
}

/*!
 * \public
 * \brief TaxypayRegistryBase::isNullOrEmpty
 * \note Should be reimplemented
 * \return
 */
bool
TaxypayRegistryBase::isNullOrEmpty()
{
  if (data_ == "../-") {
    return true;
  }
  return false;
}
