/***************************************************************************
 * Copyright (c) 2007-2024                                                 *
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

#include "typeimage.h"

const std::unordered_map<QString, QString> TypeImage::FILE_TYPE_ICONS = {
  { "txt", "txt-109.png" },   { "arc", "asc-4704.png" },
  { "avi", "avi-10.png" },    { "dvi", "dvi-568.png" },
  { "cdr", "cdr-329.png" },   { "doc", "doc.png" },
  { "docx", "docx.png" },     { "xls", "xls-121.png" },
  { "xlsx", "xlsx.png" },     { "pdf", "pdf.png" },
  { "dwg", "dwg-256.png" },   { "dws", "dws-5288.png" },
  { "7z", "7z-452.png" },     { "zipx", "7z-452.png" },
  { "zip", "zip-128.png" },   { "arj", "arj-233.png" },
  { "exe", "exe-29.png" },    { "pps", "pps-80.png" },
  { "ppt", "ppt-81.png" },    { "odt", "odt-716.png" },
  { "odf", "odt-716.png" },   { "ods", "ods-1215.png" },
  { "odb", "odb-4855.png" },  { "mp3", "mp3-58.png" },
  { "mpa", "mpa-625.png" },   { "jpeg", "jpeg.png" },
  { "jpg", "jpeg.png" },      { "pct", "pct-73.png" },
  { "db", "db-20.png" },      { "mdf", "mdf-1372.png" },
  { "asp", "asp-8.png" },     { "aspx", "aspx-9.png" },
  { "html", "pagehtml.png" }, { "htm", "pagehtml.png" },
  { "", "generic.png" }
};

QString
TypeImage::getRCName(const QString& type_)
{
  return QString(RC_PATH).arg(type_);
}

QString
TypeImage::resourceName(const QString& type_) const
{
  auto it = FILE_TYPE_ICONS.find(type_.toLower());
  return getRCName(it != FILE_TYPE_ICONS.end() ? it->second : "generic.png");
}

QPixmap
TypeImage::type(const QString& fileName_) const
{
  QFileInfo fileInfo(fileName_);
  return QPixmap(resourceName(fileInfo.suffix()));
}

bool
TypeImage::isImageViewable(const QString& fileName_) const
{
  return QImageReader::supportedImageFormats().contains(
    QFileInfo(fileName_).suffix().toLower().toUtf8());
}

QStringList
TypeImage::stringListViewable() const
{
  QStringList supportedFormats;
  for (const auto& format : QImageReader::supportedImageFormats()) {
    supportedFormats.append(QString::fromUtf8(format));
  }
  return supportedFormats;
}
