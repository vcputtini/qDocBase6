#include "lineedit_fieldchecking.h"
#include "globals.h"
#include "progid.h"
#include "qmessagebox.h"

/*!
 * \brief lineEdit_fieldChecking::lineEdit_fieldChecking
 * \param parent
 */
lineEdit_fieldChecking::lineEdit_fieldChecking(QWidget* parent)
  : QLineEdit(parent)
{
  Messages_ = &Messages_->instance();
}

/*!
 * \brief lineEdit_fieldChecking::~lineEdit_fieldChecking
 */
lineEdit_fieldChecking::~lineEdit_fieldChecking() {}

/*!
 * \protected
 * \brief lineEdit_fieldChecking::keyPressEvent
 * \param event_
 */
void
lineEdit_fieldChecking::keyPressEvent(QKeyEvent* event_)
{
  // Class handler to allow editing events.
  QLineEdit::keyPressEvent(event_);

  if (event_->type() == QEvent::KeyPress) {
    QKeyEvent* key_ = static_cast<QKeyEvent*>(event_);

    if ((key_->key() == Qt::Key_Enter) || (key_->key() == Qt::Key_Return)) {
      focusNextChild();
      return;
    }
  }
}

/*!
 * \public
 * \brief lineEdit_fieldChecking::setFieldDescription
 * \param text_
 */
void
lineEdit_fieldChecking::setFieldDescription(const QString& text_)
{
  descr_ = std::move(text_);
}

/*!
 * \public
 * \brief lineEdit_fieldChecking::emptyField
 * \return
 */
bool
lineEdit_fieldChecking::emptyField()
{
  if (text().isEmpty()) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00006).text())
        .arg(descr_),
      QMessageBox::Close);
    return true;
  }
  return false;
}

/*!
 * \public
 * \brief lineEdit_fieldChecking::invalidChars
 * \return
 */
bool
lineEdit_fieldChecking::invalidChars()
{
  if (Globals::isValidChar(text().simplified())) {
    QMessageBox::warning(
      this,
      ProgId::Name,
      QString(Messages_->set(MessagesNS::Tokens::GEN_WARN_00007).text())
        .arg(descr_),
      QMessageBox::Close);
    return true;
  }
  return false;
}
