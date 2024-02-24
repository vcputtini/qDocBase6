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

#ifndef MULTIFILES_H
#define MULTIFILES_H

#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QFuture>
#include <QObject>
#include <QPair>
#include <QString>
#include <QtConcurrentRun>

#include <QDebug>

#include "messages.h"

/*!
 * \brief The MultiFilesRead_P class
 */
class MultiFilesRead_P
{
  typedef QPair<int, QByteArray> pair_t_;

public:
  pair_t_ readFile(const QString fname_);

private:
  QByteArray ba_;
};

/*!
 * \brief The MultiFilesWrite_P class
 */
class MultiFilesWrite_P
{
public:
  int writeFile(QByteArray baOut_, const QString fname_);
};

/* ------------------------------------------------------------------------ */

/*!
 * \brief The MuiltFiles class
 */
class MultiFiles : public QFile
{
  Q_OBJECT

  typedef QPair<int, QByteArray> pair_t_;

public:
  explicit MultiFiles(QObject* parent = nullptr);
  ~MultiFiles();

  void setInputFile(const QString fIn_ = nullptr);
  void setOutputFile(const QString fOut_ = nullptr);
  void setFileName(const QString& fname_ = nullptr);
  const int read();
  int write(QByteArray baOut_);
  void execute();

  void setData(QByteArray ba_);
  QByteArray data();
  qint64 bytesWrite() const;

  qint64 sizeFileInout() const;
  qint64 sizefileOutput() const;
  bool fileExists(const QString fname_ = nullptr);

private:
  MessagesNS::Messages* Messages_;

  QByteArray ba_;
  QByteArray baData_;

  qint64 bytesWrite_;

  QString fInput_;
  QString fOutput_;

  MultiFilesRead_P MultiFilesRead_p;
  MultiFilesWrite_P MultiFilesWrite_p;

  pair_t_ (MultiFilesRead_P::*pt2read)(QString);
  int (MultiFilesWrite_P::*pt2write)(QByteArray, QString);

  bool fileError(const int fe_);

signals:
  void errorText(const QString&);
};

#endif
