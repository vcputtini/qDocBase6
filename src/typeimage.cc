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

/*!
 * \brief TypeImage::TypeImage
 */
TypeImage::TypeImage() {}

/*!
 * \brief TypeImage::~TypeImage
 */
TypeImage::~TypeImage() {}

/*!
 * \private
 * \brief TypeImage::getRCName
 * \param type_
 * \return resource file name
 */
const QString
TypeImage::getRCName(const QString& type_) const
{
  for (auto it_begin_ = fileTypes_m_.constBegin(),
            it_end_ = fileTypes_m_.constEnd();
       it_begin_ != it_end_;
       ++it_begin_) {
    const auto key_ = it_begin_.key();
    if (fileTypes_m_[key_].first == type_) {
      return fileTypes_m_[key_].second; // file name
    }
  }

  return fileTypes_m_[SuffixFileTypes::GENERIC].second;
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
  return QString(rc_path_).arg(std::move(getRCName(type_)));
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
  QPixmap pix_;
  pix_.load(resourceName(fi.suffix().toLower()));
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

  if (qsizetype suff_idx_ = fileName_.lastIndexOf("."); suff_idx_ > 0) {
    const auto suff_str_ = fileName_.mid(suff_idx_ + 1, fileName_.size());
    return stringListViewable().contains(suff_str_);
  }
  return false;
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
 * BMP	Windows Bitmap - Read/write
 * GIF	Graphic Interchange Format (optional) -	Read
 * JPG	Joint Photographic Experts Group - Read/write
 * JPEG	Joint Photographic Experts Group - Read/write
 * PNG	Portable Network Graphics - Read/write
 * PBM	Portable Bitmap	- Read
 * PGM	Portable Graymap - Read
 * PPM	Portable Pixmap	- Read/write
 * XBM	X11 Bitmap - Read/write
 * XPM	X11 Pixmap - Read/write
 *
 * \return QStringList
 */
QStringList
TypeImage::stringListViewable()
{
  const QStringList list_ = { "bmp", "gif", "jpg", "jpeg", "png",
                              "pbm", "pgm", "ppm", "xbm",  "xpm" };
  return list_;
}
