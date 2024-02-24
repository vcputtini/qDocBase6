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
#include "filefilters_p.h"

FileFilters_P::FileFilters_P(QWidget* parent)
  : QComboBox(parent)
  , lineEdit_Filter_(new QLineEdit)
{
  lineEdit_Filter_->setMaxLength(255);
  lineEdit_Filter_->setClearButtonEnabled(true);
  setLineEdit(lineEdit_Filter_);
  defaultItems();
}

FileFilters_P::~FileFilters_P() {}

void
FileFilters_P::defaultItems()
{
  clear();

#ifdef Q_OS_LINUX
  QString filter_ = "*";
#else
  QString filter_ = "*.*";
#endif

  insertItems(18,
              QStringList() << (settings_.dirImageDefault()
                                  ? settings_.getRootDir() + "/" + filter_
                                  : filter_)
                            << "*.pdf"
                            << "*.jpg"
                            << "*.jpeg"
                            << "*.odf"
                            << "*.odf"
                            << "*.odg"
                            << "*.txt"
                            << "*.docx"
                            << "*.doc"
                            << "*.xlsx"
                            << "*.xls"
                            << "*.pptx"
                            << "*.ppt"
                            << "*.dwg"
                            << "*.zip");
  setCurrentIndex(0);
}
