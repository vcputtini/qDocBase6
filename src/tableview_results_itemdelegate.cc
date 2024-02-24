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
#include "tableview_results_itemdelegate.h"

/*!
 * \brief TableView_Results_ItemDelegate::TableView_Results_ItemDelegate
 * \param parent
 */
TableView_Results_ItemDelegate::TableView_Results_ItemDelegate(QObject* parent)
  : QItemDelegate(parent)
{
}

/*!
 * \brief TableView_Results_ItemDelegate::updateEditorGeometry
 * \param editorWidget_
 * \param option_
 * \param index_
 */
void
TableView_Results_ItemDelegate::updateEditorGeometry(
  QWidget* editorWidget_,
  const QStyleOptionViewItem& option_,
  const QModelIndex& index_) const
{
  editorWidget_->setGeometry(option_.rect);
}

/*!
 * \brief TableView_Results_ItemDelegate::paint
 * \param painter_
 * \param option_
 * \param index_
 */
void
TableView_Results_ItemDelegate::paint(QPainter* painter_,
                                      const QStyleOptionViewItem& option_,
                                      const QModelIndex& index_) const
{
  const QString text_ =
    index_.model()->data(index_, Qt::DisplayRole).toString();
  QStyleOptionViewItem myOption_ = option_;
  myOption_.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

  if ((index_.column() == 0) || (index_.column() == 5) ||
      (index_.column() == 6)) {
    drawDisplay(painter_, myOption_, myOption_.rect, text_);
    drawFocus(painter_, myOption_, myOption_.rect);
  } else {
    QItemDelegate::paint(painter_, option_, index_);
  }
}
