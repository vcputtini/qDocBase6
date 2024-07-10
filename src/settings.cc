/***************************************************************************
 * Copyright (c) 2007-2020                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase6 project, a images/documents           *
 * database program.                                                       *
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

#include "settings.h"
#include "progid.h"

/*!
 * \brief Settings::Settings
 */
Settings::Settings()
{
  readSettings();
}

/*!
 * \brief Settings::~Settings
 */
Settings::~Settings() {}

/*!
 * \protected
 * \brief Settings::readSettings
 */
void
Settings::readSettings()
{
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
  rootDir_ = settings.value("rootdir", "/").toString();
  exportDir_ = settings.value("exportdir", dfltTmpDir_).toString();
  tempDir_ = settings.value("tempdir", dfltTmpDir_).toString();

  free_ = settings.value("free", true).toInt();
  readOnly_ = settings.value("readonly", false).toInt();
  roSave_ = settings.value("rosave", false).toInt();

  identCadDflt_ = settings.value("idcaddflt").toString();
  identDefault_ = settings.value("iddefault").toString();

  dirImgDflt_ = (settings.value("setDirImgDflt").toBool() ? true : false);

  dirDownload_ = settings.value("dirDownload").toString();

  bRemoveImageFromDir_ =
    (settings.value("removeImageFromDir").toBool() ? true : false);

  bKeepEntity_ = (settings.value("keepEntity").toBool() ? true : false);

  dbNativeDateFormatISO_ =
    (settings.value("dbNativeDateFormatISO").toBool() ? true : false);
  dbNativeDateFormatEUA_ =
    (settings.value("dbNativeDateFormatEUA").toBool() ? true : false);
  dbNativeDateFormatEUR_ =
    (settings.value("dbNativeDateFormatEUR").toBool() ? true : false);
}

/*!
 * \public
 * \brief Settings::getRootDir
 * \return
 */
const QString
Settings::getRootDir() const
{
  return rootDir_;
}

/*!
 * \public
 * \brief Settings::getExportDir
 * \return
 */
const QString
Settings::getExportDir() const
{
  return exportDir_;
}

/*!
 * \public
 * \brief Settings::getTempDir
 * \return
 */
const QString
Settings::getTempDir() const
{
  return tempDir_;
}

/*!
 * \public
 * \brief Settings::removeImageFromDir
 * \return
 */
bool
Settings::removeImageFromDir()
{
  return bRemoveImageFromDir_;
}

/*!
 * \public
 * \brief Settings::keepEntity
 * \return
 */
bool
Settings::keepEntity()
{
  return bKeepEntity_;
}

/*!
 * \public
 * \brief Settings::getLivre
 * \return
 */
const int
Settings::getLivre() const
{
  return free_;
}

/*!
 * \public
 * \brief Settings::getReadOnly
 * \return
 */
const int
Settings::getReadOnly() const
{
  return readOnly_;
}

/*!
 * \public
 * \brief Settings::getLimited
 * \return
 */
const int
Settings::getLimited() const
{
  return roSave_;
}

/*!
 * \public
 * \brief getDbNativeDateFormat
 * \return
 */
Globals::SupportedDateFormats
Settings::getDbNativeDateFormat() const
{
  if (dbNativeDateFormatISO_) {
    return Globals::SupportedDateFormats::ISO;
  } else if (dbNativeDateFormatEUA_) {
    return Globals::SupportedDateFormats::EUA;
  } else {
    return Globals::SupportedDateFormats::UK;
  }
}

/*!
 * \public
 * \brief Settings::identDefaultReg
 * \return
 */
const QString
Settings::identDefaultReg() const
{
  return identCadDflt_;
}

/*!
 * \public
 * \brief Settings::identDefault
 * \return
 */
const QString
Settings::identDefault() const
{
  return identDefault_;
}

/*!
 * \public
 * \brief Settings::dirImageDefault
 * \return
 */
bool
Settings::dirImageDefault() const
{
  return dirImgDflt_;
}

/*!
 * \public
 * \brief Settings::dirDfltDownload
 * \return
 */
const QString
Settings::dirDfltDownload() const
{
  return dirDownload_;
}

/*!
 * \public
 * \brief Settings::CFldName
 * \param f
 * \return
 */
const QString
Settings::CFldName(const qint8 f)
{
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
  return settings.value(QString("field0%0Name").arg(f)).toString();
}

/*!
 * \public
 * \brief Settings::CFldLength
 * \param f
 * \return
 */
const int
Settings::CFldLength(const qint8 f)
{
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
  return settings.value(QString("field0%0Len").arg(f)).toInt();
}

/*!
 * \public
 * \brief Settings::CFldActive
 * \param f
 * \return
 */
bool
Settings::CFldActive(const qint8 f)
{
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
  if (settings.value(QString("field0%0Act").arg(f)).toInt() == 1) {
    return true;
  }
  return false;
}

/*!
 * \public
 * \brief Settings::fillList
 * \return
 */
QStringList
Settings::fillList()
{
  QStringList list_;
  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());

  for (int i_ = 0; i_ < 6; ++i_) {
    if (settings.value(QString("field0%0Act").arg(i_)).toBool())
      list_ << settings.value(QString("field0%0Name").arg(i_)).toString();
  }
  if (list_.count() > 0) {
    return list_;
  }

  return QStringList();
}
