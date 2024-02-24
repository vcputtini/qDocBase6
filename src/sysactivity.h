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

#ifndef SYSACTIVITY_H
#define SYSACTIVITY_H

#include <QApplication>
#include <QObject>
#include <QWidget>

#include <QInputDialog>
#include <QMessageBox>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql>

#include <QDate>
#include <QHostInfo>
#include <QString>

#include "globals.h"

/*!
 * \brief TABLE SCHEMA
 *
 *  CREATE TABLE DOCSYSACTIVITY (
 *      ACT_ID          INTEGER         NOT NULL AUT_INCREMENT,
 *      ACT_TIMESTAMP   TIMESTAMP       DEFAULT NULL,
 *      ACT_HOSTNAME    VARCHAR(255)    DEFAULT NULL,
 *      ACT_USER        VARCHAR(30)     DEFAULT NULL,
 *      ACT_MODULE      SMALLINT        DEFAULT -1,
 *      ACT_OPERATION   TINYINT         DEFAULT -1,
 *      ACT_DOCID       bigint(20)  	DEFAULT NULL,
 *      PRIMARY KEY(ACT_ID),
 *      KEY(ACT_TIMESTAMP),
 *      KEY(ACT_USER)
 *  ) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='DOCBASE::OPERATIONS LOG';
 *
 */

/*!
 * \brief The SysActivity class
 * Records activities of system users for audits.
 *
 */
class SysActivity : public QWidget
{
  Q_OBJECT
public:
  explicit SysActivity(QWidget* parent = nullptr);
  ~SysActivity();

public:
  enum class Operations
  {
    Save,
    Update,
    Delete
  };

public:
  bool logger(Globals::ModulesOfApp modules_,
              Operations operations_,
              qint64 docid_ = -1);

private:
  void save(const QString user_,
            Globals::ModulesOfApp modules_,
            Operations operations_,
            qint64 docid_ = -1);

  QString operatorsName_;

signals:
  void lastError(QString);

public slots:
  void inputDialog_operatorSelected(const QString& text_);
};

#endif // SYSACTIVITY_H
