#include "br_cnpj.h"

/*!
 * \brief BR_CNPJ::BR_CNPJ
 * \param data_
 */
BR_CNPJ::BR_CNPJ() {}

/*!
 * \brief BR_CNPJ::data
 * Returns the CNPJ value passed via the type's constructor.
 * Allows the user to choose between two formats
 * of presentation
 * \param format
 * \return QString(CNPJ)
 */
const QString
BR_CNPJ::data(DataFormats format_)
{
  if (valid_) {
    stDelimiter stDel_[][2]{ { 2, "." }, { 6, "." }, { 10, "/" }, { 15, "-" } };

    switch (format_) {
      case NumericOnly:
        if (data_.count(".") == 2 && data_.count("/") == 1 &&
            data_.count("-") == 1) {
          data_.replace(".", "").replace("/", "").replace("-", "");
        }
        break;
      case Delimited:
        if (data_.length() == 14) {
          for (int i_ = 0; i_ < 4; i_++) {
            data_.insert(stDel_[i_][0].pos_, stDel_[i_][0].delemiter_);
          }
        }
    }
  }
  return data_;
}

/*!
 * \brief BR_CNPJ::VD
 * \return integer representing VD
 */
const int
BR_CNPJ::VD()
{
  return calcVD();
}

/*!
 * \brief BR_CNPJ::section
 * Returns the significant parts of the CNPJ
 * via a structure.
 *
 * Members:
 * first = Registration Number
 * secont = Head office or branch number
 * vd = verifying digit value
 *
 * \return struct Section
 */
BR_CNPJ::Section
BR_CNPJ::section()
{
  Section sec;
  if (valid_) {
    sec.first_ = std::move(data_.left(8));
    sec.second_ = std::move(data_.mid(8, 4));
    sec.vd_ = std::move(data_.right(2));
  } else {
    sec.first_ = "";
    sec.second_ = "";
    sec.vd_ = "";
  }
  return sec;
}

/*!
 * \protected
 * \brief BR_CNPJ::normalize
 *
 * Performs several checks with the entered value
 * in order to ensure that the value is valid for
 * be calculated. In general try to make sure
 * the value, and if it is correct, convert it to the
 * internal format to be calculated.
 */
void
BR_CNPJ::normalize()
{

  if (isNullOrEmpty()) {
    valid_ = false;
    return;
  }

  /* Tests whether the passed cnpj is composed of
   * numbers in the range of 00000000000000 to 99999999999999
   */
  for (int i = 0; i < 10; i++) {
    if (data_.count(QString::number(i)) == data_.length()) {
      valid_ = false;
      data_ = "BR_CNPJ::invalid";
      return;
    }
  }

  if (data_.length() < 14 || data_.length() > 18) {
    valid_ = false;
    data_ = "BR_CNPJ::invalid";
    return;
  }

  if (data_.contains(".") && data_.contains("/") &&
      data_.contains("-")) { // perhaps formated
    if (data_.count(".") == 2 && data_.count("/") == 1 &&
        data_.count("-") == 1) { // right
      valid_ = true;
      data_.replace(".", "").replace("/", "").replace("-", "");
      return;
    } else { // wrong
      valid_ = false;
      data_ = "BR_CNPJ::invalid";
      return;
    }
  }

  valid_ = true;
}

/*!
 * \protected
 * \brief CNPJ::calcVD
 * Compute the value of the check digit
 * \return inteiro com o valor do DV
 */
const int
BR_CNPJ::calcVD() const
{
  if (data_.isEmpty()) {
    return false;
  }

  QString aux_ = std::move(data_.left(12));

  // seed
  int weight1_[12] = { 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2 };
  int weight2_[13] = { 6, 5, 4, 3, 2, 9, 8, 7, 6, 5, 4, 3, 2 };

  /* DV 1 */
  int p_[12];
  int sum_{ 0 };
  for (int i_ = 0; i_ < 12; i_++) {
    p_[i_] = aux_.mid(i_, 1).toInt() * weight1_[i_];
    sum_ += std::move(p_[i_]);
  }
  int mod1_ = std::move(sum_ % 11);
  int dig1_{ 0 };
  if (mod1_ < 2) {
    dig1_ = 0;
  } else {
    dig1_ = std::move(11 - mod1_);
  }

  /* DV 2 */
  aux_ += std::move(QString::number(dig1_)); // join VD 1
  sum_ = 0;
  int p1_[13];
  for (int i_ = 0; i_ < 13; i_++) {
    p1_[i_] = aux_.mid(i_, 1).toInt() * weight2_[i_];
    sum_ += std::move(p1_[i_]);
  }
  int mod2_ = std::move(sum_ % 11);
  int dig2_ = 0;
  if (mod2_ < 2) {
    dig2_ = 0;
  } else {
    dig2_ = std::move(11 - mod2_);
  }

  return QString("%0%1").arg(dig1_).arg(dig2_).toInt();
}
