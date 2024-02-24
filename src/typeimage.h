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

#ifndef TYPEIMAGE_H
#define TYPEIMAGE_H

#include <QFileInfo>
#include <QImageReader>
#include <QMap>
#include <QMapIterator>
#include <QPair>
#include <QPixmap>
#include <QString>
#include <QStringList>

#include <initializer_list>

/*!
 * \brief The TypeImage class
 */
class TypeImage
{
public:
  explicit TypeImage();
  ~TypeImage();

  const QString resourceName(const QString type_);
  QPixmap type(const QString fileName_);
  bool isImageViewable(const QString fileName_);
  QStringList stringListViewable();

private:
  static constexpr const char* rc_path_ = ":/images/png/images/%0";

  enum class SuffixFileTypes
  {
    SUFF_TXT,
    SUFF_ASC,
    SUFF_AVI,
    SUFF_DVI,
    SUFF_CDR,
    SUFF_DOC,
    SUFF_DOCX,
    SUFF_XLS,
    SUFF_XLSX,
    SUFF_PDF,
    SUFF_DWG,
    SUFF_DWS,
    SUFF_7Z,
    SUFF_ZIPX,
    SUFF_ZIP,
    SUFF_ARJ,
    SUFF_EXE,
    SUFF_PPS,
    SUFF_PPT,
    SUFF_ODT,
    SUFF_ODF,
    SUFF_ODS,
    SUFF_ODB,
    SUFF_MP3,
    SUFF_MPA,
    SUFF_JPEG,
    SUFF_JPG,
    SUFF_MPEG,
    SUFF_MPG,
    SUFF_OGG,
    SUFF_RA,
    SUFF_WAV,
    SUFF_WMA,
    SUFF_PCT,
    SUFF_DB,
    SUFF_MDF,
    SUFF_ASP,
    SUFF_ASPX,
    SUFF_HTML,
    SUFF_HTM
  };

  static constexpr std::initializer_list<SuffixFileTypes> FileTypes_List_ = {
    SuffixFileTypes::SUFF_TXT,  SuffixFileTypes::SUFF_ASC,
    SuffixFileTypes::SUFF_AVI,  SuffixFileTypes::SUFF_DVI,
    SuffixFileTypes::SUFF_CDR,  SuffixFileTypes::SUFF_DOC,
    SuffixFileTypes::SUFF_DOCX, SuffixFileTypes::SUFF_XLS,
    SuffixFileTypes::SUFF_XLSX, SuffixFileTypes::SUFF_PDF,
    SuffixFileTypes::SUFF_DWG,  SuffixFileTypes::SUFF_DWS,
    SuffixFileTypes::SUFF_7Z,   SuffixFileTypes::SUFF_ZIPX,
    SuffixFileTypes::SUFF_ZIP,  SuffixFileTypes::SUFF_ARJ,
    SuffixFileTypes::SUFF_EXE,  SuffixFileTypes::SUFF_PPS,
    SuffixFileTypes::SUFF_PPT,  SuffixFileTypes::SUFF_ODT,
    SuffixFileTypes::SUFF_ODF,  SuffixFileTypes::SUFF_ODS,
    SuffixFileTypes::SUFF_ODB,  SuffixFileTypes::SUFF_MP3,
    SuffixFileTypes::SUFF_MPA,  SuffixFileTypes::SUFF_JPEG,
    SuffixFileTypes::SUFF_JPG,  SuffixFileTypes::SUFF_MPEG,
    SuffixFileTypes::SUFF_MPG,  SuffixFileTypes::SUFF_OGG,
    SuffixFileTypes::SUFF_RA,   SuffixFileTypes::SUFF_WAV,
    SuffixFileTypes::SUFF_WMA,  SuffixFileTypes::SUFF_PCT,
    SuffixFileTypes::SUFF_DB,   SuffixFileTypes::SUFF_MDF,
    SuffixFileTypes::SUFF_ASP,  SuffixFileTypes::SUFF_ASPX,
    SuffixFileTypes::SUFF_HTML, SuffixFileTypes::SUFF_HTM
  };

  const QMap<SuffixFileTypes, QPair<QString, QString>> fileTypes_m_ = {
    { SuffixFileTypes::SUFF_TXT, { "txt", "txt-109.png" } },
    { SuffixFileTypes::SUFF_ASC, { "arc", "asc-4704.png" } },
    { SuffixFileTypes::SUFF_AVI, { "avi", "avi-10.png" } },
    { SuffixFileTypes::SUFF_DVI, { "dvi", "dvi-568.png" } },
    { SuffixFileTypes::SUFF_CDR, { "cdr", "cdr-329.png" } },
    { SuffixFileTypes::SUFF_DOC, { "doc", "doc.png" } },
    { SuffixFileTypes::SUFF_DOCX, { "docx", "docx.png" } },
    { SuffixFileTypes::SUFF_XLS, { "xls", "xls-121.png" } },
    { SuffixFileTypes::SUFF_XLSX, { "xlsx", "xlsx.png" } },
    { SuffixFileTypes::SUFF_PDF, { "pdf", "pdf.png" } },
    { SuffixFileTypes::SUFF_DWG, { "dwg", "dwg-256.png" } },
    { SuffixFileTypes::SUFF_DWS, { "dws", "dws-5288.png" } },
    { SuffixFileTypes::SUFF_7Z, { "7z", "7z-452.png" } },
    { SuffixFileTypes::SUFF_ZIPX, { "zipx", "7z-452.png" } },
    { SuffixFileTypes::SUFF_ARJ, { "arj", "arj-233.png" } },
    { SuffixFileTypes::SUFF_ZIP, { "zip", "zip-128.png" } },
    { SuffixFileTypes::SUFF_EXE, { "exe", "exe-29.png" } },
    { SuffixFileTypes::SUFF_PPS, { "pps", "pps-80.png" } },
    { SuffixFileTypes::SUFF_PPT, { "ppt", "ppt-81.png" } },
    { SuffixFileTypes::SUFF_ODT, { "odt", "odt-716.png" } },
    { SuffixFileTypes::SUFF_ODF, { "odt", "odt-716.png" } },
    { SuffixFileTypes::SUFF_ODS, { "ods", "ods-1215.png" } },
    { SuffixFileTypes::SUFF_ODB, { "odb", "odb-4855.png" } },
    { SuffixFileTypes::SUFF_MP3, { "mp3", "mp3-58.png" } },
    { SuffixFileTypes::SUFF_MPA, { "mpa", "mpa-625.png" } },
    { SuffixFileTypes::SUFF_JPEG, { "jpeg", "jpeg.png" } },
    { SuffixFileTypes::SUFF_JPG, { "jpg", "jpeg.png" } },
    { SuffixFileTypes::SUFF_PCT, { "pct", "pct-73.png" } },
    { SuffixFileTypes::SUFF_DB, { "db", "db-20.png" } },
    { SuffixFileTypes::SUFF_MDF, { "mdf", "mdf-1372.png" } },
    { SuffixFileTypes::SUFF_ASP, { "asp", "asp-8.png" } },
    { SuffixFileTypes::SUFF_ASPX, { "aspx", "aspx-9.png" } },
    { SuffixFileTypes::SUFF_HTML, { "html", "pagehtml.png" } },
    { SuffixFileTypes::SUFF_HTM, { "htm", "pagehtml.png" } }
  };

  inline const QString getRCName(const QString& type_) const;
};

#endif // TYPEIMAGE_H
