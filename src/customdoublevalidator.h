#ifndef CUSTOMDOUBLEVALIDATOR_H
#define CUSTOMDOUBLEVALIDATOR_H

#include <QDoubleValidator>
#include <QObject>
#include <QString>

#include <QtMath>

#include <limits.h>
#include <math.h>

#ifndef LLONG_MAX
#define LLONG_MAX Q_INT64_C(0x7fffffffffffffff)
#endif

using namespace std;

/*!
 * \brief Valida corretamente a digitacao de numero com ponto decimal
 * \class CustomDoubleValidator
 * \author Volnei Cervi Puttini
 * \date 2011-02-17
 *
 * Faz com que os parametros top, bottol e decimals sejam rigorosamente
 * respeitados durante a digitacao do numero decimal.
 * Ou seja, se um numero for definido como por exemplo: bottom=0.0 top=9999.99
 * decimals=2 ira permitir a digitacao do numero apenas dentro dos limites
 * estabelecidos. no formato 9999.99 como dado neste exemplo.
 *
 * %ChangeLog%
 * 2013-05-23 Atualizacao para versao Qt5
 *
 */

class CustomDoubleValidator : public QDoubleValidator
{
  Q_OBJECT
public:
  CustomDoubleValidator(double bottom,
                        double top,
                        int decimals,
                        QObject* parent = 0);

  virtual QValidator::State validate(QString& input, int& pos) const;

private:
  /* Funcoes auxiliares */
  enum NumberMode
  {
    IntegerMode,
    DoubleStandardMode,
    DoubleScientificMode
  };
  bool validateChars(const QString& str,
                     NumberMode numMode,
                     QByteArray* buff,
                     int decDigits) const;

  inline int numDigits(qlonglong n) const;
  inline char digitToCLocale(const QChar& in) const;

  qlonglong pow10(int n) const;

signals:

public slots:
};

#endif // CUSTOMDOUBLEVALIDATOR_H
