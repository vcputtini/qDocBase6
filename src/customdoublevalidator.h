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

#ifndef CUSTOMDOUBLEVALIDATOR_H
#define CUSTOMDOUBLEVALIDATOR_H

#include <QDoubleValidator>
#include <QObject>
#include <QString>

#include <QtMath>

#include <climits>
#include <cmath>

#ifndef LLONG_MAX
#define LLONG_MAX Q_INT64_C(0x7fffffffffffffff)
#endif

using namespace std;

/*!
 * \class CustomDoubleValidator
 * \brief Valida corretamente a digitacao de numero com ponto decimal
 *
 * It ensures that the top, bottom and decimals parameters are strictly
 * respected when typing the decimal number. In other words, if a number is
 * defined as: bottom=0.0 top=9999.99 decimals=2 it will allow the number to be
 * entered only within the established limits. in the format 9999.99 as given in
 * this example.
 *
 */
class CustomDoubleValidator : public QDoubleValidator
{
  Q_OBJECT
public:
  CustomDoubleValidator(double bottom_,
                        double top_,
                        int decimals_,
                        QObject* parent_ = nullptr);

  virtual QValidator::State validate(QString& input_, int& pos_) const;

private:
  enum NumberMode
  {
    IntegerMode,
    DoubleStandardMode,
    DoubleScientificMode
  };
  bool validateChars(const QString& str_,
                     NumberMode numMode_,
                     QByteArray* buff_,
                     int decDigits_) const;

  inline int numDigits(qlonglong n_) const;
  inline char digitToCLocale(const QChar& in_) const;

  qlonglong pow10(int n_) const;
};

#endif // CUSTOMDOUBLEVALIDATOR_H
