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
#include "messages.h"

namespace MessagesNS {

/*!
 * \brief Messages::Messages
 */
Messages::Messages()
  : msg_{}
{
  fillMap();
}

/*!
 * \brief Messages::~Messages
 */
Messages::~Messages() {}

/*!
 * \brief Messages::instance
 * \return
 */
Messages&
Messages::instance()
{
  static Messages m_;
  return m_;
}

/*!
 * \brief Messages::set
 * \param token_
 * \return *this
 */
Messages&
Messages::set(Tokens&& token_)
{
  msg_ = std::move(messages_hash_.value(token_));
  return *this;
}

/*!
 * \brief Messages::text
 * \return
 */
const QString
Messages::text() noexcept
{
  return msg_;
}

/*!
 * \brief Messages::fillMap
 */
void
Messages::fillMap()
{
  messages_hash_ = {
    { Tokens::INTERNAL_ERROR, QObject::tr(M_INTERNAL_ERROR) },
    { Tokens::FILE_NOTDELETED, QObject::tr(M_FILE_NOTDELETED) },
    { Tokens::FILE_ERROR_READING, QObject::tr(M_FILE_ERROR_READING) },
    { Tokens::FILE_ERROR_READ, QObject::tr(M_FILE_ERROR_READ) },
    { Tokens::FILE_ERROR_WRITE, QObject::tr(M_FILE_ERROR_WRITE) },
    { Tokens::FILE_ERROR_FATAL, QObject::tr(M_FILE_ERROR_FATAL) },
    { Tokens::FILE_ERROR_RESOURCE, QObject::tr(M_FILE_ERROR_RESOURCE) },
    { Tokens::FILE_ERROR_OPEN, QObject::tr(M_FILE_ERROR_OPEN) },
    { Tokens::FILE_ERROR_ABORT, QObject::tr(M_FILE_ERROR_ABORT) },
    { Tokens::FILE_ERROR_TIMEOUT, QObject::tr(M_FILE_ERROR_TIMEOUT) },
    { Tokens::FILE_ERROR_UNSPECIFIED, QObject::tr(M_FILE_ERROR_UNSPECIFIED) },
    { Tokens::FILE_ERROR_REMOVE, QObject::tr(M_FILE_ERROR_REMOVE) },
    { Tokens::FILE_ERROR_RENAME, QObject::tr(M_FILE_ERROR_RENAME) },
    { Tokens::FILE_ERROR_POSITION, QObject::tr(M_FILE_ERROR_POSITION) },
    { Tokens::FILE_ERROR_RESIZE, QObject::tr(M_FILE_ERROR_RESIZE) },
    { Tokens::FILE_ERROR_PERMISSIONS, QObject::tr(M_FILE_ERROR_PERMISSIONS) },
    { Tokens::FILE_ERROR_COPY, QObject::tr(M_FILE_ERROR_COPY) },
    { Tokens::DB_WARN_RECSAVED, QObject::tr(M_DB_RECSAVED) },
    { Tokens::DB_WARN_RECNOTFOUND, QObject::tr(M_DB_RECNOTFOUD) },
    { Tokens::DB_WARN_RECNOTSAVED, QObject::tr(M_DB_RECNOTSAVED) },
    { Tokens::DB_WARN_RECDELETED, QObject::tr(M_DB_RECDELETED) },
    { Tokens::DB_WARN_IMAGENOTSAVED, QObject::tr(M_DB_IMAGENOSAVED) },
    { Tokens::DB_WARN_RECALREADYEXISTS, QObject::tr(M_DB_RECALREADYEXISTS) },
    { Tokens::GEN_WARN_00001, QObject::tr(GEN_WARN_00001) },
    { Tokens::GEN_WARN_00002, QObject::tr(GEN_WARN_00002) },
    { Tokens::GEN_WARN_00003, QObject::tr(GEN_WARN_00003) },
    { Tokens::GEN_WARN_00004, QObject::tr(GEN_WARN_00004) },
    { Tokens::GEN_WARN_00005, QObject::tr(GEN_WARN_00005) },
    { Tokens::GEN_WARN_00006, QObject::tr(GEN_WARN_00006) },
    { Tokens::GEN_WARN_00007, QObject::tr(GEN_WARN_00007) },
    { Tokens::GEN_WARN_00008, QObject::tr(GEN_WARN_00008) },
    { Tokens::GEN_WARN_00009, QObject::tr(GEN_WARN_00009) },
    { Tokens::GEN_WARN_00010, QObject::tr(GEN_WARN_00010) },
    { Tokens::GEN_WARN_00011, QObject::tr(GEN_WARN_00011) },
    { Tokens::GEN_WARN_00012, QObject::tr(GEN_WARN_00012) },
    { Tokens::GEN_WARN_00013, QObject::tr(GEN_WARN_00013) },
    { Tokens::GEN_WARN_00014, QObject::tr(GEN_WARN_00014) },
    { Tokens::GEN_WARN_00015, QObject::tr(GEN_WARN_00015) },
    { Tokens::GEN_WARN_00016, QObject::tr(GEN_WARN_00016) },
    { Tokens::GEN_WARN_00017, QObject::tr(GEN_WARN_00017) },
    { Tokens::GEN_WARN_00018, QObject::tr(GEN_WARN_00018) },
    { Tokens::GEN_WARN_00019, QObject::tr(GEN_WARN_00019) },
    { Tokens::GEN_WARN_00020, QObject::tr(GEN_WARN_00020) },
    { Tokens::GEN_WARN_00021, QObject::tr(GEN_WARN_00021) },
    { Tokens::GEN_WARN_00022, QObject::tr(GEN_WARN_00022) },
    { Tokens::GEN_WARN_00023, QObject::tr(GEN_WARN_00023) },
    { Tokens::INTERR_0001, QObject::tr(M_INTERR_0001) },
    { Tokens::INTERR_0002, QObject::tr(M_INTERR_0002) }
  };
}

} // namespace MessagesNS
