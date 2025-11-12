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
#ifndef TAXYPAYREGISTRYBASE_H
#define TAXYPAYREGISTRYBASE_H

#include <QDebug>
#include <QString>

/*!
 * \brief The TaxypayRegistryBase class
 */
class TaxypayRegistryBase
{
public:
  TaxypayRegistryBase();

  enum TRModel
  {
    V1,
    AlphaNumeric
  };

  enum TRFormat
  {
    NumericOnly,
    Delimited
  };

  struct Section
  {
    QString V1_first_;
    QString V1_second_;
    QString V1_dv_;

    QString root_[3]; // SS SSS SSS
    QString order_;   // SSSS
    QString dv_;
  };

  static constexpr int lengthWithoutVD_ = 12;
  static constexpr int lengthWithVD_ = 14;
  static constexpr int formattedLengthWithoutVD_ = 18;
  static constexpr int formattedLengthWithVD_ = 15;

  bool operator==(const TaxypayRegistryBase& other_) const;
  bool operator!=(const TaxypayRegistryBase& other_) const;

  void setData(const QString& other_ = QString());

  TRModel whichCNPJModel() const;

  virtual bool isNullOrEmpty();
  virtual bool isValid();
  virtual const QString data(TRFormat format_ = TRFormat::Delimited) = 0;
  virtual const int VD() = 0;
  virtual Section section() = 0;

private:
protected:
  QString data_;
  int vd_;
  bool valid_;
  struct stDelimiter
  {
    int pos_;
    QString delemiter_;
  };

  virtual void normalize() = 0;
  virtual const int calcVD() const = 0;
};

#endif // TAXYPAYREGISTRYBASE_H
