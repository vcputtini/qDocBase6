/***************************************************************************
 * Copyright (c) 2007-2024                                                 *
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
#ifndef FORMENTITIES_H
#define FORMENTITIES_H

#include <QAbstractButton>
#include <QApplication>
#include <QCloseEvent>
#include <QComboBox>
#include <QDate>
#include <QFont>
#include <QFontMetricsF>
#include <QIcon>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QModelIndex>
#include <QObject>
#include <QPixmap>
#include <QPrinter>
#include <QProgressDialog>
#include <QPushButton>
#include <QRectF>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QWidget>

#include <QImage>
#include <QPainter>
#include <QPixmap>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>

#include "abstractprint.h"
#include "globals.h"
#include "messages.h"

namespace Ui {
class FormEntities;
}

/*!
 * \brief The FormEntities class
 */
class FormEntities : public QWidget
{
  Q_OBJECT

public:
  explicit FormEntities(QWidget* parent = nullptr);
  ~FormEntities();

private:
  Ui::FormEntities* ui;

private:
  Globals globals;
  MessagesNS::Messages* Messages_;

  enum class ExecSaveOrUpdate
  {
    Save = 0x00,
    Update = 0x01
  };

  enum class TabPages
  {
    PageECS = 0x00,
    PageProject = 0x01,
    PageCustom = 0x02
  };

  enum class Fields
  {
    F_Name,
    F_Alias,
    F_Address,
    F_AddrNumber,
    F_District,
    F_City,
    F_None
  };

  bool isRegActive();
  void setTabCategories(Globals::Categories categ_);
  void enableFields();
  void disableFields();

  void update_data(int state_);
  void execQuery(ExecSaveOrUpdate control_);

  bool fieldChecks();

  static constexpr const char* insert_stmnt_ =
    "INSERT INTO DOCENTITY ( "
    "ET_DATA, ET_ATIVO, ET_CATEG, ET_NOME, ET_ALIAS, "
    "ET_ENDER, ET_ADDRNUMBER, ET_BAIR, ET_CITY, ET_UF, ET_POSTALCODE, "
    "ET_CATPHONE1, ET_TELNUM1, ET_CATPHONE2, ET_TELNUM2, ET_CATPHONE3, "
    "ET_TELNUM3, ET_CATPHONE4, ET_TELNUM4, "
    "ET_CONTATO1, ET_EMAIL1, ET_CONTATO2, ET_EMAIL2, "
    "ET_BR_CPF, ET_BR_RG, ET_BR_CNPJ, ET_BR_IE, ET_BR_IM, ET_OTHERDOCTYPE, "
    "ET_URL, "
    "ET_OBCOD, ET_OBANO, ET_OBCLI, ET_OBIDPROJ, ET_OBCRONO, "
    "ET_OUTESPEC, ET_OUTNFLD1, ET_OUTDFLD1, ET_OUTNFLD2, ET_OUTDFLD2, "
    "ET_OUTOBS, "
    "ET_OBSGER) "
    "VALUES (:ET_DATA, :ET_ATIVO, :ET_CATEG, :ET_NOME, :ET_ALIAS, "
    ":ET_ENDER, :ET_ADDRNUMBER, :ET_BAIR, :ET_CITY, :ET_UF, "
    ":ET_POSTALCODE, "
    ":ET_CATPHONE1, :ET_TELNUM1, :ET_CATPHONE2, :ET_TELNUM2, "
    ":ET_CATPHONE3, :ET_TELNUM3, :ET_CATPHONE4, :ET_TELNUM4,"
    ":ET_CONTATO1, :ET_EMAIL1, :ET_CONTATO2, :ET_EMAIL2, "
    ":ET_BR_CPF, :ET_BR_RG, :ET_BR_CNPJ, :ET_BR_IE, :ET_BR_IM, "
    ":ET_OTHERDOCTYPE, :ET_URL, "
    ":ET_OBCOD, :ET_OBANO, :ET_OBCLI, :ET_OBIDPROJ, :ET_OBCRONO, "
    ":ET_OUTESPEC, :ET_OUTNFLD1, :ET_OUTDFLD1, :ET_OUTNFLD2, :ET_OUTDFLD2, "
    ":ET_OUTOBS, "
    ":ET_OBSGER)";

  static constexpr const char* update_stmnt_ =
    "UPDATE DOCENTITY SET "
    "ET_DATA=:ET_DATA, ET_ATIVO=:ET_ATIVO, ET_CATEG=:ET_CATEG, "
    "ET_NOME=:ET_NOME, ET_ALIAS=:ET_ALIAS, "
    "ET_ENDER=:ET_ENDER, ET_ADDRNUMBER=:ET_ADDRNUMBER, "
    "ET_BAIR=:ET_BAIR, ET_CITY=:ET_CITY, "
    "ET_UF=:ET_UF, ET_POSTALCODE=:ET_POSTALCODE, "
    "ET_CATPHONE1=:ET_CATPHONE1, ET_TELNUM1=:ET_TELNUM1, "
    "ET_CATPHONE2=:ET_CATPHONE2, ET_TELNUM2=:ET_TELNUM2, "
    "ET_CATPHONE3=:ET_CATPHONE3, ET_TELNUM3=:ET_TELNUM3, "
    "ET_CATPHONE4=:ET_CATPHONE4, ET_TELNUM4=:ET_TELNUM4, "
    "ET_CONTATO1=:ET_CONTATO1, ET_EMAIL1=:ET_EMAIL1, "
    "ET_CONTATO2=:ET_CONTATO2, ET_EMAIL2=:ET_EMAIL2, "
    "ET_BR_CPF=:ET_BR_CPF, ET_BR_RG=:ET_BR_RG, ET_BR_CNPJ=:ET_BR_CNPJ, "
    "ET_BR_IE=:ET_BR_IE, "
    "ET_BR_IM=:ET_BR_IM, ET_OTHERDOCTYPE=:ET_OTHERDOCTYPE,"
    "ET_URL=:ET_URL, "
    "ET_OBCOD=:ET_OBCOD, ET_OBANO=:ET_OBANO, "
    "ET_OBCLI=:ET_OBCLI, ET_OBIDPROJ=:ET_OBIDPROJ, "
    "ET_OBCRONO=:ET_OBCRONO, "
    "ET_OUTESPEC=:ET_OUTESPEC, "
    "ET_OUTNFLD1=:ET_OUTNFLD1, ET_OUTDFLD1=:ET_OUTDFLD1, "
    "ET_OUTNFLD2=:ET_OUTNFLD2, ET_OUTDFLD2=:ET_OUTDFLD2, "
    "ET_OUTOBS=:ET_OUTOBS, ET_OBSGER=:ET_OBSGER "
    "WHERE ET_IDENT=:ET_IDENT";

protected:
  virtual void closeEvent(QCloseEvent* event_) override;

public slots:
  virtual void comboBox_Customer_currentIndexChanged(int);
  virtual void tableView_Entities_doubleClicked(QModelIndex index_);
  virtual void toolButton_NicknameSuggestion_clicked();

  void checkBox_ZipCode_Type_stateChanged(int state_);

  void printActionEntities_triggered(bool);
  void printInactiveEntities_triggered(bool);

  void saveRecord();
  void updateRecord();
  void deleteRecord();
  void clearRecord();

  void printRecords(bool isActive_);
  // void printInactiveRecord();

signals:
  void open(bool);
  void closed(bool);
  // void hide(DockType);
  void deleteMenu(Globals::ModulesOfApp module_);
  void stateChanged(int);
};

// --------------------------------------------------------------------------
/*!
 * \brief The PrintActiveEntities class
 */
class PrintActiveEntities : public AbstractPrint
{
  Q_OBJECT
public:
  virtual void printReport(QPrinter* prt_) override;
};

#endif // FORMENTITIES_H
