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
#include "combobox_phones_p.h"

/*!
 * \class comboBox_Phones_P
 * \brief comboBox_Phones_P::comboBox_Phones_P
 * \param parent
 */
comboBox_Phones_P::comboBox_Phones_P(QWidget* parent)
  : QComboBox(parent)
{
  clear();
  addItem(QPixmap(":/images/png/images/celular50x48.png"),
          tr("Celular Comercial"));
  addItem(QPixmap(":/images/png/images/celular50x48.png"),
          tr("Celular Residêncial"));
  addItem(QPixmap(":/images/png/images/celular50x48.png"),
          tr("Fixo Comercial"));
  addItem(QPixmap(":/images/png/images/celular50x48.png"),
          tr("Fixo Residêncial"));
  /*!
   * \note Icons "whatsapp-transparent.png" and "others.png" were downloaded
   * respectively from sites "https://www.pngwing.com/en/search?q=whatsapp" and
   * "https://icon-icons.com/pt/icone/outros/155053"
   */
  addItem(QPixmap(":/images/png/images/whatsapp-transparent.png"),
          tr("WhatsApp"));

  addItem(QPixmap(":/images/png/images/others.png"),
          tr("Outras Redes Sociais"));

  setCurrentIndex(0);
}

/*!
 * \brief comboBox_Phones_P::~comboBox_Phones_P
 */
comboBox_Phones_P::~comboBox_Phones_P() {}
