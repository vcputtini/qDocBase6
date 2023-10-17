#include "customuppervalidator.h"

/*!
 * \brief CustomUpperValidator::CustomUpperValidator
 * \param parent
 */
CustomUpperValidator::CustomUpperValidator(QObject* parent)
  : QValidator{ parent }
{
}

/*!
 * \brief CustomUpperValidator::validate
 * \param input_
 * \param pos_
 * \return
 */
QValidator::State
CustomUpperValidator::validate(QString& input_, int& pos_) const
{
  input_ = input_.toUpper();
  return QValidator::Acceptable;
}
