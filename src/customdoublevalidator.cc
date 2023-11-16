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

#include "customdoublevalidator.h"

#include <climits>

CustomDoubleValidator::CustomDoubleValidator(double bottom_,
                                             double top_,
                                             int decimals_,
                                             QObject* parent_)
  : QDoubleValidator(bottom_, top_, decimals_, parent_)
{
}

QValidator::State
CustomDoubleValidator::validate(QString& input_, int& pos_) const
{
  QString buff(input_);

  if (buff.isEmpty()) {
    return QValidator::Acceptable;
  }

  double t_{ 0.0 };
  double b_{ 0.0 };
  double d_{ 0.0 };

  QByteArray abuff;
  if (!validateChars(input_, DoubleStandardMode, &abuff, decimals())) {
    return Invalid;
  }

  // t = buff.section('.', 0, 0).length();
  d_ = buff.section('.', 1, 2).length();

  if (bottom() >= 0 && buff.startsWith('-')) {
    return Invalid;
  }

  if (top() < 0 && buff.startsWith('+')) {
    return Invalid;
  }

  if (d_ > decimals()) {
    return QValidator::Invalid;
  }

  if (buff.toDouble() < bottom() || buff.toDouble() > top()) {
    return QValidator::Invalid;
  }

  /*
   * Test the limits of number
   * ex: if the maximum is 999999999.99, it allows you to type up to
   *  999999999.00
   */
  double i = buff.toDouble();
  b_ = bottom();
  t_ = top();

  if (notation() == StandardNotation) {
    double max = qMax(qAbs(b_), qAbs(t_));
    if (max < LLONG_MAX) {
      double n = qPow(numDigits(qlonglong(max)), 10);
      if (qAbs(i) > n)
        return Invalid;
    }
  }

  return QValidator::Intermediate;
}

/**
 * @bried Retorna o numero de digitos de um dado numero inteiro
 *
 */
int
CustomDoubleValidator::numDigits(qlonglong n) const
{
  return floor(log10(abs(n != 0 ? n : 1))) + 1;
}

/**
 * @bried Adaptação do codigo original dos fontes da Nokia Qt Framework Qt 4.7.1
 *        a fim de suprir algumas necessidades especificas
 *        O codigo foi alterado para minhas necessidades particularidades
 */
bool
CustomDoubleValidator::validateChars(const QString& str_,
                                     NumberMode numMode_,
                                     QByteArray* buff_,
                                     int decDigits_) const
{
  buff_->clear();
  buff_->reserve(str_.length());

  const bool scientific_ = numMode_ == DoubleScientificMode;
  bool lastWasE_{ false };
  bool lastWasDigit_{ false };
  int decPointCnt_{ 0 };
  bool dec_{ false };
  int decDigitCnt_{ 0 };

  for (int i = 0; i < str_.length(); ++i) {
    char c_ = digitToCLocale(str_.at(i));

    if (c_ >= '0' && c_ <= '9') {
      if (numMode_ != IntegerMode) {
        // If a double has too many digits after decpt, it shall be Invalid.
        if (dec_ && decDigits_ != -1 && decDigits_ < ++decDigitCnt_)
          return false;
      }
      lastWasDigit_ = true;
    } else {
      switch (c_) {
        case ',': // nao faz nada. A virgula DEVE ter o mesmo comportamento do
                  // ponto decimal
        case '.':
          if (numMode_ == IntegerMode) {
            // If an integer has a decimal point, it shall be Invalid.
            return false;
          } else {
            // If a double has more than one decimal point, it shall be Invalid.
            if (++decPointCnt_ > 1) {
              return false;
            }
#if 0
                        // If a double with no decimal digits has a decimal point, it shall be
                        // Invalid.
                        if (decDigits == 0)
                            return false;
#endif // On second thoughts, it shall be Valid.

            dec_ = true;
          }
          break;

        case '+':
        case '-':
          if (&scientific) {
            // If a scientific has a sign that's not at the beginning or after
            // an 'e', it shall be Invalid.
            if (i != 0 && !lastWasE_) {
              return false;
            }
          } else {
            // If a non-scientific has a sign that's not at the beginning,
            // it shall be Invalid.
            if (i != 0) {
              return false;
            }
          }
          break;
        default:
          // If it's not a valid digit, it shall be Invalid.
          return false;
      }
      lastWasDigit_ = false;
    }

    lastWasE_ = c_ == 'e';
    if (c_ != ',') {
      buff_->append(c_);
    }
  }

  return true;
}

/*!
 * \brief Adaptation of the original source code from Nokia Qt Framework
 * Qt 4.7.1 in order to meet some specific needs.
 */
char
CustomDoubleValidator::digitToCLocale(const QChar& in_) const
{
  const QChar zero_ = '0';
  const QChar group_ = ',';
  const ushort zeroUnicode_ = zero_.unicode();
  const ushort tenUnicode_ = zeroUnicode_ + 10;

  if (in_.unicode() >= zeroUnicode_ && in_.unicode() < tenUnicode_) {
    return '0' + in_.unicode() - zeroUnicode_;
  }

  if (in_.unicode() >= '0' && in_.unicode() <= '9') {
    return in_.toLatin1();
  }

  if (in_ == '+') {
    return '+';
  }

  if (in_ == '-') {
    return '-';
  }

  if (in_ == '.') {
    return '.';
  }

  if (in_ == ',') {
    return ',';
  }

  // if (in == exponential() || in == exponential().toUpper())
  //     return 'e';

  // In several languages group() is the char 0xA0, which looks like a space.
  // People use a regular space instead of it and complain it doesn't work.
  if (group_.unicode() == 0xA0 && in_.unicode() == ' ') {
    return ',';
  }

  return 0;
}

/**
 * @bried Funcao para calculo de uma potencia de 10
 */
qlonglong
CustomDoubleValidator::pow10(int n_) const
{
  qlonglong result = 1;
  for (int i = 0; i < n_; ++i)
    result *= 10;
  return result;
}
