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
#ifndef FORMTOPSECRET2_H
#define FORMTOPSECRET2_H

#include <QApplication>
#include <QDoubleValidator>
#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QWidget>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

// Defined in main.cc: See first.
extern const char* SECURITY_ENCRYPT_KEY_;

namespace Ui {
class FormTopSecret2;
}

/*!
 * \brief The FormTopSecret2 class
 */
class FormTopSecret2 : public QWidget
{
  Q_OBJECT

public:
  explicit FormTopSecret2(QWidget* parent = nullptr);
  ~FormTopSecret2();

private:
  Ui::FormTopSecret2* ui;

  /*!
   * \warning
   * \brief SECPASSWORD
   */
  static constexpr const char* ENCRYPT_KEY_ = "mE025@EZ+3.K";

private:
  QSqlQueryModel* pModel_;
  void reload();

  void clearOperatorsFields();

public slots:
  virtual void pushButton_Release_clicked();
  virtual void pushButton_UpdateAdm_clicked();
  virtual void pushButton_Delete_clicked();
  virtual void pushButton_Update_clicked();
  virtual void pushButton_Save_clicked();
  virtual void tableView_Operators_doubleClicked(QModelIndex idx);
};

#endif // FORMTOPSECRET2_H
