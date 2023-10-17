#include "customlowervalidator.h"

/*!
 * \brief CustomLowerValidator::CustomLowerValidator
 * \param parent
 */
CustomLowerValidator::CustomLowerValidator(QObject* parent)
  : QValidator(parent)
{
}

/*!
 * \brief CustomLowerValidator::validate
 * \param input
 * \param pos
 * \return
 */
QValidator::State
CustomLowerValidator::validate(QString& input, int& pos) const
{
  input = input.toLower();
  return QValidator::Acceptable;
}
