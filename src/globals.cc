#include "globals.h"
#include "qcombobox.h"
#include "qimagereader.h"
#include "qsqlquery.h"
#include "settings.h"
#include "typeimage.h"
// #include "qradiobutton.h"

// STDC++
#include <tuple>
#include <utility>

/*!
 * \class
 * \brief Globals::Globals
 *
 */
Globals::Globals()
  : stateAbbrev{ STATE_01, STATE_02, STATE_03, STATE_04, STATE_05, STATE_06,
                 STATE_07, STATE_08, STATE_09, STATE_10, STATE_11, STATE_12,
                 STATE_13, STATE_14, STATE_15, STATE_16, STATE_17, STATE_18,
                 STATE_19, STATE_20, STATE_21, STATE_22, STATE_23, STATE_24,
                 STATE_25, STATE_26, STATE_27 }
{
  catphone_map_ = {
    { PhoneNumberCategories::Phone_Business, QObject::tr("Comercial") },
    { PhoneNumberCategories::Phone_Home, QObject::tr("Residencial") },
    { PhoneNumberCategories::Phone_Landline_Business,
      QObject::tr("Comercial Fixo") },
    { PhoneNumberCategories::Phone_Landline_Home,
      QObject::tr("Residencial Fixo") },
    { PhoneNumberCategories::Phone_WAPP, QObject::tr("WhatsApp") },
    { PhoneNumberCategories::Phone_Others, QObject::tr("Outras Redes Sociais") }
  };

  categories_map_ = { { Categories::Company, QObject::tr("Empresas") },
                      { Categories::Customer, QObject::tr("Clientes") },
                      { Categories::Supplier, QObject::tr("Fornecedores") },
                      { Categories::Project, QObject::tr("Projetos") },
                      { Categories::Custom, QObject::tr("Personalizados") },
                      { Categories::Basic, QObject::tr("Básico") } };
}

/*!
 * \brief Globals::~Globals
 */
Globals::~Globals() {}

/*!
 * \public
 * \brief Globals::unCompressImage
 * \param barr_
 * \return
 */
const QByteArray
Globals::unCompressImage(const QByteArray barr_)
{
  auto unCompress_ = [](const QByteArray ba_) {
    return qUncompress(std::move(ba_));
  };
  QFuture<QByteArray> future_ =
    QtConcurrent::run(unCompress_, std::move(barr_));
  QByteArray ba_result_ = std::move(future_.result());
  return ba_result_;
}

/*!
 * \public
 * \brief Globals::KiB
 * \return
 */
const qint64
Globals::KiB()
{
  return qPow(2, 10);
}

/*!
 * \public
 * \brief Globals::MiB
 * \return
 */
const qint64
Globals::MiB()
{
  return qPow(2, 20);
}

/*!
 * \public
 * \brief Globals::GiB
 * \return
 */
const qint64
Globals::GiB()
{
  return qPow(2, 30);
}

/*!
 * \public
 * \brief Globals::TiB
 * \return
 */
const qint64
Globals::TiB()
{
  return qPow(2, 40);
}

/*!
 * \public
 * \brief Globals::PiB
 * \return
 */
const qint64
Globals::PiB()
{
  return qPow(2, 50);
}

/*!
 * \public
 * \brief Globals::whatLocale
 * \return
 */
const QPair<QString, QString>
Globals::whatLocale()
{
  return qMakePair(QLocale::system().name(), QLocale().name());
}

/*!
 * \public
 * \brief Globals::is_ptBR
 * \return
 */
const bool
Globals::is_ptBR()
{
  return ((whatLocale().first == "pt_BR") && (whatLocale().second == "pt_BR"));
}

/*!
 * \public
 * \brief Globals::toIEC
 * \param size_
 * \param strFormatted_
 * \param numFormatted_
 */
void
Globals::toIEC(const qreal size_,
               QString& strFormatted_,
               QString& numFormatted_)
{
  if (size_ <= 0.0) {
    strFormatted_ = QString();
    numFormatted_ = QString();
    return;
  }

  if (size_ <= KiB()) {
    strFormatted_ = std::move(QString::number(size_) + " B");
    return;
  } else if (size_ > KiB() && size_ < MiB()) {
    strFormatted_ = std::move(QString::number(size_ / KiB(), 'f', 1) + " KiB");
    return;
  } else if (size_ >= MiB() && size_ < GiB()) {
    strFormatted_ = std::move(QString::number(size_ / MiB(), 'f', 1) + " MiB");
    return;
  } else if (size_ >= GiB() && size_ < TiB()) {
    strFormatted_ = std::move(QString::number(size_ / GiB(), 'f', 1) + " GiB");
    return;
  } else if (size_ >= TiB() && size_ < PiB()) {
    strFormatted_ = std::move(QString::number(size_ / TiB(), 'f', 1) + " TiB");
    return;
  } else {
    strFormatted_ = std::move(QObject::tr("Fora de Faixa"));
    return;
  }
}

/*!
 * \public
 * \brief Globals::isExecutable
 * \param fileName_
 * \return
 */
bool
Globals::isExecutable(const QString fileName_)
{
  QFileInfo info_(std::move(fileName_));
  const QStringList list_ = { "exe",
                              "bat",
                              "com",
                              "msi",
                              "inf",
                              "sys"
                              "bin",
                              "dll",
                              "ini",
                              "zip",
                              "7z",
                              "rar"
                              "arj",
                              "dat",
                              "rpm",
                              "deb",
                              "ocx" };
#ifdef Q_OS_LINUX
  if (info_.permission(QFile::ExeGroup | QFile::ExeOther | QFile::ExeOwner |
                       QFile::ExeUser)) {
    return true;
  }
#endif
  return list_.contains(std::move(info_.suffix().toLower()));
}

/*!
 * \public
 * \brief Globals::to_ptBR_Date
 * \note Accepted input formats:
 *          ISO yyyy-MM-dd | yyyy.MM.dd
 *          EUA MM/dd/yyyy
 *          EUR dd/MM/yyyy | dd.MM.yyyy
 * \param date_
 * \return Date in MM/dd/yyyy format, or empty string
 */
const QString
Globals::to_ptBR_Date(const QString date_)
{

  Settings settings_;
  Globals::SupportedDateFormats dateFormats_ =
    settings_.getDbNativeDateFormat();

  static const QString regexp_iso_{
    "^(?<year>[0-9]{4})([-|.])(?<month>[0-9]{2})([-|.])(?<day>[0-9]{2})$"
  };
  static const QString regexp_eua_{
    "^(?<month>[0-9]{2})([/])(?<day>[0-9]{2})([/])(?<year>[0-9]{4})$"
  };
  static const QString regexp_uk_{
    "^(?<day>[0-9]{2})([/|.])(?<month>[0-9]{2})([/|.])(?<year>[0-9]{4})$"
  };

  QString year_{};
  QString month_{};
  QString day_{};

  switch (dateFormats_) {
    case SupportedDateFormats::ISO: {
      static QRegularExpression re_(regexp_iso_);
      QRegularExpressionMatch match = re_.match(std::move(date_));
      if (match.hasMatch()) {
        month_ = std::move(match.captured("month"));
        day_ = std::move(match.captured("day"));
        year_ = std::move(match.captured("year"));
      }
      break;
    }
    case SupportedDateFormats::EUA: {
      static QRegularExpression re_(regexp_eua_);
      QRegularExpressionMatch match = re_.match(std::move(date_));
      if (match.hasMatch()) {
        month_ = std::move(match.captured("month"));
        day_ = std::move(match.captured("day"));
        year_ = std::move(match.captured("year"));
      }
      break;
    }
    case SupportedDateFormats::UK: {
      static QRegularExpression re_(regexp_uk_);
      QRegularExpressionMatch match = re_.match(std::move(date_));
      if (match.hasMatch()) {
        month_ = std::move(match.captured("month"));
        day_ = std::move(match.captured("day"));
        year_ = std::move(match.captured("year"));
      }
    }
    default: {
      return QString();
    }
  }

  return QString("%0/%1/%2")
    .arg(std::move(day_))
    .arg(std::move(month_))
    .arg(std::move(year_));
}

/*!
 * \public
 * \brief Globals::toISODate
 * \param date_
 * \return
 */
const QString
Globals::toISODate(const QString date_)
{
  if (date_.indexOf("/") == 2) {
    auto [yy_, mon_, dd_] =
      std::tuple(date_.right(4), date_.mid(3, 2), date_.left(2));
    return QString("%0-%1-%2")
      .arg(std::move(yy_))
      .arg(std::move(mon_))
      .arg(std::move(dd_));
  }
  return QString();
}

/*!
 * \brief Globals::toEUADate
 * \param date_
 * \return
 */
const QString
Globals::toEUADate(const QString date_)
{
  if (date_.indexOf("/") == 2) {
    auto [yy_, mon_, dd_] =
      std::tuple(date_.right(4), date_.mid(3, 2), date_.left(2));
    return QString("%0-%1-%2")
      .arg(std::move(mon_))
      .arg(std::move(dd_))
      .arg(std::move(yy_));
  }
  return QString();
}

/*!
 * \brief Globals::currencySimbol
 * \return
 */
const QString
Globals::currencySimbol()
{
  QLocale curr_;
  return curr_.currencySymbol();
}

/*!
 * \public
 * \brief Globals::whatCategory
 * \param parent
 * \return
 */
const int
Globals::whatCategory(QWidget* parent)
{
  QList<QRadioButton*> allObj_ = parent->findChildren<QRadioButton*>();
  for (const QRadioButton* rb_ : allObj_) {
    if (rb_->objectName() == "radioButton_Company" && rb_->isChecked()) {
      return static_cast<int>(Categories::Company);
    } else if (rb_->objectName() == "radioButton_Customer" &&
               rb_->isChecked()) {
      return static_cast<int>(Categories::Customer);
    } else if (rb_->objectName() == "radioButton_Suppliers" &&
               rb_->isChecked()) {
      return static_cast<int>(Categories::Supplier);
    } else if (rb_->objectName() == "radioButton_Projects" &&
               rb_->isChecked()) {
      return static_cast<int>(Categories::Project);
    } else if (rb_->objectName() == "radioButton_Custom" && rb_->isChecked()) {
      return static_cast<int>(Categories::Custom);
    } else if (rb_->objectName() == "radioButton_Basic" && rb_->isChecked()) {
      return static_cast<int>(Categories::Basic);
    }
  }
  return -1;
}

/*!
 * \public
 * \brief categoryName
 * \param categ_
 * \return
 */
const QString
Globals::categoryName(Categories categ_)
{
  return Globals::categories_map_.value(categ_);
}

/*!
 * \brief Globals::isCategoryChecked
 * \param parent
 * \return bool
 */
const bool
Globals::isCategoryChecked(QWidget* parent)
{
  QList<QRadioButton*> allObj_ = parent->findChildren<QRadioButton*>();
  for (const QRadioButton* rb_ : allObj_) {
    if (rb_->objectName() == "radioButton_Company" && rb_->isChecked()) {
      return true;
    } else if (rb_->objectName() == "radioButton_Customer" &&
               rb_->isChecked()) {
      return true;
    } else if (rb_->objectName() == "radioButton_Suppliers" &&
               rb_->isChecked()) {
      return true;
    } else if (rb_->objectName() == "radioButton_Projects" &&
               rb_->isChecked()) {
      return true;
    } else if (rb_->objectName() == "radioButton_Custom" && rb_->isChecked()) {
      return true;
    } else if (rb_->objectName() == "radioButton_Basic" && rb_->isChecked()) {
      return true;
    }
  }
  return false;
}

/*!
 * \public
 * \brief Globals::indexPhoneCategory
 * \param parent
 * \return
 */
const int
Globals::indexPhoneCategory(QComboBox* cb_)
{
  switch (cb_->currentIndex()) {
    case 0: {
      return static_cast<int>(PhoneNumberCategories::Phone_Business);
    }
    case 1: {
      return static_cast<int>(PhoneNumberCategories::Phone_Home);
    }
    case 2: {
      return static_cast<int>(PhoneNumberCategories::Phone_Landline_Business);
    }
    case 3: {
      return static_cast<int>(PhoneNumberCategories::Phone_Landline_Home);
    }
    case 4: {
      return static_cast<int>(PhoneNumberCategories::Phone_WAPP);
    }
    case 5: {
      return static_cast<int>(PhoneNumberCategories::Phone_Others);
    }
  }

  return static_cast<int>(PhoneNumberCategories::Phone_NONE);
}

/*!
 * \public
 * \brief Globals::phoneCategoryText
 * \param categ_
 * \return
 */
const QString
Globals::phoneCategoryText(PhoneNumberCategories categ_)
{
  return Globals::catphone_map_.value(categ_);
}

/*!
 * \public
 * \overload
 * \brief Globals::phoneCategoryText
 * \param categ_
 * \return
 */
const QString
Globals::phoneCategoryText(const int categ_)
{
  return Globals::catphone_map_.value(
    static_cast<PhoneNumberCategories>(std::move(categ_)));
}

/*!
 * \public
 * \brief Globals::extractCustomerCod
 * \param customer_
 * \return
 */
const qint64
Globals::extractCustomerCod(const QString&& customer_)
{
  return customer_.section('#', 0, 0).toLongLong();
}

/*!
 * \public
 * \brief Globals::extractCustomerName
 * \param customer_
 * \return
 */
const QString
Globals::extractCustomerName(const QString&& customer_)
{
  return customer_.section('#', 1, 1);
}

/*!
 * \public
 * \brief Globals::extractProjectCod
 * \param proj_
 * \return
 */
const short
Globals::extractProjectCod(const QString&& proj_)
{
  return proj_.section('-', 0, 0).toShort();
}

/*!
 * \public
 * \brief Globals::extractProjectYear
 * \param proj_
 * \return
 */
const QString
Globals::extractProjectYear(const QString&& proj_)
{
  return proj_.section('-', 1, 1);
}

/*!
 * \public
 * \brief isValidChar
 * \param text_
 * \return bool
 *
 * \note If we do not use 'static' when creating the RE, the compiler
 * will issue the warning message: "Don't create temporary
 * QRegularExpression objects. Use the static QRegularExpression object
 * instead [clazy-use-static-qregularexpression]". This can be considered
 * as something similar to a code optimization because if we use 'static'
 * the RE will not be recreated with each function call, as can be seen
 * in:
 * https://github.com/KDE/clazy/blob/master/docs/checks/README-use-static-qregularexpression.md
 *
 */
bool
Globals::isValidChar(const QString& text_)
{
  static QRegularExpression re_("[!@#&$%+=:;<>{}\\[\\]\\^\\~\\|\\\"]");
  QRegularExpressionMatch match_ = re_.match(std::move(text_));
  if (re_.isValid()) {
    if (match_.hasMatch()) {
      return true;
    }
  }
  return false;
}

/*!
 * \public
 * \brief Globals::normalizeFileName
 * \param fname_
 * \return
 */
const QString
Globals::normalizeFileName(const QString fname_)
{
  QString aux_ = std::move(fname_.trimmed());
  aux_.replace(QString(" "), QString("_"));

  return aux_;
}

/*!
 * \public
 * \brief isOperatorValid
 *  Informing both Operator and Password parameters, it searches if the
 *  informed pair is correct. If you inform only the Operator, it confirms
 * or non-existence of the name in the register.* \note The SQL statement
 * uses the BINARY parameter in the WHERE clause in order to make it
 * case-sensitive.
 *
 * \param operator_
 * \param operPasswd_ (optional)
 * \return true | false
 */
bool
Globals::isOperatorValid(const QString& operator_, const QString& operPasswd_)
{
  if (operator_.isEmpty() || operator_ == "admin") {
    return false;
  }

  QString stmnt_{};
  if (operPasswd_.isEmpty()) {
    stmnt_ = "SELECT OP_NAME FROM DOCSECURITY WHERE BINARY OP_NAME=:OP_NAME";
  } else {
    stmnt_ = "SELECT OP_NAME FROM DOCSECURITY WHERE BINARY OP_NAME=:OP_NAME "
             "AND OP_PASSWD=DES_ENCRYPT(:OP_PASSWD,'mE025@EZ+3.K')";
  }

  QSqlQuery qry_;
  qry_.prepare(stmnt_);
  qry_.bindValue(":OP_NAME", std::move(operator_.simplified()));
  if (!operPasswd_.isEmpty()) {
    qry_.bindValue(":OP_PASSWD", std::move(operPasswd_.simplified()));
  }

  if (qry_.exec()) {
    qry_.next();
    return (qry_.value(0).toString().isEmpty() ? false : true);
  }
  return false;
}

/*!
 * \public
 * \brief Globals::lastDocumentId
 * \return
 */
const quint64
Globals::lastDocumentId()
{
  QSqlQuery qry_;
  qry_.prepare("SELECT MAX(ST_IDENT) FROM DOCSTORAGE");
  if (qry_.exec()) {
    qry_.next();
    return qry_.value(0).toLongLong();
  }
  return 1;
}

/*!
 * \public
 * \brief Globals::lastEntityId
 * \return
 */
const quint64
Globals::lastEntityId()
{
  QSqlQuery qry_;
  qry_.prepare("SELECT MAX(ET_IDENT) FROM DOCENTITY");
  if (qry_.exec()) {
    qry_.next();
    return qry_.value(0).toLongLong();
  }
  return 1;
}

/*!
 * \public
 * \brief Globals::formatProjId
 * \param id_
 * \param year_
 * \return
 */
const QString
Globals::formatProjId(const int id_, const int year_)
{
  return QString("%0-%1").arg(id_).arg(year_);
}

/*!
 * \public
 * \brief Globals::loadImages
 * \param fname_
 * \return
 */
QPair<QImage, QString>
Globals::loadImages(const QString fname_)
{
  TypeImage tImages_;
  QFileInfo fi_(std::move(fname_));
  QString ext_ = std::move(fi_.suffix().toLower());
  QImage img_;
  QSize sz_;

  if (tImages_.isImageViewable(std::move(fname_))) {
    QImageReader reader(std::move(fname_));
    reader.read(&img_);
    if (img_.size().height() < 100)
      sz_ = std::move(QSize(img_.size().width(), 500));
    else
      sz_ = std::move(img_.size());
  } else {
    img_.load(tImages_.resourceName(ext_));
    sz_ = std::move(QSize(64, 64));
  }

  return (qMakePair(img_.scaled(sz_, Qt::KeepAspectRatio), ext_));
}

/*!
 * \public
 * \brief Globals::zoomImage
 * \param zi
 * \param gv
 */
void
Globals::zoomImage(ZoomImage zi, QGraphicsView* gv)
{
  switch (zi) {
    case ZoomImage::In:
      if (gv->transform().m11() < 2.15)
        gv->scale(1.2, 1.2);
      else {
        gv->resetTransform();
        gv->scale(.5, .5);
      }
      break;
    case ZoomImage::Out:
      if (gv->transform().m11() > 0.02) {
        gv->scale(.5, .5);
      } else {
        gv->resetTransform();
        gv->scale(.2, .2);
      }
      break;
    default:
      Q_UNREACHABLE();
  }
}
