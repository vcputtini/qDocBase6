#include "qdocbase.h"

#include <QApplication>
#include <QLocale>
#include <QSettings>
#include <QString>
#include <QTranslator>

#include "progid.h"

/*!
 * \internal
 * \warning DO NOT CHANGE the value of the key below as this will prevent access
 * to data in the DOCBASE6:DOCSECURITY table. If this is necessary, change the
 * content of the 'admin' user field BEFORE to the new key.
 *
 * \brief SECURITY_ENCRYPT_KEY_
 */
const char* SECURITY_ENCRYPT_KEY_ = "mE025@EZ+3.K";
// ---------------------------------------------------------------------------

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString& locale : uiLanguages) {
    const QString baseName = "qDocBase6_" + QLocale(locale).name();
    if (translator.load(QString(":/i18n/" + baseName).simplified())) {
      a.installTranslator(&translator);
      break;
    }
  }
  qDocBase w;

  QSettings settings(ProgId::strOrganization(), ProgId::strInternalName());
  if (settings.value("readonly").toBool()) {
    w.setWindowTitle(
      QString("%0 [%1]").arg(ProgId::strName(), QObject::tr("Read only")));
  } else {
    w.setWindowTitle(ProgId::strName());
  }
  w.showMaximized();
  return a.exec();
}
