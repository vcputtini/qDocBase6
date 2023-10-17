#include "customdoublevalidator.h"

CustomDoubleValidator::CustomDoubleValidator(double bottom,
                                             double top,
                                             int decimals,
                                             QObject* parent)
  : QDoubleValidator(bottom, top, decimals, parent)
{
}

QValidator::State
CustomDoubleValidator::validate(QString& input, int& pos) const
{
  QString buff(input);

  if (buff.isEmpty())
    return QValidator::Acceptable;

  double t;
  double b;
  double d;

  QByteArray abuff;
  if (!validateChars(input, DoubleStandardMode, &abuff, decimals())) {
    return Invalid;
  }

  t = buff.section('.', 0, 0).length();
  d = buff.section('.', 1, 2).length();

  if (bottom() >= 0 && buff.startsWith('-'))
    return Invalid;

  if (top() < 0 && buff.startsWith('+'))
    return Invalid;

  if (d > decimals())
    return QValidator::Invalid;

  if (buff.toDouble() < bottom() || buff.toDouble() > top())
    return QValidator::Invalid;

  /* Testa os limites do numero
     ex: se maximo e 999999999.99 permite digitar ate 999999999.00
  */
  double i = buff.toDouble();
  b = bottom();
  t = top();

  if (notation() == StandardNotation) {
    double max = qMax(qAbs(b), qAbs(t));
    if (max < LLONG_MAX) {
      double n = qPow(numDigits(qlonglong(max)), 10);
      if (qAbs(i) > n)
        return Invalid;
    }
  }

  return QValidator::Intermediate;
}

/**
 * @bried Retorna o numero de digitos de um dado numero inteiro
 *
 */
int
CustomDoubleValidator::numDigits(qlonglong n) const
{
  return floor(log10(abs(n != 0 ? n : 1))) + 1;
}

/**
 * @bried Adaptação do codigo original dos fontes da Nokia Qt Framework Qt 4.7.1
 *        a fim de suprir algumas necessidades especificas
 *        O codigo foi alterado para minhas necessidades particularidades
 */
bool
CustomDoubleValidator::validateChars(const QString& str,
                                     NumberMode numMode,
                                     QByteArray* buff,
                                     int decDigits) const
{
  buff->clear();
  buff->reserve(str.length());

  const bool scientific = numMode == DoubleScientificMode;
  bool lastWasE = false;
  bool lastWasDigit = false;
  int decPointCnt = 0;
  bool dec = false;
  int decDigitCnt = 0;

  for (int i = 0; i < str.length(); ++i) {
    char c = digitToCLocale(str.at(i));

    if (c >= '0' && c <= '9') {
      if (numMode != IntegerMode) {
        // If a double has too many digits after decpt, it shall be Invalid.
        if (dec && decDigits != -1 && decDigits < ++decDigitCnt)
          return false;
      }
      lastWasDigit = true;
    } else {
      switch (c) {
        case ',': // nao faz nada. A virgula DEVE ter o mesmo comportamento do
                  // ponto decimal
        case '.':
          if (numMode == IntegerMode) {
            // If an integer has a decimal point, it shall be Invalid.
            return false;
          } else {
            // If a double has more than one decimal point, it shall be Invalid.
            if (++decPointCnt > 1)
              return false;
#if 0
                        // If a double with no decimal digits has a decimal point, it shall be
                        // Invalid.
                        if (decDigits == 0)
                            return false;
#endif // On second thoughts, it shall be Valid.

            dec = true;
          }
          break;

        case '+':
        case '-':
          if (scientific) {
            // If a scientific has a sign that's not at the beginning or after
            // an 'e', it shall be Invalid.
            if (i != 0 && !lastWasE)
              return false;
          } else {
            // If a non-scientific has a sign that's not at the beginning,
            // it shall be Invalid.
            if (i != 0)
              return false;
          }
          break;
        default:
          // If it's not a valid digit, it shall be Invalid.
          return false;
      }
      lastWasDigit = false;
    }

    lastWasE = c == 'e';
    if (c != ',')
      buff->append(c);
  }

  return true;
}

/**
 * @bried Adaptação do codigo original dos fontes da Nokia Qt Framework Qt 4.7.1
 *        a fim de suprir algumas necessidades especificas
 *        O codigo foi alterado para minhas necessidades particularidades
 */
char
CustomDoubleValidator::digitToCLocale(const QChar& in) const
{
  const QChar _zero = '0';
  const QChar _group = ',';
  const ushort zeroUnicode = _zero.unicode();
  const ushort tenUnicode = zeroUnicode + 10;

  if (in.unicode() >= zeroUnicode && in.unicode() < tenUnicode)
    return '0' + in.unicode() - zeroUnicode;

  if (in.unicode() >= '0' && in.unicode() <= '9')
    return in.toLatin1();

  if (in == '+')
    return '+';

  if (in == '-')
    return '-';

  if (in == '.')
    return '.';

  if (in == ',')
    return ',';

  // if (in == exponential() || in == exponential().toUpper())
  //     return 'e';

  // In several languages group() is the char 0xA0, which looks like a space.
  // People use a regular space instead of it and complain it doesn't work.
  if (_group.unicode() == 0xA0 && in.unicode() == ' ')
    return ',';

  return 0;
}

/**
 * @bried Funcao para calculo de uma potencia de 10
 */
qlonglong
CustomDoubleValidator::pow10(int exp) const
{
  qlonglong result = 1;
  for (int i = 0; i < exp; ++i)
    result *= 10;
  return result;
}
