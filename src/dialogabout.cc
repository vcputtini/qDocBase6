/***************************************************************************
 * Copyright (c) 2007-2020                                                 *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com                                                *
 *                                                                         *
 * This file is part of the qDocbase2 project, a images/documents           *
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

#include "dialogabout.h"
#include "progid.h"

DialogAbout::DialogAbout(QWidget* parent)
  : QDialog(parent)
{
  setupUi(this);

  // qDebug() << __FUNCTION__;

  graphicsViewAbout->resetTransform();

  QFont fontArial15("Arial", 15, QFont::Bold);
  QFont fontArial10("Arial", 10, QFont::Normal);
  QFontMetricsF fmArial10(fontArial10);
  QFontMetricsF fmArial15(fontArial15);

  scene_ = new QGraphicsScene;
  scene_->setSceneRect(-290, -175, 580, 350);
  graphicsViewAbout->setScene(scene_);
  scene_->clear();

  pixItemLogo_ = new QGraphicsPixmapItem;

  QColor color_;
  color_.setCmyk(60, 40, 0, 40);
  scene_->setBackgroundBrush(QBrush(color_));

  QImage img_;
  QPixmap pixTmp_;

  QImageReader reader(":/images/png/images/docbase64x64.png");
  reader.read(&img_);
  pixTmp_.convertFromImage(img_);

  QRectF rect = scene_->sceneRect();

  QTransform tf;
  tf.translate(-290, -175); // move (0,0) to the top left corner

  qreal posH_{};
  QString sText{};

  posH_ = (rect.width() - pixTmp_.width()) / 2;
  pixItemLogo_->setTransform(tf);
  pixItemLogo_->setPixmap(pixTmp_);
  pixItemLogo_->setPos(posH_, pixTmp_.height() / 4);
  scene_->addItem(pixItemLogo_);

  qreal linha = pixTmp_.height() + 20;
  textItem_ = new QGraphicsTextItem;
  sText = tr("qDocBase6: Document Database Manager");
  textItem_->setTransform(tf);
  textItem_->setFont(fontArial15);
  textItem_->setDefaultTextColor(Qt::white);
  posH_ = 10;
  textItem_->setPos(posH_, linha);
  textItem_->setPlainText(sText);
  scene_->addItem(textItem_);

  QGraphicsTextItem* text[15];
  text[0] = new QGraphicsTextItem;
  /*!
   *  \attention PLEASE, DO NOT REMOVE THIS COPYRIGHT. Just add yours!
   */
  sText = tr("Copyrigh(C)'2007-%0, Volnei Cervi Puttini <vcputtini@gmail.com>")
            .arg(QDate::currentDate().year());
  text[0]->setTransform(tf);
  text[0]->setFont(fontArial10);
  text[0]->setDefaultTextColor(Qt::black);
  posH_ = 10;
  text[0]->setPos(posH_, linha += fmArial15.lineSpacing());
  text[0]->setPlainText(sText);
  scene_->addItem(text[0]);

  text[1] = new QGraphicsTextItem;
  sText = tr("All Rights Reserved");
  text[1]->setTransform(tf);
  text[1]->setFont(fontArial10);
  text[1]->setDefaultTextColor(Qt::black);
  posH_ = 10;
  text[1]->setPos(posH_, linha += fmArial15.lineSpacing());
  text[1]->setPlainText(sText);
  scene_->addItem(text[1]);

  text[2] = new QGraphicsTextItem;
  sText = tr("Version: %0").arg(ProgId::strVersion());
  text[2]->setTransform(tf);
  text[2]->setFont(fontArial10);
  text[2]->setDefaultTextColor(Qt::black);
  posH_ = 10;
  text[2]->setPos(posH_, linha += fmArial10.lineSpacing());
  text[2]->setPlainText(sText);
  scene_->addItem(text[2]);
  QString verCC_{};
#if (defined Q_OS_LINUX) && (defined __GNUC__)
  verCC_ = tr("Compiler: GNU GCC %1.%2.%3")
             .arg(__GNUC__)
             .arg(__GNUC_MINOR__)
             .arg(__GNUC_PATCHLEVEL__);
#elif (defined Q_OS_WINDOWS)
#if Q_CC_MSVC
  verCC_ = QString("Compiler: Microsoft Visual C/C++ Versao: %0 Build: %1")
             .arg(_MSC_FULL_VER)
             .arg(_MSC_BUILD);
#elif (defined __MINGW32)
  verCC_ = tr("Compiler: MinGW32 %1.%2")
             .arg(__MINGW32_VERSION_MAJOR)
             .arg(__MINGW32_VERSION_MINOR);
#elif (defined __MINGW64)
  verCC_ = tr("Compiler: MinGW64 %1.%2")
             .arg(__MINGW64_VERSION_MAJOR)
             .arg(__MINGW64_VERSION_MINOR);
#endif
#else
  verCC_ = tr("Compiler: Undetermined");
#endif

  QString buildArch_ =
    tr("Build Arch: %0").arg(QSysInfo::buildCpuArchitecture());
  QString currArch_ =
    tr("Current Arch: %0").arg(QSysInfo::currentCpuArchitecture());
  const QString buildTimeStamp =
    QString("Built in: %0").arg(QDate::currentDate().toString());

  QStringList lText;
  lText << verCC_ << buildTimeStamp << buildArch_ << currArch_
        << QSysInfo::prettyProductName() << "" << tr("Database: MariaDB(tm)")
        << ""
        << ""
        << tr("The program is provided AS IS whith NO WARRANTY OF ANY KIND")
        << tr("INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND")
        << tr("FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.");
  int i = 2;
  linha += fmArial10.lineSpacing() * 2;

  for (const QString& s : std::as_const(lText)) {
    text[i] = new QGraphicsTextItem;
    text[i]->setTransform(tf);
    text[i]->setFont(fontArial10);
    text[i]->setDefaultTextColor(Qt::black);
    posH_ = 10;
    text[i]->setPos(posH_, linha += fmArial10.lineSpacing());
    text[i]->setPlainText(s);
    scene_->addItem(text[i]);
    ++i;
  }

  graphicsViewAbout->setScene(scene_);
}
