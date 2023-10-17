/***************************************************************************
 * Copyright (c) 2007-2023                                                 *
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

/*!
 * \brief TypeImage::TypeImage
 */
TypeImage::TypeImage()
{
  initMaps();
}

/*!
 * \brief TypeImage::~TypeImage
 */
TypeImage::~TypeImage() {}

/*!
 * \private
 * \brief TypeImage::initMaps
 */
void
TypeImage::initMaps()
{
  mapTypes_["txt"] = "txt-109.png";
  mapTypes_["asc"] = "asc-4704.png";
  mapTypes_["avi"] = "avi-10.png";
  mapTypes_["dvi"] = "div-568.png";
  mapTypes_["cdr"] = "cdr-329.png";

  mapTypes_["doc"] = "doc.png";
  mapTypes_["docx"] = "docx.png";
  mapTypes_["dotx"] = "dotx-1756.png";
  mapTypes_["xls"] = "xls-121.png";
  mapTypes_["xlsx"] = "xlsx.png";

  mapTypes_["pdf"] = "pdf.png";
  mapTypes_["dwg"] = "dwg-256.png";
  mapTypes_["dws"] = "dws-5288.png";
  mapTypes_["7z"] = "7z-452.png";
  mapTypes_["zipx"] = "7z-452.png";

  mapTypes_["arj"] = "arj-233.png";
  mapTypes_["zip"] = "zip-128.png";
  mapTypes_["exe"] = "exe-29.png";
  mapTypes_["pps"] = "pps-80.png";
  mapTypes_["ppt"] = "ppt-81.png";

  mapTypes_["odt"] = "odt-716.png";
  mapTypes_["odf"] = "odt-716.png";
  mapTypes_["ods"] = "ods-1215.png";
  mapTypes_["odb"] = "odb-4855.png";
  mapTypes_["mp3"] = "mp3-58.png";
  mapTypes_["mpa"] = "mpa-625.png";

  mapTypes_["jpeg"] = "jpeg.png";
  mapTypes_["jpg"] = "jpeg.png";

  mapTypes_["mpeg"] = "mpa-625.png";
  mapTypes_["mpg"] = "mpa-625.png";
  mapTypes_["ogg"] = "ogg-413.png";
  mapTypes_["ra"] = "ra-89.png";
  mapTypes_["wav"] = "wav-113.png";
  mapTypes_["wma"] = "wma-115.png";

  mapTypes_["pct"] = "pct-73.png";
  mapTypes_["db"] = "db-20.png";
  mapTypes_["odf"] = "odf-560.png";
  mapTypes_["mdf"] = "mdf-1372.png";
  mapTypes_["asp"] = "asp-8.png";
  mapTypes_["aspx"] = "aspx-9.png";

  mapTypes_["html"] = "pagehtml.png";
  mapTypes_["htm"] = "pagehtml.png";
}

/*!
 * \public
 * \brief TypeImage::resourceName
 *        Returns the resource name based on the given file name extension
 *
 * \param type - Inform only the extension ex: doc, xls ...
 * \return
 */
const QString
TypeImage::resourceName(const QString type_)
{

  if (mapTypes_.contains(type_)) {
    return QString(":/images/png/images/%0").arg(mapTypes_.value(type_));
  } else {
    return ":/images/png/images/generic.png";
  }
}

/*!
 * \public
 * \brief TypeImage::type
 *        Returns the name of the resource based on the given file name
 * extension
 * \param fileName - full file name: ex. file.doc*
 * \return QPixmap
 */
QPixmap
TypeImage::type(const QString fileName_)
{
  QFileInfo fi(fileName_);

  QString s_ = QString();
  if (mapTypes_.contains(fi.suffix().toLower())) {
    s_ = QString(":/images/png/images/%0")
           .arg(mapTypes_.value(fi.suffix().toLower()));
  }

  QPixmap pix_;
  if (!s_.isEmpty()) {
    pix_.load(s_);
  } else {
    pix_.load(":/images/png/images/generic.png");
  }
  return pix_;
}

/*!
 * \public
 * \brief TypeImage::isImageViewable
 *        Checks if an image file is viewable within the system
 * \param fileName
 * \return true | false
 */
bool
TypeImage::isImageViewable(const QString fileName_)
{
  if (fileName_.isEmpty()) {
    return false;
  }

  QFileInfo fi_(fileName_);

  return stringListViewable().contains(fi_.suffix().toLower());
}

/*!
 * \public
 * \brief TypeImage::stringListViewable
 * Returns a QStringList() type with the extensions
 * of visible types
 * This is used in various parts of the program to
 * various purposes
 *
 * Supported native image formats
 *
 * BMP	Windows Bitmap - Read/write<br>
 * GIF	Graphic Interchange Format (optional) -	Read<br>
 * JPG	Joint Photographic Experts Group - Read/write<br>
 * JPEG	Joint Photographic Experts Group - Read/write<br>
 * PNG	Portable Network Graphics - Read/write<br>
 * PBM	Portable Bitmap	- Read<br>
 * PGM	Portable Graymap - Read<br>
 * PPM	Portable Pixmap	- Read/write<br>
 * XBM	X11 Bitmap - Read/write<br>
 * XPM	X11 Pixmap - Read/write<br>
 *
 * \return QStringList
 */
QStringList
TypeImage::stringListViewable()
{
  /**

   */
  const QStringList list_ = { "bmp", "jpg", "jpeg", "png", "pbm",
                              "pgm", "ppm", "xbm",  "xpm" };
  return list_;
}
