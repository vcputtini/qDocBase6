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
#ifndef BR_CNPJ_ALPHA_H
#define BR_CNPJ_ALPHA_H

#include <QChar>
#include <QLineEdit>
#include <QObject>
#include <QRegularExpression>
#include <QString>
#include <QValidator>

#include "taxypayregistrybase.h"

/*!
 * \brief The NEW FORMAT of BR_CNPJ_ALPHA class
 *
 * \note In Brazil, the National Register of Legal Entities is a unique number
 * that identifies a legal entity and other types of legal arrangement without
 * legal personality with the Brazilian Federal Revenue Service.
 * (Source:wikipedia)
 *
 * The National Register of Legal Entities is identified by the acronym CNPJ
 * ('C'adastro 'N'acional de 'P'essoa 'J'uridica).
 *
 * New alphanumeric format for CNPJ in Brazil
 *                         NEW MODEL
 *  NUMERICAL ONLY         CNPJ ALPHANUMERIC CNPJ
 *  NN.NNN.NNN/NNNN-NN --> SS.SSS.SSS/SSSS-NN
 *
 */
class BR_CNPJ_ALPHA : public TaxypayRegistryBase
{
public:
  explicit BR_CNPJ_ALPHA();

  const QString data(TRFormat format_ = TRFormat::Delimited) override;
  const int VD() override;
  Section section() override;
  bool isNullOrEmpty() override;

private:
  const int weights[13] = { 0, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2 };

  int calculateVD(const int* values_, int len_) const;
  int charToValue(QChar c) const;
  QString removeMask(QString cnpj_) const;

protected:
  QString VD_;

  void normalize() override;
  const int calcVD() const override;
};

#endif // BR_CNPJ_ALPHA_H
