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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDir>
#include <QSettings>
#include <QString>
#include <QStringList>

#include "globals.h"

/*!
 * \brief The Settings class
 * Read/write parameters and configuration
 */
class Settings
{
public:
  Settings();
  ~Settings();

  const QString getRootDir() const;
  const QString getExportDir() const;
  const QString getTempDir() const;

  bool removeImageFromDir();
  bool keepEntity();

  const int getLivre() const;
  const int getReadOnly() const;
  const int getLimited() const;

  Globals::SupportedDateFormats getDbNativeDateFormat() const;

  const QString identDefaultReg() const;
  const QString identDefault() const;

  bool dirImageDefault() const;
  const QString dirDfltDownload() const;

  /* Customized Fields */
  const QString CFldName(const qint8 f = 0);
  const int CFldLength(const qint8 f = 0);
  bool CFldActive(const qint8 f = 0);
  QStringList fillList();

private:
  QString rootDir_;
  QString exportDir_;
  QString tempDir_;

  int free_;     // Free use
  int readOnly_; // Read only
  int
    roSave_; // Allows only inclusion, without alteration or removal of records

  QString identCadDflt_;
  QString identDefault_;
  QString dirDownload_;

  QString dfltRootDir_;
  QString dfltTmpDir_;

  bool dbNativeDateFormatISO_;
  bool dbNativeDateFormatEUA_;
  bool dbNativeDateFormatEUR_;

  bool bRemoveImageFromDir_;
  bool bKeepEntity_; // If 'true', do not clear the entity's label

  bool dirImgDflt_;

protected:
  void readSettings();
};

#endif
