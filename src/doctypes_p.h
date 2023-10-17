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
#ifndef DOCTYPES_P_H
#define DOCTYPES_P_H

#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QStringList>
#include <QWidget>

namespace Ui {
class DocTypes_P;
}

/*!
 * \brief The DocTypes_P class
 *
 * Promoted class from combobox_DocType
 */
class DocTypes_P : public QComboBox
{
  Q_OBJECT
public:
  explicit DocTypes_P(QWidget* parent = nullptr);
  ~DocTypes_P();

  const int docIndex() const;

private:
  QLineEdit* lineEdit_Filter_;
  void defaultItems();

  static constexpr const char* CHOICE_VOID = "";
  static constexpr const char* CHOICE_1 = QT_TR_NOOP("BR CNPJ");
  static constexpr const char* CHOICE_2 = QT_TR_NOOP("BR I.E.");
  static constexpr const char* CHOICE_3 = QT_TR_NOOP("BR I.M.");
  static constexpr const char* CHOICE_4 = QT_TR_NOOP("BR CPF");
  static constexpr const char* CHOICE_5 = QT_TR_NOOP("BR RG");
  static constexpr const char* CHOICE_6 = QT_TR_NOOP("Outros");
};

#endif // DOCTYPES_P_H
