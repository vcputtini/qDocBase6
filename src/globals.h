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
#ifndef GLOBALS_H
#define GLOBALS_H

#include <QByteArray>
#include <QComboBox>
#include <QFile>
#include <QFileInfo>
#include <QFuture>
#include <QFutureWatcher>
#include <QGraphicsView>
#include <QImage>
#include <QImageReader>
#include <QLocale>
#include <QMultiMap>
#include <QObject>
#include <QPair>
#include <QRadioButton>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QtConcurrent/QtConcurrentMap>
#include <QtConcurrent/QtConcurrentRun>
#include <QtConcurrent>
#include <QtMath>

#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

/*!
 * \brief The Globals class
 * Functions and constants that are used throughout the program
 */
class Globals : public QObject
{
  Q_OBJECT
public:
  explicit Globals();
  ~Globals();

  /*!
   * \enum
   * \brief The SupportedDateFormats enum
   */
  enum class SupportedDateFormats
  {
    ISO, /* yyyy-MM-dd | yyyy.MM.dd */
    EUA, /* MM/dd/yyyy */
    UK   /* dd/MM/yyyy | dd.MM.yyyy */
  };

  /*!
   * \enum
   * \brief The Categories enum
   */
  enum class Categories
  {
    Company = 0x00,
    Customer = 0x01,
    Supplier = 0x02,
    Project = 0x03,
    Custom = 0x04,
    Basic = 0x05,
    None = 0x06
  };
  static inline constexpr std::initializer_list<Categories> categories_list_ = {
    Categories::Company, Categories::Customer, Categories::Supplier,
    Categories::Project, Categories::Custom,   Categories::Basic
  };

  /*!
   * \brief The PhoneNumberModels enum
   */
  enum class PhoneNumberCategories
  {
    Phone_Business = 0x00,
    Phone_Home,
    Phone_Landline_Business,
    Phone_Landline_Home,
    Phone_WAPP,
    Phone_Others,
    Phone_NONE
  };
  static inline constexpr std::initializer_list<PhoneNumberCategories>
    catphones_list_ = { PhoneNumberCategories::Phone_Business,
                        PhoneNumberCategories::Phone_Home,
                        PhoneNumberCategories::Phone_Landline_Business,
                        PhoneNumberCategories::Phone_Landline_Home,
                        PhoneNumberCategories::Phone_WAPP,
                        PhoneNumberCategories::Phone_Others };

  /*!
   * \enum
   * \brief The ModulesOfApp enum
   */
  enum class ModulesOfApp
  {
    ModEntity = 0x00,
    ModDocuments = 0x01,
    ModSearch = 0x02,
    ModConfig = 0x03,
    ModSecurity = 0x04
  };

  /*!
   * \note Change the lines below to state acronyms
   *       from your country. If there are more or fewer states
   *       must be changed in the constructor of this class.
   */
  static constexpr const char* STATE_01 = QT_TR_NOOP("AC");
  static constexpr const char* STATE_02 = QT_TR_NOOP("AL");
  static constexpr const char* STATE_03 = QT_TR_NOOP("AP");
  static constexpr const char* STATE_04 = QT_TR_NOOP("AM");
  static constexpr const char* STATE_05 = QT_TR_NOOP("BA");
  static constexpr const char* STATE_06 = QT_TR_NOOP("CE");
  static constexpr const char* STATE_07 = QT_TR_NOOP("DF");
  static constexpr const char* STATE_08 = QT_TR_NOOP("ES");
  static constexpr const char* STATE_09 = QT_TR_NOOP("GO");
  static constexpr const char* STATE_10 = QT_TR_NOOP("MA");
  static constexpr const char* STATE_11 = QT_TR_NOOP("MT");
  static constexpr const char* STATE_12 = QT_TR_NOOP("MS");
  static constexpr const char* STATE_13 = QT_TR_NOOP("MG");
  static constexpr const char* STATE_14 = QT_TR_NOOP("PA");
  static constexpr const char* STATE_15 = QT_TR_NOOP("PB");
  static constexpr const char* STATE_16 = QT_TR_NOOP("PR");
  static constexpr const char* STATE_17 = QT_TR_NOOP("PE");
  static constexpr const char* STATE_18 = QT_TR_NOOP("PI");
  static constexpr const char* STATE_19 = QT_TR_NOOP("RJ");
  static constexpr const char* STATE_20 = QT_TR_NOOP("RN");
  static constexpr const char* STATE_21 = QT_TR_NOOP("RS");
  static constexpr const char* STATE_22 = QT_TR_NOOP("RO");
  static constexpr const char* STATE_23 = QT_TR_NOOP("RR");
  static constexpr const char* STATE_24 = QT_TR_NOOP("SC");
  static constexpr const char* STATE_25 = QT_TR_NOOP("SP");
  static constexpr const char* STATE_26 = QT_TR_NOOP("SE");
  static constexpr const char* STATE_27 = QT_TR_NOOP("TO");

  const QStringList
    stateAbbrev; // Acronyms of the federal states are assigned to this list

  static inline QMultiMap<Categories, QString> categories_map_;
  static inline QMultiMap<PhoneNumberCategories, QString> catphone_map_;

  const qint64 MAX_SIZE_IMG_16MB = 16777216;  // 16MiB
  const qint64 MAX_SIZE_IMG_2GB = 2147483648; // 2MiB

public:
  static const QByteArray unCompressImage(const QByteArray barr_);

  static const qint64 KiB();
  static const qint64 MiB();
  static const qint64 GiB();
  static const qint64 TiB();
  static const qint64 PiB();

  static const QPair<QString, QString> whatLocale();
  static const bool is_ptBR();

  static bool isExecutable(const QString fileName_);
  static void toIEC(const qreal size_,
                    QString& strFormatted_,
                    QString& numFormatted_);

  static const QString to_ptBR_Date(const QString date_);
  static const QString toISODate(const QString date_);
  static const QString toEUADate(const QString date_);

  static const QString currencySimbol();

  static const int whatCategory(QWidget* parent = nullptr);
  static const QString categoryName(Categories categ_);
  static const bool isCategoryChecked(QWidget* parent = nullptr);

  static const int indexPhoneCategory(QComboBox* cb_ = nullptr);
  static const QString phoneCategoryText(PhoneNumberCategories categ_);
  static const QString phoneCategoryText(const int categ_);

  static const qint64 extractCustomerCod(const QString&& customer_);
  static const QString extractCustomerName(const QString&& customer_);
  static const short extractProjectCod(const QString&& proj_);
  static const QString extractProjectYear(const QString&& proj_);

  static bool isValidChar(const QString& text_);

  static const QString normalizeFileName(const QString fname_);

  // Database functions -------------------------------------------------
  static bool isOperatorValid(const QString& operator_ = QString(),
                              const QString& operPasswd_ = QString());
  static const quint64 lastDocumentId();
  static const quint64 lastEntityId();
  // ---------------------------------------------------------------------

  static const QString formatProjId(const int id_ = 0, const int year_ = 0);

  static QPair<QImage, QString> loadImages(const QString fname_);

  enum class ZoomImage
  {
    In,
    Out
  };
  static void zoomImage(ZoomImage zi, QGraphicsView* gv);
};

#endif // GLOBALS_H
