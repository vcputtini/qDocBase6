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

#include "multifiles.h"

/*!
 * \brief MultiFiles::MultiFiles
 * \param parent
 */
MultiFiles::MultiFiles(QObject* parent)
  : QFile(parent)
{
  Messages_ = &Messages_->instance();
}

/*!
 * \brief MultiFiles::~MultiFiles
 */
MultiFiles::~MultiFiles()
{
  baData_.clear();
}

/* --------------------------------------------------------------------- */

/*!
 * \brief MultiFilesRead_P::readFile
 * \param fname_
 * \return
 */
MultiFilesRead_P::pair_t_
MultiFilesRead_P::readFile(const QString fname_)
{
  pair_t_ aux_pair_;
  QFile fileIn_;
  fileIn_.setFileName(std::move(fname_));
  ba_.clear();

  if (!fileIn_.open(QIODevice::ReadOnly)) {
    aux_pair_.first = std::move(fileIn_.error());
    aux_pair_.second = std::move(ba_);
  } else {
    while (!fileIn_.atEnd()) {
      ba_ += std::move(fileIn_.readLine());
    }
    aux_pair_.first = std::move(QFile::NoError);
    aux_pair_.second = std::move(ba_);
    fileIn_.close();
  }
  return aux_pair_;
}

/*!
 * \brief MultiFilesWrite_P::writeFile
 * \param baOut_
 * \param fname_
 * \return
 */
int
MultiFilesWrite_P::writeFile(QByteArray baOut_, const QString fname_)
{
  QFile fileOut_;
  fileOut_.setFileName(std::move(fname_));
  if (fileOut_.open(QIODevice::WriteOnly)) {
    fileOut_.write(std::move(baOut_));
    fileOut_.close();
  }
  return fileOut_.error();
}

/* --------------------------------------------------------------------- */

/*!
 * \public
 * \brief MultiFiles::setInputFile
 * \param fIn_
 */
void
MultiFiles::setInputFile(const QString fIn_)
{
  fInput_ = std::move(fIn_);
}

/*!
 * \public
 * \brief MultiFiles::setOutputFile
 * \param fOut_
 */
void
MultiFiles::setOutputFile(const QString fOut_)
{
  fOutput_ = std::move(fOut_);
}

/*!
 * \brief MultiFiles::setFileName
 * \param fname_
 */
void
MultiFiles::setFileName(const QString& fname_)
{
  fInput_ = std::move(fname_);
}

/*!
 * \brief MultiFiles::read
 * \return
 */
const int
MultiFiles::read()
{
  pt2read = &MultiFilesRead_P::readFile;

  QFuture<QPair<int, QByteArray>> fut_ =
    QtConcurrent::run(pt2read, &MultiFilesRead_p, fInput_);

  if (fut_.result().second.size() > 0) {
    baData_ = std::move(fut_.result().second);
  }
  return fut_.result().first;
}

int
MultiFiles::write(QByteArray baOut_)
{
  pt2write = &MultiFilesWrite_P::writeFile;

  QFuture<int> future_ =
    QtConcurrent::run(pt2write, MultiFilesWrite_p, baOut_, fOutput_);
  return (future_.result());
}

/*!
 * \brief MultiFiles::execute
 */
void
MultiFiles::execute()
{
  if (!fInput_.isEmpty()) {
    if (const int e = read(); e != QFile::NoError) {
      fileError(e);
    }
  } else if (!fOutput_.isEmpty()) {

    // \todo PUT LOGIC FOR DUPLICATE FILE NAMES

    if (const int e = write(baData_); e != QFile::NoError) {
      fileError(e);
    }
  }
}

/*!
 * \public
 * \brief MultiFiles::setData
 * \param ba_
 */
void
MultiFiles::setData(QByteArray ba_)
{
  baData_.clear();
  baData_ = std::move(ba_);
}

/*!
 * \brief MultiFiles::data
 * \return
 */
QByteArray
MultiFiles::data()
{
  return baData_;
}

/*!
 * \brief MultiFiles::sizeFileInout
 * \return
 */
qint64
MultiFiles::sizeFileInout() const
{
  QFileInfo info_(std::move(fInput_));
  return info_.size();
}

/*!
 * \brief MultiFiles::sizefileOutput
 * \return
 */
qint64
MultiFiles::sizefileOutput() const
{
  QFileInfo info_(std::move(fOutput_));
  return info_.size();
}

/*!
 * \brief MultiFiles::fileExists
 * \param fname_
 * \return
 */
bool
MultiFiles::fileExists(const QString fname_)
{
  QFileInfo info_(std::move(fname_));
  return info_.exists();
}

/*!
 * \note SIGNAL
 * \brief MultiFiles::fileError
 * \param fe_
 * \return
 */
bool
MultiFiles::fileError(const int fe_)
{
  switch (fe_) {
    case QFile::ReadError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_READ).text());
      return false;
      break;
    }
    case QFile::WriteError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_WRITE).text());
      return false;
      break;
    }
    case QFile::FatalError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_FATAL).text());
      return false;
      break;
    }
    case QFile::ResourceError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_RESOURCE).text());
      return false;
      break;
    }
    case QFile::OpenError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_OPEN).text());
      return false;
      break;
    }
    case QFile::AbortError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_ABORT).text());
      return false;
      break;
    }
    case QFile::TimeOutError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_TIMEOUT).text());
      return false;
      break;
    }
    case QFile::UnspecifiedError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_UNSPECIFIED).text());
      return false;
      break;
    }
    case QFile::RemoveError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_REMOVE).text());
      return false;
      break;
    }
    case QFile::RenameError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_RENAME).text());
      return false;
      break;
    }
    case QFile::PositionError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_POSITION).text());
      return false;
      break;
    }
    case QFile::ResizeError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_RESIZE).text());
      return false;
      break;
    }
    case QFile::PermissionsError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_PERMISSIONS).text());
      return false;
      break;
    }
    case QFile::CopyError: {
      emit errorText(
        Messages_->set(MessagesNS::Tokens::FILE_ERROR_COPY).text());
      return false;
      break;
    }
    default: {
      emit errorText("");
      return true;
    }
  }
}
